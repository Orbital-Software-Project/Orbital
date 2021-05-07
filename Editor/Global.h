#pragma once

#include "Engine/Texture.h"
#include "Engine/IEntity.h"
#include "Engine/SceneRenderer.h"

#include <memory>

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

        this->VideoFrame = std::make_shared<Texture>();

        this->SequencerFrame = std::make_shared<Texture>();
    }

public:
    Global(Global const&) = delete;
    void operator=(Global const&) = delete;

    std::shared_ptr<SceneRenderer> Renderer;

    std::shared_ptr<Texture> VideoFrame;

    std::shared_ptr<Texture> SequencerFrame;

    std::mutex GlobalMutex;

    std::string RootDir = "";

};

}
