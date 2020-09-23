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
#include "NodeEditor.h"

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

    {
        // Init ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }
    


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
    NodeEditor nodeEd(renderer);


    std::unique_ptr<SlamTask> slamTask = std::make_unique<SlamTask>();

    
    
    

    while (!glfwWindowShouldClose(window))
    {


        glfwPollEvents();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool showDockSpace = true;
        this->ShowDockSpace(&showDockSpace);


        preview.OnRender();
        outliner.OnRender();
        mapViewer.OnRender();
        propertyEd.OnRender();
        nodeEd.OnRender();

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


void Core::ShowDockSpace(bool* p_open)
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }
        
    

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace", p_open, window_flags);

    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        //ImGui::ShowDockingDisabledMessage();
    }


    ImGui::End();
}

}


