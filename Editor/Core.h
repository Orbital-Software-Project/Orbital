#pragma once

#include <openvslam/system.h>
#include "openvslam/config.h"

struct GLFWwindow;

namespace Orb {
    class Core {

        public:
            Core();
            ~Core();

            static void OnResize(GLFWwindow *window, int width, int height);

            void Run();

            void ExecuteSLAM(const std::shared_ptr<openvslam::config>& cfg,
                             const std::string& vocab_file_path,
                             const std::string& video_file_path);

            std::string PickFile();

    };

}
