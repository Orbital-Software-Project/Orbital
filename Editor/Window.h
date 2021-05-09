#pragma once

#include "Editor/Views/IView.hpp"
#include "Editor/Views/LayoutManager.h"
#include "Engine/Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>
#include <string>

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

        void loadImGuiTheme();

    private:
        std::vector<WindowData> childWindows;

        std::shared_ptr<Shader> shader;

        std::unique_ptr<LayoutManager> lmgr;

    };

}
