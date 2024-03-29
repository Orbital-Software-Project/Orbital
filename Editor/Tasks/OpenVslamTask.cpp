#include "OpenVslamTask.h"

#include "Editor/Base/ScopeMutexLock.hpp"

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>



#include <chrono>
#include <thread>
#include <mutex>

#include <opencv2/core.hpp>

#include <Eigen/Core>

#include <glm/ext/matrix_transform.hpp>

#include "Editor/Global.h"
#include "Editor/Utils.hpp"
#include "Editor/Engine/PrimitiveFactory.h"


namespace Orb {

    


    OpenVslamTask::OpenVslamTask(std::string videoFile, std::string configFile, std::string vocabFile) {
        this->videoFile  = videoFile;
        this->configFile = configFile;
        this->vocabFile  = vocabFile;


        this->entGroup = std::make_shared<EntityGroup>();
        entGroup->SetName("stella_vslam reconstruction");
        EditorState::GetInstance().Renderer->AddEntity(entGroup);


        this->videoEnt = std::make_shared<Video>(videoFile);
        this->videoEnt->SetSyncCurrFrameIdxSequencer(true);
        entGroup->AddEntity(this->videoEnt);


        this->pointCloud = std::make_shared<Mesh>();
        this->pointCloud->SetName("Pointcloud");
        this->pointCloud->VisibleInOutliner = true;
        this->pointCloud->SetPolygonMode(GL_POINTS);
        this->pointCloud->DrawOnlyVertColors(true);
        entGroup->AddEntity(this->pointCloud);
        

        this->keyframes = std::make_shared<Mesh>();
        this->keyframes->SetName("Keyframes");
        this->keyframes->VisibleInOutliner = true;
        this->keyframes->DrawOnlyVertColors(true);
        this->keyframes->SetPolygonMode(GL_LINE_STRIP);
        entGroup->AddEntity(this->keyframes);


        this->slamCam = std::make_shared<Camera>();
        this->slamCam->SetName("Camera");
        this->slamCam->VisibleInOutliner = true;
        entGroup->AddEntity(this->slamCam);

        this->cameraTransformation = std::make_shared<FrameTransformation>();
        this->cameraTransformation->SetName("Camera transformation");
        this->cameraTransformation->SetEntity(this->slamCam);
        entGroup->AddEntity(this->cameraTransformation);
    }

    OpenVslamTask::~OpenVslamTask() {
        this->Cancel();
    }

    void OpenVslamTask::Run() {

        this->Status = TaskStatus::Running;

        cfg_ptr = std::make_shared<stella_vslam::config>(this->configFile);
        SLAM = std::make_unique<stella_vslam::system>(cfg_ptr, this->vocabFile);
        SLAM->startup();

        framePublisher = SLAM->get_frame_publisher();
        mapPublisher   = SLAM->get_map_publisher();


        cv::VideoCapture video(this->videoFile);

        if (!video.isOpened()) {
            this->Status = TaskStatus::Errored;
            return;
        }

        unsigned int frameCount = video.get(cv::CAP_PROP_FRAME_COUNT);

        std::vector<double> track_times;
        cv::Mat frame;
        double timestamp = 0.0;
        int frame_skip = 1;
        unsigned int num_frame = 0;

        while (video.read(frame)) {

            this->PauseLoop();

            if (this->CancelRequested) {
                this->Status = TaskStatus::Canceled;
                SLAM->request_terminate();
                break;
            }

            // Extra check if terminate is requested, maybe by something else
            if (SLAM->terminate_is_requested()) {
                break;
            }

            const auto tp_1 = std::chrono::steady_clock::now();

            if (!frame.empty() && (num_frame % frame_skip == 0)) {
                // input the current frame and estimate the camera pose
                SLAM->feed_monocular_frame(frame, timestamp, cv::Mat());
            }

            this->videoEnt->SetFrameIdx(num_frame);

            const auto tp_2 = std::chrono::steady_clock::now();
            const auto track_time = std::chrono::duration_cast<std::chrono::duration<double>>(tp_2 - tp_1).count();

            if (num_frame % frame_skip == 0) {
                track_times.push_back(track_time);
            }

            timestamp += 1.0 / cfg_ptr->camera_->fps_;
            ++num_frame;

            {

                this->updatePointCloud();
                this->updateKeyframes();

                // Update camera matrix and store transformation per frame
                this->updateCameraPos();
                this->cameraTransformation->SetFrameTransformation(num_frame, this->slamCam->Matrix);
               
                this->updateVideoPlane(frame.cols, frame.rows, 2.5f);

                // Update video preview
                EditorState::GetInstance().VideoFrame = this->videoTexture;
            }

            this->updateReport(frameCount, num_frame);
        }

        // wait until the loop BA is finished
        while (SLAM->loop_BA_is_running()) {
            std::this_thread::sleep_for(std::chrono::microseconds(2000));
        }

        
        // Update last report
        this->updateReport(frameCount, num_frame);

        // shutdown the SLAM process
        SLAM->shutdown();
        

        if (false) {

            //output the trajectories for evaluation
            SLAM->save_frame_trajectory("frame_trajectory.txt", "TUM");
            SLAM->save_keyframe_trajectory("keyframe_trajectory.txt", "TUM");

            //output the tracking times for evaluation
            /*
            std::ofstream ofs("track_times.txt", std::ios::out);
            if (ofs.is_open()) {
                for (const auto track_time : track_times) {
                    ofs << track_time << std::endl;
                }
                ofs.close();
            }
            */

        }

        //if (!map_db_path.empty()) {
        // output the map database
        //  SLAM.save_map_database(map_db_path);
        //}

        std::sort(track_times.begin(), track_times.end());
        const auto total_track_time = std::accumulate(track_times.begin(), track_times.end(), 0.0);
        std::cout << "median tracking time: " << track_times.at(track_times.size() / 2) << "[s]" << std::endl;
        std::cout << "mean tracking time: " << total_track_time / track_times.size() << "[s]" << std::endl;

        this->Status = TaskStatus::Finished;

    }

    ITask::ITaskReport* OpenVslamTask::GetReport() {
        ScopeMutexLock lock(this->TaskReportMutex);

        return &this->report;
    }


    void OpenVslamTask::updateReport(int frameCount, int num_frame) {

        ScopeMutexLock lock(this->TaskReportMutex);
        this->report.Percentage = Percentage(frameCount, num_frame);

    }
    
    void OpenVslamTask::updatePointCloud() {
        
        std::vector<std::shared_ptr<stella_vslam::data::landmark>> allLandmarks;
        std::set< std::shared_ptr<stella_vslam::data::landmark>> localLandmarks;
        mapPublisher->get_landmarks(allLandmarks, localLandmarks);


        // Small performance optimization
        // Do not update the pointcloud for every frame
        static int allLandMarkSize = 0;
        int currSize = allLandmarks.size();
        if (currSize == allLandMarkSize) {
            return;
        }
        allLandMarkSize = currSize;



        if (allLandmarks.size() == 0) {
            return;
        }

        std::vector<Vertex> vertices;
        for (auto currLandMark : allLandmarks) {
            if (!currLandMark || currLandMark->will_be_erased()) {
                continue;
            }

            if (localLandmarks.count(currLandMark)) {
                continue;
            }

            Eigen::Vector3d pos = currLandMark->get_pos_in_world();

            Vertex vertLandmarks({
                                     pos.cast<float>().eval().x(),
                                     pos.cast<float>().eval().y(),
                                     pos.cast<float>().eval().z()
                }, {
                    1.0f,
                    1.0f,
                    0.0f
                });

            vertices.push_back(vertLandmarks);
        }

        for (auto currLandMark : allLandmarks) {
            if (!currLandMark || currLandMark->will_be_erased()) {
                continue;
            }

            Eigen::Vector3d pos = currLandMark->get_pos_in_world();
            Vertex vertAllLandmarks({
                                        pos.cast<float>().eval().x(),
                                        pos.cast<float>().eval().y(),
                                        pos.cast<float>().eval().z()
                }, {
                    0.0f,
                    0.0f,
                    1.0f
                });

            vertices.push_back(vertAllLandmarks);

        }

        this->pointCloud->UpdateColored(vertices);
    }

    void OpenVslamTask::updateKeyframes() {

        std::vector< std::shared_ptr<stella_vslam::data::keyframe>> keyFrames;
        mapPublisher->get_keyframes(keyFrames);

        // Small performance optimization
        // Do not update the pointcloud for every frame
        static int keyFrameSize = 0;
        int currSize = keyFrames.size();
        if (currSize == keyFrameSize) {
            return;
        }
        keyFrameSize = currSize;

        std::vector<Vertex> vertices;
        const auto draw_edge = [&](const stella_vslam::Vec3_t& cam_center_1, const stella_vslam::Vec3_t& cam_center_2) {
            vertices.push_back(
                Vertex({
                           cam_center_1.cast<float>().x(),
                           cam_center_1.cast<float>().y(),
                           cam_center_1.cast<float>().z()
                    }, {
                        0.0f, 1.0f, 1.0f
                    })
            );

            vertices.push_back(
                Vertex({
                           cam_center_2.cast<float>().x(),
                           cam_center_2.cast<float>().y(),
                           cam_center_2.cast<float>().z()
                    }, {
                        0.0f, 1.0f, 1.0f
                    })
            );
        };

        for (const auto keyFrame : keyFrames) {
            if (!keyFrame || keyFrame->will_be_erased()) {
                continue;
            }

            const stella_vslam::Vec3_t cam_center_1 = keyFrame->get_cam_center();

            // covisibility graph
            bool drawCovisibilityGraph = true;
            if (drawCovisibilityGraph) {

                const auto covisibilities = keyFrame->graph_node_->get_covisibilities_over_weight(100);
                if (!covisibilities.empty()) {
                    for (const auto covisibility : covisibilities) {
                        if (!covisibility || covisibility->will_be_erased()) {
                            continue;
                        }
                        if (covisibility->id_ < keyFrame->id_) {
                            continue;
                        }
                        const stella_vslam::Vec3_t cam_center_2 = covisibility->get_cam_center();
                        draw_edge(cam_center_1, cam_center_2);
                    }
                }
            }

            // spanning tree
            bool drawSpanningTree = false;
            if (drawSpanningTree) {

                auto spanning_parent = keyFrame->graph_node_->get_spanning_parent();
                if (spanning_parent) {
                    const stella_vslam::Vec3_t cam_center_2 = spanning_parent->get_cam_center();
                    draw_edge(cam_center_1, cam_center_2);
                }
            }

            // loop edges

            bool drawLoopEdge = false;
            if (drawLoopEdge) {

                const auto loop_edges = keyFrame->graph_node_->get_loop_edges();
                for (const auto loop_edge : loop_edges) {
                    if (!loop_edge) {
                        continue;
                    }
                    if (loop_edge->id_ < keyFrame->id_) {
                        continue;
                    }
                    const stella_vslam::Vec3_t cam_center_2 = loop_edge->get_cam_center();
                    draw_edge(cam_center_1, cam_center_2);
                }
            }
        }

        this->keyframes->UpdateColored(vertices);

    }

    void OpenVslamTask::updateCameraPos() {

        //http://ogldev.atspace.co.uk/index.html

        // Get camera pose and convert it to opengl conform matrix (glm)
        Eigen::Matrix4f camera_pos = Eigen::Matrix4f::Identity();
        camera_pos = mapPublisher->get_current_cam_pose().inverse().eval().cast<float>();
        glm::mat4 converted = Utils::ToGLM_Mat4f(camera_pos);

        // With inverse i get the correct pos, and
        // Translation is correct
        // slamcam is correct except wrong rotation

        // https://stackoverflow.com/questions/63429179/eigen-and-glm-products-produce-different-results

        this->slamCam->Matrix = converted;

    }

    void OpenVslamTask::updateVideoPlane(float width, float height, float depth) {
        // https://stackoverflow.com/questions/46578529/how-to-compute-the-size-of-the-rectangle-that-is-visible-to-the-camera-at-a-give

        // calc box
        float aspect = width / height;

        width = 1;
        height = width / aspect;

        float t = std::tan(glm::radians(45.0f) / 2);
        float newHeight = t * depth;
        float newWidth = newHeight * aspect;

        // Prevent the image from flipping
        if (newHeight > 0) {
            newHeight *= -1;
        }

        if (this->videoPlane.get() == nullptr) {
            this->videoPlane = PrimitiveFactory::SizedPlane(newWidth, newHeight);
            this->videoPlane->SetName("Video plane");
            this->videoPlane->VisibleInOutliner = true;
            
            this->videoTexture = std::make_shared<Texture>();
            this->videoPlane->Textures.push_back(this->videoTexture);

            this->entGroup->AddEntity(this->videoPlane);
        }

        if (!this->videoPlane->Visible) {
            return;
        }

        glm::mat4 newMat = glm::translate(this->slamCam->Matrix, glm::vec3(0.0f, 0.0f, depth));
        this->videoPlane->Matrix = newMat;

        this->videoTexture->UpdateTexture(framePublisher->draw_frame());
    }

}


