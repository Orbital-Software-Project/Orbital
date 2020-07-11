#include "Core.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/core.hpp>

#include <opencv2/videoio.hpp>

#include <memory>

namespace Orb {

Core::Core() {}

Core::~Core() {}

void Core::OnResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Core::Run() {
    GLFWwindow* window = nullptr;

    if (! glfwInit())
        return;

    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    std::string glsl_version = "";
#ifdef __APPLE__
    // GL 3.2 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint( // required on Mac OS
                    GLFW_OPENGL_FORWARD_COMPAT,
                    GL_TRUE
                    );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif __linux__
    // GL 3.2 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#elif _WIN32
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif

    float highDPIscaleFactor = 1.0;
#ifdef _WIN32
    // if it's a HighDPI monitor, try to scale everything
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    if (xscale > 1 || yscale > 1)
    {
        highDPIscaleFactor = xscale;
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
#elif __APPLE__
    // to prevent 1200x800 from becoming 2400x1600
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif



    window = glfwCreateWindow(1280, 720, "Orbital", nullptr, nullptr);
    if (! window)
    {
        glfwTerminate();
        return;
    }

    glfwSetFramebufferSizeCallback(window, Core::OnResize);
    glfwMakeContextCurrent(window);


    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return;
    }



    int actualWindowWidth, actualWindowHeight;
    glfwGetWindowSize(window, &actualWindowWidth, &actualWindowHeight);
    glViewport(0, 0, actualWindowWidth, actualWindowHeight);



    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    std::string videoFile = "";
    std::string vocabFile = "";
    std::string cfgFile = "";

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("Demo window");
        ImGui::Text("Select a file");



        // Filepicker
        if(ImGui::Button("Video")) {
            videoFile = this->PickFile();

        }

        ImGui::Text(videoFile.c_str());


        if(ImGui::Button("Vocab")) {
            vocabFile = this->PickFile();

        }

        ImGui::Text(vocabFile.c_str());

        if(ImGui::Button("Cfg")) {
            cfgFile = this->PickFile();

        }

        ImGui::Text(cfgFile.c_str());


        if(videoFile != "" && cfgFile != "" && vocabFile != "") {
            if(ImGui::Button("Run SLAM")) {
                this->ExecuteSLAM(std::make_shared<openvslam::config>(cfgFile),
                                  vocabFile,
                                  videoFile
                                  );
            }
        }

        if(this->numFrames > 0) {
            ImGui::ProgressBar(static_cast<float>(this->framesProcessed) / static_cast<float>(this->numFrames));
        }

        ImGui::End();






        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Core::ExecuteSLAM(const std::shared_ptr<openvslam::config> cfg,
                       const std::string vocab_file_path,
                       const std::string video_file_path) {



    // run the SLAM in another thread
    this->slamTask = std::thread([cfg, vocab_file_path, video_file_path, this]() {



        // build a SLAM system
        openvslam::system SLAM(cfg, vocab_file_path);

        // startup the SLAM process
        SLAM.startup();

        cv::VideoCapture video(video_file_path);
        this->numFrames = video.get(cv::CAP_PROP_FRAME_COUNT);



        std::vector<double> track_times;

        cv::Mat frame;
        double timestamp = 0.0;

        int frame_skip = 1;

        unsigned int num_frame = 0;

        bool is_not_end = true;


        while (is_not_end) {
            is_not_end = video.read(frame);

            const auto tp_1 = std::chrono::steady_clock::now();

            if (!frame.empty() && (num_frame % frame_skip == 0)) {
                // input the current frame and estimate the camera pose
                SLAM.feed_monocular_frame(frame, timestamp, cv::Mat());
            }

            this->framesProcessed++;

            const auto tp_2 = std::chrono::steady_clock::now();

            const auto track_time = std::chrono::duration_cast<std::chrono::duration<double>>(tp_2 - tp_1).count();
            if (num_frame % frame_skip == 0) {
                track_times.push_back(track_time);
            }



            timestamp += 1.0 / cfg->camera_->fps_;
            ++num_frame;

            // check if the termination of SLAM system is requested or not
            if (SLAM.terminate_is_requested()) {
                break;
            }
        }

        // wait until the loop BA is finished
        while (SLAM.loop_BA_is_running()) {
            std::this_thread::sleep_for(std::chrono::microseconds(5000));
        }



        // shutdown the SLAM process
        SLAM.shutdown();


        if (true /*eval_log*/) {
            //output the trajectories for evaluation
            SLAM.save_frame_trajectory("frame_trajectory.txt", "TUM");
            SLAM.save_keyframe_trajectory("keyframe_trajectory.txt", "TUM");
            //output the tracking times for evaluation
            std::ofstream ofs("track_times.txt", std::ios::out);
            if (ofs.is_open()) {
                for (const auto track_time : track_times) {
                    ofs << track_time << std::endl;
                }
                ofs.close();
            }
        }

        //if (!map_db_path.empty()) {
        // output the map database
        //  SLAM.save_map_database(map_db_path);
        //}

        std::sort(track_times.begin(), track_times.end());
        const auto total_track_time = std::accumulate(track_times.begin(), track_times.end(), 0.0);
        std::cout << "median tracking time: " << track_times.at(track_times.size() / 2) << "[s]" << std::endl;
        std::cout << "mean tracking time: " << total_track_time / track_times.size() << "[s]" << std::endl;


    });

    //thread.join();








}


std::string Core::PickFile() {
    std::string file = "";
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );

    if ( result == NFD_OKAY ) {
        puts("Success!");
        file = outPath;
        puts(outPath);
        free(outPath);
    }
    else if ( result == NFD_CANCEL ) {
        puts("User pressed cancel.");
    }
    else {
        printf("Error: %s\n", NFD_GetError() );
    }
    return file;
}


}


