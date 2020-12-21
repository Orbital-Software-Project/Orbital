#pragma once

#include "Texture.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <memory>
#include <GL/glew.h>


namespace Orb {

class VideoPreview {

public:
    VideoPreview();

    ~VideoPreview();

    void OnRender();

};

}
