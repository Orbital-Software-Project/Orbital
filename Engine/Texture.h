#pragma once

#include <opencv2/core.hpp>
#include <GL/glew.h>

namespace Orb {

class Texture {

public:
    Texture();

    Texture(cv::Mat colorMap);

    ~Texture();

    const cv::Mat GetColorMap();

    const GLuint GetTextureID();

    void UpdateColorMap(cv::Mat newColorMap);

private:
    void init();

private:
    cv::Mat colorMap = cv::Mat();
    GLuint textureID = 0;
    bool initialized = false;

};

}
