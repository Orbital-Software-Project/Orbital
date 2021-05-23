#pragma once

#include "Editor/Views/IView.hpp"
#include "Engine/Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>
#include <string>

namespace Orb {

    class Window {
    public:
        struct WindowData {
            GLFWwindow* Window;
            std::vector<std::shared_ptr<IView>> Editors;
        };


    public:
        Window();

        ~Window();

        bool DoEvents();

        void AddView(std::shared_ptr<IView> editorView);

        void Render();

        static void OnResize(GLFWwindow* window, int width, int height);

        std::vector<std::shared_ptr<IView>> GetViews();
        
        void RemoveView(std::shared_ptr<IView> view);

        void MoveByDelta(int dx, int dy);

        void Close();

        void Minimize();

        void Maximize();

    private:
        std::vector<WindowData> childWindows;

    };

}
