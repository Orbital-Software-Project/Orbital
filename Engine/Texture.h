#pragma once

#include <opencv2/core.hpp>
#include <GL/glew.h>

namespace Orb {

class Texture {

public:
    Texture();

    Texture(std::string file);

    Texture(cv::Mat colorMap);

    ~Texture();

    const cv::Mat GetColorMap();

    GLuint GetTextureID();

    std::string GetFilePath();

    void UpdateColorMap(cv::Mat newColorMap);

    void Bind();

private:
    void init();

private:
    std::string filePath = "";
    cv::Mat colorMap = cv::Mat();
    GLuint textureID = 0;
    bool initialized = false;

};

}
