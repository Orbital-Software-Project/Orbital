#pragma once

#include <stella_vslam/system.h>
#include <stella_vslam/config.h>
#include <stella_vslam/publish/frame_publisher.h>
#include <stella_vslam/publish/map_publisher.h>
#include <stella_vslam/data/keyframe.h>
#include <stella_vslam/data/landmark.h>

#include <string>
#include <memory>

#include "Editor/Tasks/TaskWorker.h"
#include "Editor/Engine/Vertex.hpp"
#include "Editor/Engine/Mesh.h"
#include "Editor/Engine/Camera.h"
#include "Editor/Engine/Video.h"
#include "Editor/Engine/EntityGroup.hpp"
#include "Editor/Engine/FrameTransformation.hpp"

namespace Orb {

class OpenVslamTask : public ITask {

public:

    struct OpenVslamTaskReport : public ITask::ITaskReport {
        OpenVslamTaskReport() {}
    };

    OpenVslamTask() { }

    OpenVslamTask(std::string videoFile, std::string configFile, std::string vocabFile);

    ~OpenVslamTask();

    void Run();

    ITaskReport* GetReport();

private:
    void updateReport(int frameCount, int num_frame);
    void updatePointCloud();
    void updateKeyframes();
    void updateCameraPos();
    void updateVideoPlane(float width, float height, float depth);

private:
    OpenVslamTaskReport report;

    std::string videoFile = "", configFile = "", vocabFile = "";

    std::shared_ptr<stella_vslam::config> cfg_ptr;
    std::unique_ptr<stella_vslam::system> SLAM;
    std::shared_ptr<stella_vslam::publish::map_publisher> mapPublisher;
    std::shared_ptr<stella_vslam::publish::frame_publisher> framePublisher;

    std::shared_ptr<Video> videoEnt = nullptr;
    std::shared_ptr<EntityGroup> entGroup = nullptr;
    std::shared_ptr<Mesh> pointCloud = nullptr;
    std::shared_ptr<Mesh> keyframes = nullptr;
    std::shared_ptr<Texture> videoTexture = nullptr;
    std::shared_ptr<Mesh> videoPlane = nullptr;
    std::shared_ptr<Camera> slamCam = nullptr;
    std::shared_ptr<FrameTransformation> cameraTransformation = nullptr;

};
}
