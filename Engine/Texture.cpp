#include "Texture.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

namespace Orb {

Texture::Texture() {
    this->init();
}

Texture::Texture(std::string file) {
    this->filePath = file;

    this->init();
    cv::Mat imageData = cv::imread(file.c_str());
    this->UpdateColorMap(imageData);
}

Texture::Texture(cv::Mat colorMap) {

    this->init();
    this->UpdateColorMap(colorMap);
}

Texture::~Texture() {
    if(this->initialized) {
        glDeleteTextures(1, &this->textureID);
    }
    this->colorMap.release();
}

const cv::Mat Texture::GetColorMap() {
    return this->colorMap;
}

std::string Texture::GetFilePath() {
    return this->filePath;
}

GLuint Texture::GetTextureID() {
    return this->textureID;
}

void Texture::UpdateColorMap(cv::Mat newColorMap) {
    if(this->initialized) {

        // Rotate texture
        cv::rotate(newColorMap, this->colorMap, cv::ROTATE_90_CLOCKWISE);

        glBindTexture(GL_TEXTURE_2D, this->textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set texture clamping method
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set image data alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, this->colorMap.step / this->colorMap.elemSize());

        int cols = this->colorMap.cols;
        int rows = this->colorMap.rows;

        glTexImage2D(GL_TEXTURE_2D,       // Type of texture
                     0,                   // Pyramid level (for mip-mapping) - 0 is the top level
                     GL_RGB,              // Internal colour format to convert to
                     cols,                // Image width  i.e. 640 for Kinect in standard mode
                     rows,                // Image height i.e. 480 for Kinect in standard mode
                     0,                   // Border width in pixels (can either be 1 or 0)
                     GL_BGR,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                     GL_UNSIGNED_BYTE,    // Image data type
                     this->colorMap.ptr() // The actual image data itself
                     );

        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture::Bind() {
    if(this->initialized) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->textureID);
    }
}

void Texture::init() {
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenTextures(1, &this->textureID);
    this->initialized = this->textureID > 0;
}

}
