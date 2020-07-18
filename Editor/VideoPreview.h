#pragma once


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <GL/glew.h>

#include "Texture.hpp"

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
