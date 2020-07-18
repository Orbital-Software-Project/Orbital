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

            void ExecuteSLAM(const std::shared_ptr<openvslam::config> cfg,
                             const std::string vocab_file_path,
                             const std::string video_file_path);

            std::string PickFile();

            void UpdateVideoFrame(cv::Mat& image);

    private:
            std::thread slamTask;
            GLuint currVideoFrameTexID = 0;
            bool cancelRequested = false;



    };

}
