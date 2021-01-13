#include "Video.h"


namespace Orb {

Video::Video() {
    this->SetName("Video");
    this->init();
}

Video::Video(std::string filepath) {
    this->SetName("Video");
    this->init();
    this->UpdateVideo(filepath);
}

Video::~Video() {
    if (this->Data.isOpened()) {
        this->Data.release();
    }
}

std::string Video::GetFilePath() {
    return this->FilePath;
}

void Video::UpdateVideo(std::string data) {
    
    this->FilePath = data;
    this->Data = cv::VideoCapture(data);

    if (this->Data.isOpened()) {
        this->numFrames = this->Data.get(cv::CAP_PROP_FRAME_COUNT);
    }


    this->isCurrTextureUpdated = false;
}

void Video::init() {
    this->isCurrTextureUpdated = false;
    this->currFrame = std::make_shared<Texture>();
}


int Video::GetFrameIdx() {
    return this->currFrameIdx;
}

void Video::SetFrameIdx(int idx) {
    if (idx >= this->numFrames || idx < 0) {
        return;
    }

    if (this->currFrameIdx < this->numFrames) {
        this->isCurrTextureUpdated = false;
        this->currFrameIdx = idx;
    }

}

int Video::GetNumFrames() {
    return this->numFrames;
}

bool Video::HasFrames() {
    return this->numFrames > 0;
}

cv::Mat Video::ReadFrameByIndex(int idx) {

    if (idx >= this->numFrames) {
        return cv::Mat();
    }

    if (!this->Data.isOpened()) {
        return cv::Mat();
    }

    this->Data.set(cv::CAP_PROP_POS_FRAMES, idx);
    cv::Mat result;

    this->Data.read(result);
    this->Data.set(cv::CAP_PROP_POS_FRAMES, this->currFrameIdx);

    return result;
}

void Video::Draw(std::shared_ptr<Shader> shader) {

}

IEntity::EntityType Video::GetEntityType() {
    return EntityType::Video;
}


}
