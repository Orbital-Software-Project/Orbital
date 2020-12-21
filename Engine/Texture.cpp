#include "Texture.h"


#include <opencv2/imgproc.hpp>
#include <iostream>

namespace Orb {

Texture::Texture() {
    this->init();
}

Texture::Texture(std::string filepath) {
    this->init();

    this->FilePath = filepath;
    this->UpdateTexture(cv::imread(filepath));
}

Texture::Texture(cv::Mat colorMap) {
    this->init();

    this->UpdateTexture(colorMap);
}

Texture::~Texture() {
    if(this->initialized) {
        glDeleteTextures(1, &this->ID);
    }
}

const cv::Mat Texture::GetColorMap() {
    return this->Data;
}

std::string Texture::GetFilePath() {
    return this->FilePath;
}

GLuint Texture::GetTextureID() {

    if (!this->initialized) {
        this->init();
    }

    if (this->openglBufferUpdateRequired) {
        this->updateOpenGLBuffer();
    }

    return this->ID;
}

void Texture::UpdateTexture(cv::Mat data) {
    this->Data = data.clone();
    this->openglBufferUpdateRequired = true;
}

void Texture::Bind(std::shared_ptr<Shader> shader) {
    // Should only by called from the ui thread

    if(!this->initialized) {
        this->init();
    }

    if (this->initialized) {
        shader->SetInt("Texture", 0);
        // Only update buffer when required and binded in ui thread
        if (this->openglBufferUpdateRequired) {
            this->openglBufferUpdateRequired = false;
            this->updateOpenGLBuffer();
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->ID);
    }
}

void Texture::init() {

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenTextures(1, &this->ID);
    this->initialized = this->ID > 0;
}

void Texture::updateOpenGLBuffer() {
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set image data alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, this->Data.step / this->Data.elemSize());

    int cols = this->Data.cols;
    int rows = this->Data.rows;

    glTexImage2D(GL_TEXTURE_2D, // Type of texture
        0,                      // Pyramid level (for mip-mapping) - 0 is the top level
        GL_RGB,                 // Internal colour format to convert to
        cols,                   // Image width  i.e. 640 for Kinect in standard mode
        rows,                   // Image height i.e. 480 for Kinect in standard mode
        0,                      // Border width in pixels (can either be 1 or 0)
        GL_BGR,                 // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
        GL_UNSIGNED_BYTE,       // Image data type
        this->Data.ptr()        // The actual image data itself
    );

    glGenerateMipmap(GL_TEXTURE_2D);
}

}
