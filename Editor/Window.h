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
        std::vector<std::shared_ptr<IView>> Editors;
    };

    class Window {
    public:
        Window();

        ~Window();

        bool DoEvents();

        void AddView(std::shared_ptr<IView> editorView);

        void Render();

        static void OnResize(GLFWwindow* window, int width, int height);

        void ShowDockSpace(bool* p_open);

        std::vector<std::shared_ptr<IView>> GetViews();
        
        void RemoveView(std::shared_ptr<IView> view) {
            auto begin = this->childWindows[0].Editors.begin();
            auto end = this->childWindows[0].Editors.end();
            
            auto res = std::find(begin, end, view);
            if (res != end) {
                this->childWindows[0].Editors.erase(res);
            }

        }

    private:
        void loadImGuiTheme();

        std::vector<WindowData> childWindows;

        //std::shared_ptr<Shader> shader;

        //std::unique_ptr<LayoutManager> lmgr;

    };

}
