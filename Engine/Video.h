#pragma once


#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>


#include "Shader.h"
#include "Texture.h"
#include "IEntity.h"

namespace Orb {

class Video : public IEntity {

public:
    Video();

    Video(std::string file);

    ~Video();

    std::string GetFilePath();

    void UpdateVideo(std::string data);

    void Draw(std::shared_ptr<Shader> shader);

    EntityType GetEntityType();

    int GetFrameIdx();

    void SetFrameIdx(int idx);

    int GetNumFrames();

    bool HasFrames();

    cv::Mat ReadFrameByIndex(int idx);
    
    void SetSyncCurrFrameIdxSequencer(bool sync) {
        this->syncWithSequencer = sync;
    }

    bool SyncCurrFrameIdxSequencer() {
        return this->syncWithSequencer;
    }


public:
    std::string FilePath = "";
    cv::VideoCapture Data;
    std::shared_ptr<Texture> currFrame = nullptr;

private:
    void init();

private:
    int numFrames = 0;
    int currFrameIdx = 0;
    bool isCurrTextureUpdated = false;
    bool syncWithSequencer = false;

};

}
