#pragma once

#include <opencv2/core.hpp>
#include <GL/glew.h>

namespace Orb {

class Texture {

public:
    Texture() {
        this->init();
    }

    Texture(cv::Mat colorMap) {
        this->init();
        this->UpdateColorMap(this->colorMap);
    }

    ~Texture() {
        if(this->initialized) {
            glDeleteTextures(1, &this->textureID);
        }
        this->colorMap.release();
    }

    const cv::Mat GetColorMap() {
        return this->colorMap;
    }

    const GLuint GetTextureID() {
        return this->textureID;
    }

    void UpdateColorMap(cv::Mat newColorMap) {
        this->colorMap = newColorMap.clone();

        glBindTexture(GL_TEXTURE_2D, this->textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set texture clamping method
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexImage2D(GL_TEXTURE_2D,        // Type of texture
                     0,                    // Pyramid level (for mip-mapping) - 0 is the top level
                     GL_RGB,               // Internal colour format to convert to
                     this->colorMap.cols,  // Image width  i.e. 640 for Kinect in standard mode
                     this->colorMap.rows,  // Image height i.e. 480 for Kinect in standard mode
                     0,                    // Border width in pixels (can either be 1 or 0)
                     GL_RGB,               // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                     GL_UNSIGNED_BYTE,     // Image data type
                     this->colorMap.ptr()  // The actual image data itself
                     );
    }

private:
    void init() {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glGenTextures(1, &this->textureID);
        this->initialized = this->textureID > 0;
    }

private:
    cv::Mat colorMap = cv::Mat();
    GLuint textureID = 0;
    bool initialized = false;

};

}
