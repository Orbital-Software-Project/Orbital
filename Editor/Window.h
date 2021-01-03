#pragma once

#include "Editor/Views/IView.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

namespace Orb {

    struct WindowData {
        GLFWwindow* Window;
        std::vector<std::unique_ptr<IView>> Editors;
    };

    class Window {
    public:
        Window();

        ~Window();

        void EnterMsgLoop();

        void AddView(std::unique_ptr<IView> editorView);

        static void OnResize(GLFWwindow* window, int width, int height);

        void ShowDockSpace(bool* p_open);

    private:
        std::vector<WindowData> childWindows;

    };

}
