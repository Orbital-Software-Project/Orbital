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

#include <memory>

#include "VideoPreview.h"
#include "MapViewer.h"

#include "Global.h"
#include "Utils.hpp"
#include "SlamTask.h"

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

    //float highDPIscaleFactor = 1.0;
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

    // Default example paths
    std::string videoFile = "/home/celvin/Documents/Projects/Orbital/Content/aist_living_lab_1/video.mp4";
    std::string vocabFile = "/home/celvin/Documents/Projects/Orbital/Content/orb_vocab/orb_vocab.dbow2";
    std::string cfgFile   = "/home/celvin/Documents/Projects/Orbital/Content/aist_living_lab_1/config.yaml";

    VideoPreview preview;
    MapViewer mapViewer;
    SlamTask slamTask(videoFile, cfgFile, vocabFile);

    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        preview.OnRender();
        mapViewer.OnRender();

        {
            ImGui::Begin("Panel");

            // Filepicker
            if(ImGui::Button("Video")) {
                videoFile = Utils::PickFile();
            }

            ImGui::Text("%s", videoFile.c_str());


            if(ImGui::Button("Vocab")) {
                vocabFile = Utils::PickFile();
            }

            ImGui::Text("%s", vocabFile.c_str());

            if(ImGui::Button("Cfg")) {
                cfgFile = Utils::PickFile();
            }

            ImGui::Text("%s", cfgFile.c_str());

            // SLAM can only be started when files are present
            if(videoFile != "" && cfgFile != "" && vocabFile != "") {
                if(ImGui::Button("Run SLAM")) {
                    slamTask.Run();

                    Global::FramePublisher = slamTask.GetFramePublisher();
                    Global::MapPublisher = slamTask.GetMapPublisher();




                }

                TaskReport report = slamTask.GetReport();
                // Show progressbar and cancel button only when slam is running
                if(report.Status == TaskStatus::Running) {
                    ImGui::ProgressBar(static_cast<float>(report.FramesProcessed) / static_cast<float>(report.NumFrames));
                    if(ImGui::Button("Cancel")) {
                        slamTask.Cancel();
                    }
                }
            }

            ImGui::End();
        }

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

}


