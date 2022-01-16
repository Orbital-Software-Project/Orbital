#include "Window.h"

#include <string>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


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

            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

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

            }

        }

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

        bool shouldClose = glfwWindowShouldClose(mainWnd.Window);

            glfwPollEvents();

            glClearColor(
                0.0f,
                0.0f,
                0.0f,
                1.00f
            );

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            return shouldClose;
    }

    void Window::Close() {
        glfwMakeContextCurrent(this->childWindows[0].Window);
        glfwSetWindowShouldClose(this->childWindows[0].Window, GL_TRUE);
    }

    void Window::Minimize() {
        glfwMakeContextCurrent(this->childWindows[0].Window);
        glfwIconifyWindow(this->childWindows[0].Window);
    }

    void Window::Maximize() {
        glfwMakeContextCurrent(this->childWindows[0].Window);

        int maximized = glfwGetWindowAttrib(this->childWindows[0].Window, GLFW_MAXIMIZED);
        if (maximized == GL_TRUE) {
            glfwRestoreWindow(this->childWindows[0].Window);
        } else {
           glfwMaximizeWindow(this->childWindows[0].Window);
        }
    }

    void Window::OnResize(GLFWwindow* window, int width, int height) {
        glfwMakeContextCurrent(window);
        glViewport(0, 0, width, height);
    }

    std::vector<std::shared_ptr<IView>> Window::GetViews() {
        return this->childWindows[0].Editors;
    }

    void Window::Render() {
        for (WindowData& cWnd : this->childWindows) {
            glfwSwapBuffers(cWnd.Window);
        }
    }

    void Window::RemoveView(std::shared_ptr<IView> view) {
        auto begin = this->childWindows[0].Editors.begin();
        auto end = this->childWindows[0].Editors.end();

        auto res = std::find(begin, end, view);
        if (res != end) {
            this->childWindows[0].Editors.erase(res);
        }
    }

    void Window::MoveWindow(int dx, int dy) {


        int maximized = glfwGetWindowAttrib(this->childWindows[0].Window, GLFW_MAXIMIZED);
        if (maximized == GL_TRUE) {
            glfwRestoreWindow(this->childWindows[0].Window);
        }


        int xpos = 0;
        int ypos = 0;
        glfwGetWindowPos(this->childWindows[0].Window, &xpos, &ypos);

        double cursorPosX = 0;
        double cursorPosY = 0;
        glfwGetCursorPos(this->childWindows[0].Window, &cursorPosX, &cursorPosY);

        int mouseWndDeltaX = dx - xpos;
        int mouseWndDeltaY = dy - ypos;
        glfwSetWindowPos(
            this->childWindows[0].Window, 
            static_cast<int>(cursorPosX - mouseWndDeltaX), 
            static_cast<int>(cursorPosY - mouseWndDeltaY));
    }

}


