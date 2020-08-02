#pragma once

#include <openvslam/system.h>
#include <openvslam/config.h>
#include <openvslam/publish/frame_publisher.h>
#include <openvslam/publish/map_publisher.h>

#include <string>
#include <opencv2/core.hpp>

#include <mutex>
#include <memory>

#include "Mesh.h"
#include "MeshImporter.h"

namespace Orb {

enum class TaskStatus {
    Created,
    Running,
    Canceled,
    Finished,
    Errored
};

struct TaskReport {
    TaskReport() : NumFrames(0), FramesProcessed(0), Status(TaskStatus::Created) {}

    unsigned int FramesProcessed;

    unsigned int NumFrames;

    TaskStatus Status;
};

class SlamTask {
public:
    SlamTask() {}

    SlamTask(std::string videoFile, std::string configFile, std::string vocabFile);

    ~SlamTask();

    void Run();

    void Cancel();

    TaskReport GetReport();

    std::shared_ptr<openvslam::publish::map_publisher> GetMapPublisher();

    std::shared_ptr<openvslam::publish::frame_publisher> GetFramePublisher();

private:
    void doSlamAsync();

private:
    TaskReport report;
    std::string videoFile = "", configFile = "", vocabFile = "";
    std::thread slamTask;
    bool requestCancel = false;
    std::mutex reportMutex;

    std::shared_ptr<openvslam::config> cfg_ptr;
    std::unique_ptr<openvslam::system> SLAM;
    std::shared_ptr<openvslam::publish::map_publisher> mapPublisher;
    std::shared_ptr<openvslam::publish::frame_publisher> framePublisher;


};
}
