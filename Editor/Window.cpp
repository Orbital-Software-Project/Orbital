#include "Window.h"

#include <string>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


#include "Editor/Global.h"

namespace Orb {

    Window::Window() {
        WindowData mainWindow;


        // GLFW/ImGui init
        {

            if (!glfwInit()) {
                return;
            }

            glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
            glfwWindowHint(GLFW_DEPTH_BITS,  24);
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

                //ImGuiIO& io = ImGui::GetIO();
                //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            }

        }

        this->loadImGuiTheme();

        this->childWindows.push_back(std::move(mainWindow));

        



    }

    Window::~Window() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext();

        glfwTerminate();
    }

    void Window::AddView(std::shared_ptr<IView> editorView) {
        this->childWindows[0].Editors.push_back(editorView);
    }

    bool Window::DoEvents() {

        // Mainwindow is the first entry of the vector
        auto& mainWnd = this->childWindows[0];

        //this->lmgr = std::make_unique<LayoutManager>(this->shader);

        bool shouldClose = glfwWindowShouldClose(mainWnd.Window);

        //while (!glfwWindowShouldClose(mainWnd.Window))
        //{

            glfwPollEvents();

            glClearColor(
                0.0f,
                0.0f,
                0.0f,
                1.00f
            );

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            

            // Fullscreen view background
            {
                //static bool use_work_area = true;
                //static ImGuiWindowFlags flags = 
                //    ImGuiWindowFlags_NoDecoration | 
                //    ImGuiWindowFlags_NoMove | 
                //    ImGuiWindowFlags_NoResize | 
                //    ImGuiWindowFlags_NoSavedSettings | 
                //    ImGuiWindowFlags_NoBringToFrontOnFocus;

                //// We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
                //// Based on your use case you may want one of the other.
                //const ImGuiViewport* viewport = ImGui::GetMainViewport();
                //ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
                //ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

                //bool open = true;
                //if (ImGui::Begin("Background", &open, flags))
                //{


                //    ImGui::End();
                //}
            }

            {
                //for (int h = 0; h < this->childWindows.size(); h++) {
                //    int viewToRemoveIdx = -1;

                //    auto& cWnd = this->childWindows[h];
                //    for (int i = 0; i < cWnd.Editors.size(); i++) {
                //        
                //        // TODO: Multiple instances in separate windows
                //        auto& editor = cWnd.Editors[i];
                //        editor->OnRender();

                //        //this->lmgr->OnRender();




                //        
                //    }

                //    
                //    
                //}

                
            }

            
        //}

            return shouldClose;
    }

    void Window::OnResize(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::ShowDockSpace(bool* p_open)
    {
        //static bool opt_fullscreen = true;
        //static bool opt_padding = false;
        //static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        //// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        //// because it would be confusing to have two docking targets within each others.
        //ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        //if (opt_fullscreen)
        //{
        //    ImGuiViewport* viewport = ImGui::GetMainViewport();
        //    //ImGui::SetNextWindowPos(viewport->GetWorkPos());
        //    //ImGui::SetNextWindowSize(viewport->GetWorkSize());
        //    //ImGui::SetNextWindowViewport(viewport->ID);
        //    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        //    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        //    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        //    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        //}
        //else
        //{
        //    dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        //}

        //// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        //// and handle the pass-thru hole, so we ask Begin() to not render a background.
        //if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        //    window_flags |= ImGuiWindowFlags_NoBackground;
        //}



        //// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        //// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        //// all active windows docked into it will lose their parent and become undocked.
        //// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        //// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        //if (!opt_padding)
        //    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        //ImGui::Begin("DockSpace", p_open, window_flags);

        //if (!opt_padding)
        //    ImGui::PopStyleVar();

        //if (opt_fullscreen)
        //    ImGui::PopStyleVar(2);

        //// DockSpace
        //ImGuiIO& io = ImGui::GetIO();
        //if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        //{
        //    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        //    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        //}
        //else
        //{
        //    //ImGui::ShowDockingDisabledMessage();
        //}


        //ImGui::End();
    }

    std::vector<std::shared_ptr<IView>> Window::GetViews() {
        return this->childWindows[0].Editors;
    }

    void Window::Render() {
        for (WindowData& cWnd : this->childWindows) {
            glfwSwapBuffers(cWnd.Window);
        }
    }

    void Window::loadImGuiTheme() {

        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.38f, 0.38f, 0.38f, 0.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);



    }
}


