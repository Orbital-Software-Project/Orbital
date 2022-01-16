#pragma once

#include <openvslam/system.h>
#include <openvslam/config.h>
#include <openvslam/publish/frame_publisher.h>
#include <openvslam/publish/map_publisher.h>
#include <openvslam/data/keyframe.h>
#include <openvslam/data/landmark.h>

#include <string>
#include <memory>

#include "Editor/Tasks/TaskWorker.h"

#include "Engine/Vertex.hpp"
#include "Engine/Mesh.h"
#include "Engine/Camera.h"
#include "Engine/Video.h"
#include "Engine/EntityGroup.hpp"
#include "Engine/FrameTransformation.hpp"

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

    std::shared_ptr<openvslam::config> cfg_ptr;
    std::unique_ptr<openvslam::system> SLAM;
    std::shared_ptr<openvslam::publish::map_publisher> mapPublisher;
    std::shared_ptr<openvslam::publish::frame_publisher> framePublisher;

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
