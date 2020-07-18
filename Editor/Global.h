#pragma once

#include "Texture.hpp"

#include <string>
#include <opencv2/core.hpp>
#include <GL/glew.h>

#include <memory>

#include <openvslam/publish/frame_publisher.h>
#include <openvslam/publish/map_publisher.h>


namespace Orb {

class Global {

public:
    inline static cv::Mat VideoFrame;

    inline static int NumFrames = 0;

    inline static int FramesProcessed = 0;

    inline static std::string CurrentVideoFile = "";

    inline static std::shared_ptr<openvslam::publish::map_publisher> MapPublisher;

    inline static std::shared_ptr<openvslam::publish::frame_publisher> FramePublisher;

};

}
