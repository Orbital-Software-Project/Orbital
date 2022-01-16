#pragma once

#include "Engine/Texture.h"
#include "Engine/IEntity.h"
#include "Engine/SceneRenderer.h"

#include <memory>

namespace Orb {

class EditorState {

public: // Singleton
    static EditorState& GetInstance()
    {
        static EditorState instance;

        return instance;
    }
private:
    EditorState() {
        this->Renderer = std::make_shared<SceneRenderer>();

        this->VideoFrame = std::make_shared<Texture>();

        this->SequencerFrame = std::make_shared<Texture>();
    }

public:
    EditorState(EditorState const&) = delete;
    void operator=(EditorState const&) = delete;

    std::shared_ptr<SceneRenderer> Renderer;

    std::shared_ptr<Texture> VideoFrame;

    std::shared_ptr<Texture> SequencerFrame;

    std::string RootDir = "";

};

}
