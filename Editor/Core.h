#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <openvslam/system.h>
#include <openvslam/config.h>
#include <openvslam/publish/frame_publisher.h>
#include <openvslam/publish/map_publisher.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <memory>




namespace Orb {
    class Core {

        public:
            Core();

            ~Core();

            static void OnResize(GLFWwindow *window, int width, int height);

            void Run();

            void ShowDockSpace(bool* p_open);

    };

}
