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
#include "Outliner.h"
#include "PropertyEditor.h"

#include "imgui_node_editor.h"

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
        //highDPIscaleFactor = xscale;
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
    std::string videoFile = "/home/celvin/Documents/Projects/Orbital/Content/bata-park/video.mp4";
    std::string vocabFile = "/home/celvin/Documents/Projects/Orbital/Content/orb_vocab/orb_vocab.dbow2";
    std::string cfgFile   = "/home/celvin/Documents/Projects/Orbital/Content/bata-park/config.yaml";

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("Shaders/Mesh.vs", "Shaders/Mesh.fs");
    std::shared_ptr<SceneRenderer> renderer = std::make_shared<SceneRenderer>();

    VideoPreview preview;
    Outliner outliner(renderer);
    MapViewer mapViewer(renderer, shader);

    PropertyEditor propertyEd(renderer);


    std::unique_ptr<SlamTask> slamTask = std::make_unique<SlamTask>();

    namespace ed = ax::NodeEditor;
    ax::NodeEditor::EditorContext* g_Context = ax::NodeEditor::CreateEditor();
    

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
        outliner.OnRender();
        mapViewer.OnRender();
        propertyEd.OnRender();

        // Main menubar
        {
            ImGui::BeginMainMenuBar();
            {
                if(ImGui::BeginMenu("File")) {
                    if(ImGui::MenuItem("Import")) {
                        std::string file = Utils::PickFile();
                        if(file != "") {
                            mapViewer.ImportMesh(file);
                        }
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();
        }


        // Control Panel
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
                    slamTask = std::make_unique<SlamTask>(videoFile, cfgFile, vocabFile);
                    slamTask->Run();

                    Global::GetInstance().FramePublisher = slamTask->GetFramePublisher();
                    Global::GetInstance().MapPublisher = slamTask->GetMapPublisher();

                }

                TaskReport report = slamTask->GetReport();
                // Show progressbar and cancel button only when slam is running
                if(report.Status == TaskStatus::Running) {
                    ImGui::ProgressBar(static_cast<float>(report.FramesProcessed) / static_cast<float>(report.NumFrames));
                    if(ImGui::Button("Cancel")) {
                        slamTask->Cancel();
                    }
                }
            }

            ImGui::End();
        }

        // Node ed test
        {
            ed::SetCurrentEditor(g_Context);

            ed::Begin("My Editor");

            int uniqueId = 1;

            // Start drawing nodes.
            ed::BeginNode(uniqueId++);
            ImGui::Text("Node A");
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
            ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();
            ed::EndNode();

            ed::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ax::NodeEditor::DestroyEditor(g_Context);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

}


