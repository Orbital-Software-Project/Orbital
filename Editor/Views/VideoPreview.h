#pragma once

#include "Engine/Texture.h"

#include "Editor/Views/IView.hpp"


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <memory>
#include <GL/glew.h>


namespace Orb {

class VideoPreview : public IView {

public:
    VideoPreview();

    ~VideoPreview();

    void OnRender();

};

}
