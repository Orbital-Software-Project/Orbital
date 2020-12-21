#pragma once

#include <string>
#include <mutex>
#include <memory>
#include <viso_mono.h>

#include "TaskWorker.h"

namespace Orb {

    struct Viso2TaskParam {

        Viso2TaskParam() : Viso2TaskParam(0, 0, 0, "") {}

        Viso2TaskParam(int cx, int cy, int f, std::string filePath) : CX(cx), CY(cy), F(f), FilePath(filePath) {}

        int CX = 0;

        int CY = 0;

        int F = 0;

        std::string FilePath = "";
    };

    class Viso2Task : public ITask {

    public:
        struct Viso2TaskReport : public ITask::ITaskReport {

            std::vector<Matrix> CameraPoses;

        };

        Viso2Task(Viso2TaskParam param);

        ~Viso2Task();

        void Run();

        ITaskReport* GetReport();

    private:
        bool requestCancel = false;
        bool pauseTask = false;
        
        std::vector<Matrix> cameraPoses;

        Viso2TaskParam param;

        std::atomic<int> frameCount = 0;
        std::atomic<int> currFrameIdx = 0;

        Viso2TaskReport taskReport;

    };
}
