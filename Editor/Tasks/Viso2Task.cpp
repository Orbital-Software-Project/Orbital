#include "Viso2Task.h"

#include "Editor/Base/ScopeMutexLock.hpp"

#include <chrono>
#include <thread>


#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include "Editor/Global.h"
#include "Engine/PrimitiveFactory.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Orb {

Viso2Task::Viso2Task(Viso2TaskParam param) {
    this->param = param;

    this->slamCam = std::make_shared<Camera>();
    this->slamCam->SetName("Slam camera");
    this->slamCam->VisibleInOutliner = true;
    EditorState::GetInstance().Renderer->AddEntity(this->slamCam);


}

Viso2Task::~Viso2Task() {
    this->Cancel();
}

void Viso2Task::Run() {
    this->Status = TaskStatus::Running;

    //VisualOdometryMono::parameters param;

    // calibration parameters for sequence 2010_03_09_drive_0019
    // param.calib.f = 1034.650696; // focal length in pixels
    // param.calib.cu = 959.500000;  // principal point (u-coordinate) in pixels
    // param.calib.cv = 539.500000;  // principal point (v-coordinate) in pixels
    // param.motion_threshold = std::numeric_limits<double>::max();

    //param.calib.f  = this->param.F;
   // param.calib.cu = this->param.CX;
    //param.calib.cv = this->param.CY;
    //param.motion_threshold = std::numeric_limits<double>::max();

   // VisualOdometryMono* viso = new VisualOdometryMono(param);

    cv::VideoCapture videoCapture(this->param.FilePath, cv::CAP_ANY);

    if (!videoCapture.isOpened()) {
        this->Status = TaskStatus::Errored;
        return;
    }

    cv::Mat currFrame;
    uint8_t* img_data = nullptr;
    //Matrix currPose = Matrix::eye(4);

    this->frameCount = videoCapture.get(cv::CAP_PROP_FRAME_COUNT);
    cv::Mat frameConverted;

    while (videoCapture.read(currFrame)) {

        if (this->PauseRequested) {
            this->Status = TaskStatus::Paused;
        }

        this->PauseLoop();

        if (this->CancelRequested) {
            this->Status = TaskStatus::Canceled;
            break; // get out of loop
        }

        
        cv::cvtColor(currFrame, frameConverted, cv::COLOR_RGB2GRAY);

        if (img_data == nullptr) {
            img_data = (uint8_t*)malloc(static_cast<size_t>(frameConverted.rows) * static_cast<size_t>(frameConverted.cols) * sizeof(uint8_t));
        }

        int32_t k = 0;
        for (int32_t u = 0; u < frameConverted.rows; u++) {
            for (int32_t v = 0; v < frameConverted.cols; v++) {
                img_data[k] = frameConverted.at<uchar>(u, v);
                k++;
            }
        }

        int32_t dims[] = { frameConverted.cols, frameConverted.rows, frameConverted.cols };

        //if (viso->process(img_data, dims, false)) {
          //  currPose = currPose * Matrix::inv(viso->getMotion());


            //this->cameraPoses.push_back(currPose);

            {
                if (EditorState::GetInstance().VideoFrame == nullptr) {
                    EditorState::GetInstance().VideoFrame = std::make_shared<Texture>();
                }

                EditorState::GetInstance().VideoFrame->UpdateTexture(currFrame);

                //std::cout << currPose << std::endl;

                for (int m = 0; m < 4; m++) {
                    for (int n = 0; n < 4; n++) {
                        //this->slamCam->Matrix[n][m] = currPose.val[m][n];
                    }
                }
            }

            std::cout << glm::to_string(this->slamCam->Matrix) << std::endl;
            this->slamCam->Matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f)) * this->slamCam->Matrix;


       // } else {

       // }


        // Update report every 50 frames
        {
            if (this->currFrameIdx % 10 == 0) {
                ScopeMutexLock lock(this->TaskReportMutex);
                this->taskReport.Percentage = Percentage(this->frameCount, this->currFrameIdx);
                //this->taskReport.CameraPoses = this->cameraPoses;
            }
        }
        

        this->currFrameIdx++;

    }


    // Last report
    {
        ScopeMutexLock lock(this->TaskReportMutex);
        this->taskReport.Percentage = Percentage(this->frameCount, this->currFrameIdx);
        //this->taskReport.CameraPoses = this->cameraPoses;
    }




    if (img_data != nullptr) {
        free(img_data);
    }

    if (!this->CancelRequested) {
        this->Status = TaskStatus::Finished;
    }
}

Viso2Task::ITaskReport* Viso2Task::GetReport() {
    ScopeMutexLock lock(this->TaskReportMutex);

    return &this->taskReport;
    
}

}


