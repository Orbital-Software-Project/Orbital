#pragma once

#include "Texture.h"

#include <string>
#include <opencv2/core.hpp>
#include <GL/glew.h>

#include <memory>

#include <openvslam/publish/frame_publisher.h>
#include <openvslam/publish/map_publisher.h>

namespace Orb {

class Global {

public: // Singleton
    static Global& GetInstance()
    {
        static Global instance;
        return instance;
    }
private:
    Global() {}

public:
    Global(Global const&) = delete;
    void operator=(Global const&) = delete;


public:
    std::shared_ptr<openvslam::publish::map_publisher> MapPublisher;

    std::shared_ptr<openvslam::publish::frame_publisher> FramePublisher;

};

}
