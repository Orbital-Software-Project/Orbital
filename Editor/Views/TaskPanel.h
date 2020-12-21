#pragma once

#include <string>
#include <memory>

namespace Orb {

    class TaskPanel {

    public:
        TaskPanel();

        ~TaskPanel();

        void OnRender();

    private:
        void processfinishedTasks();

    private:
        std::string videoFile;
        std::string vocabFile;
        std::string cfgFile;

    };

}
