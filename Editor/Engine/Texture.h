#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <GL/glew.h>

#include "Shader.h"

#include <iostream>

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

    void UpdateTexture(cv::Mat data);

    void Bind(std::shared_ptr<Shader> shader);

public:
    GLuint ID = 0;

    cv::Mat Data = cv::Mat();

    std::string FilePath = "";

private:
    void init();
    void updateOpenGLBuffer();

private:
    bool initialized = false;
    bool openglBufferUpdateRequired = false;


};

}
