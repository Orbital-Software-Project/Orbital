#pragma once

#include "Editor/Views/IView.hpp"

#include <string>
#include <memory>

namespace Orb {

    class TaskPanel : public IView {

    public:
        TaskPanel();

        ~TaskPanel();

        void OnRender();

    private:
        std::string videoFile;
        std::string vocabFile;
        std::string cfgFile;

    };

}
