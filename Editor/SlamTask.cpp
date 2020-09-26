#include "SlamTask.h"

#include <opencv2/videoio.hpp>

namespace Orb {

SlamTask::SlamTask(std::string videoFile, std::string configFile, std::string vocabFile) {
    this->videoFile = videoFile;
    this->configFile = configFile;
    this->vocabFile = vocabFile;


}

SlamTask::~SlamTask() {
    this->Cancel();
}

void SlamTask::Run() {
    this->cfg_ptr = std::make_shared<openvslam::config>(this->configFile);

    this->SLAM = std::make_unique<openvslam::system>(cfg_ptr, this->vocabFile);
    this->SLAM->startup();

    this->framePublisher = this->SLAM->get_frame_publisher();
    this->mapPublisher   = this->SLAM->get_map_publisher();

    this->requestCancel = false;
    this->report.Status = TaskStatus::Running;
    this->slamTask      = std::thread(&SlamTask::doSlamAsync, this);

    this->slamTask.detach();
}



void SlamTask::Cancel() {
    if (this->report.Status == TaskStatus::Running) {

        this->pauseTask = false;
        this->requestCancel = true;

        this->SLAM->request_terminate();

        this->slamTask.join();
        this->report.Status == TaskStatus::Canceled;

    }
}

void  SlamTask::Pause() {
    this->pauseTask = true;
}

bool SlamTask::IsPaused() {
    return this->pauseTask;
}

void  SlamTask::Resume() {
    this->pauseTask = false;
}

TaskReport SlamTask::GetReport() {
    this->reportMutex.lock();
    TaskReport rep(this->report);
    this->reportMutex.unlock();
    return rep;
}

std::shared_ptr<openvslam::publish::map_publisher> SlamTask::GetMapPublisher() {
    return this->mapPublisher;
}

std::shared_ptr<openvslam::publish::frame_publisher> SlamTask::GetFramePublisher() {
    return this->framePublisher;
}

void SlamTask::doSlamAsync() {

    cv::VideoCapture video(this->videoFile);
    unsigned int frameCount = video.get(cv::CAP_PROP_FRAME_COUNT);

    std::vector<double> track_times;
    cv::Mat frame;
    double timestamp = 0.0;
    int frame_skip = 1;
    unsigned int num_frame = 0;
    bool is_not_end = true;

    while (is_not_end) {
        if(this->requestCancel) {
            break;
        }

        while (this->pauseTask) {
            std::this_thread::sleep_for(std::chrono::microseconds(5000));
        }

        is_not_end = video.read(frame);

        const auto tp_1 = std::chrono::steady_clock::now();

        if (!frame.empty() && (num_frame % frame_skip == 0)) {
            // input the current frame and estimate the camera pose
            SLAM->feed_monocular_frame(frame, timestamp, cv::Mat());
        }

        const auto tp_2 = std::chrono::steady_clock::now();

        const auto track_time = std::chrono::duration_cast<std::chrono::duration<double>>(tp_2 - tp_1).count();
        if (num_frame % frame_skip == 0) {
            track_times.push_back(track_time);
        }

        timestamp += 1.0 / cfg_ptr->camera_->fps_;
        ++num_frame;

        // check if the termination of SLAM system is requested or not
        if (SLAM->terminate_is_requested()) {
            break;
        }

        // Create report
        this->reportMutex.lock();

        this->report = TaskReport();
        this->report.FramesProcessed = num_frame;
        this->report.NumFrames = frameCount;
        this->report.Status = TaskStatus::Running;

        this->reportMutex.unlock();

    }

    // wait until the loop BA is finished
    while (SLAM->loop_BA_is_running()) {
        std::this_thread::sleep_for(std::chrono::microseconds(5000));
    }

    // shutdown the SLAM process
    SLAM->shutdown();

    // Do not output trajectories etc. data
    if(this->requestCancel) {
        this->report.Status = TaskStatus::Canceled;
        return;
    }

    if (false /*eval_log*/) {
        //output the trajectories for evaluation
        SLAM->save_frame_trajectory("frame_trajectory.txt", "TUM");
        SLAM->save_keyframe_trajectory("keyframe_trajectory.txt", "TUM");
        //output the tracking times for evaluation
        std::ofstream ofs("track_times.txt", std::ios::out);
        if (ofs.is_open()) {
            for (const auto track_time : track_times) {
                ofs << track_time << std::endl;
            }
            ofs.close();
        }
    }

    //if (!map_db_path.empty()) {
    // output the map database
    //  SLAM.save_map_database(map_db_path);
    //}

    std::sort(track_times.begin(), track_times.end());
    const auto total_track_time = std::accumulate(track_times.begin(), track_times.end(), 0.0);
    std::cout << "median tracking time: " << track_times.at(track_times.size() / 2) << "[s]" << std::endl;
    std::cout << "mean tracking time: " << total_track_time / track_times.size() << "[s]" << std::endl;

    this->report.Status = TaskStatus::Finished;
}

}


