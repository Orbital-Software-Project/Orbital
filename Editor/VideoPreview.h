#pragma once


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <GL/glew.h>

#include "Texture.h"

namespace Orb {

class VideoPreview {
public:
    VideoPreview();
    ~VideoPreview();

    void OnRender();

private:
    Texture videoFrame;

};

}
