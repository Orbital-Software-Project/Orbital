#include "Window.h"

#include <string>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "imgui_node_editor.h"

namespace Orb {

    Window::Window() {
        WindowData mainWindow;

        if (!glfwInit()) {
            return;
        }

        glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

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
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
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

        mainWindow.Window = glfwCreateWindow(1920, 1080, "Orbital", nullptr, nullptr);

        if (!mainWindow.Window)
        {
            glfwTerminate();
            return;
        }

        glfwSetFramebufferSizeCallback(mainWindow.Window, Window::OnResize);
        glfwMakeContextCurrent(mainWindow.Window);

        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
            glfwTerminate();
            return;
        }

        int actualWindowWidth, actualWindowHeight;
        glfwGetWindowSize(mainWindow.Window, &actualWindowWidth, &actualWindowHeight);
        glViewport(0, 0, actualWindowWidth, actualWindowHeight);
        
        
        // Init ImGui
        {
            IMGUI_CHECKVERSION();

            ImGui::CreateContext();

            ImGui_ImplGlfw_InitForOpenGL(mainWindow.Window, true);
            ImGui_ImplOpenGL3_Init(glsl_version.c_str());

            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        }


        this->childWindows.push_back(std::move(mainWindow));
    }

    Window::~Window() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();
    }

    void Window::AddView(std::unique_ptr<IView> editorView) {
        this->childWindows[0].Editors.push_back(std::move(editorView));
    }

    void Window::EnterMsgLoop() {

        // Mainwindow is the first entry of the vector
        auto& mainWnd = this->childWindows[0];

        while (!glfwWindowShouldClose(mainWnd.Window))
        {
            glfwPollEvents();
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            static bool showDockSpace = true;
            this->ShowDockSpace(&showDockSpace);

#ifndef NDEBUG
            ImGui::ShowDemoWindow();
#endif

            {
                for (int h = 0; h < this->childWindows.size(); h++) {
                    auto& cWnd = this->childWindows[h];
                    for (int i = 0; i < cWnd.Editors.size(); i++) {
                        auto& editor = cWnd.Editors[i];
                        editor->OnRender();
                    }
                }
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            for (WindowData& cWnd : this->childWindows) {
                glfwSwapBuffers(cWnd.Window);
            }
        }
    }

    void Window::OnResize(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::ShowDockSpace(bool* p_open)
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


