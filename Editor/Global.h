#pragma once

#include "Texture.h"

#include <string>
#include <opencv2/core.hpp>
#include <GL/glew.h>

#include <memory>

#include <openvslam/publish/frame_publisher.h>
#include <openvslam/publish/map_publisher.h>

#include "IEntity.h"

#include <viso_mono.h>

#include "Engine/SceneRenderer.h"

namespace Orb {

class Global {

public: // Singleton
    static Global& GetInstance()
    {
        static Global instance;

        return instance;
    }
private:
    Global() {
        this->Renderer = std::make_shared<SceneRenderer>();
    }

public:
    Global(Global const&) = delete;
    void operator=(Global const&) = delete;

    std::shared_ptr<SceneRenderer> Renderer;

    std::shared_ptr<Texture> VideoFrame;

    std::mutex GlobalMutex;

};

}
