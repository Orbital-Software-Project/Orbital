#include "Editor/Core.h"
#include "Editor/Window.h"


#include "Editor/Views/Outliner.h"
#include "Editor/Views/PropertyEditor.h"
#include "Editor/Views/VideoPreview.h"
#include "Editor/Views/MapViewer.h"
#include "Editor/Views/Sequencer.h"
#include "Editor/Views/NodeEditor/NodeEditor.h"
#include "Editor/Views/TaskPanel.h"
#include "Editor/Views/Toolbar.h"

#include <memory>
#include <GL/glew.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


namespace Orb {

    Core::Core(std::string rootDir) {
        Global::GetInstance().RootDir = rootDir;
    }

    Core::~Core() {}

    void Core::Run() {

        Window wnd;
        ViewManager lmgr;

        std::string rootDir = Global::GetInstance().RootDir;
        auto shader =
            std::make_shared<Shader>(
                rootDir + "\\Shaders\\Mesh.vs",
                rootDir + "\\Shaders\\Mesh.fs");

        auto toolbar = std::make_shared<Toolbar>();
        wnd.AddView(toolbar);
        lmgr.AddView(toolbar, ViewManager::DockType::Dock_Float);

        auto outliner = std::make_shared<Outliner>(Global::GetInstance().Renderer);
        wnd.AddView(outliner);
        lmgr.AddView(outliner, ViewManager::DockType::Dock_Left);

        auto propEd = std::make_shared<PropertyEditor>(Global::GetInstance().Renderer);
        wnd.AddView(propEd);
        lmgr.AddView(propEd, ViewManager::DockType::Dock_Right);

        auto taskpnl = std::make_shared<TaskPanel>();
        wnd.AddView(taskpnl);
        lmgr.AddView(taskpnl, ViewManager::DockType::Dock_Right);

        auto videoPrev = std::make_shared<VideoPreview>();
        wnd.AddView(videoPrev);
        lmgr.AddView(videoPrev, ViewManager::DockType::Dock_Central_Top);

        auto mapViewer = std::make_shared<MapViewer>(Global::GetInstance().Renderer, shader);
        wnd.AddView(mapViewer);
        lmgr.AddView(mapViewer, ViewManager::DockType::Dock_Central_Top);

        auto seq = std::make_shared<Sequencer>();
        wnd.AddView(seq);
        lmgr.AddView(seq, ViewManager::DockType::Dock_Central_Bottom);

        auto nodeEd = std::make_shared<NodeEditor>(Global::GetInstance().Renderer);
        wnd.AddView(nodeEd);
        lmgr.AddView(nodeEd, ViewManager::DockType::Dock_Central_Bottom);


        // Default views
        while (!wnd.DoEvents()) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            lmgr.OnRender();

            // Handle IView Requests
            {
                std::shared_ptr<IView> viewToRemoveIdx = nullptr;
                for (auto view : wnd.GetViews()) {
                    // Handle requests
                    IView::ViewWindowRequest request = IView::ViewWindowRequest::None;
                    if (view->HasRequest(request)) {
                        switch (request) {
                        case IView::ViewWindowRequest::Close:
                            // Mark for remove
                            viewToRemoveIdx = view;
                            break;
                        case IView::ViewWindowRequest::Open_MapViewer:
                            wnd.AddView(std::make_shared<Orb::MapViewer>(Global::GetInstance().Renderer, shader));
                            break;
                        case IView::ViewWindowRequest::Open_Outliner:
                            wnd.AddView(std::make_shared<Orb::Outliner>(Global::GetInstance().Renderer));
                            break;
                        case IView::ViewWindowRequest::Open_PropertyEd:
                            wnd.AddView(std::make_shared<Orb::PropertyEditor>(Global::GetInstance().Renderer));
                            break;
                        case IView::ViewWindowRequest::Open_Sequencer:
                            wnd.AddView(std::make_shared<Orb::Sequencer>());
                            break;
                        case IView::ViewWindowRequest::Open_Taskpanel:
                            wnd.AddView(std::make_shared<Orb::TaskPanel>());
                            break;
                        case IView::ViewWindowRequest::Open_VideoPrev:
                            wnd.AddView(std::make_shared<Orb::VideoPreview>());
                            break;
                        case IView::ViewWindowRequest::Open_NodeEd:
                            wnd.AddView(std::make_shared<Orb::NodeEditor>(Global::GetInstance().Renderer));
                            break;
                        }
                    }
                    if (viewToRemoveIdx != nullptr) {
                        wnd.RemoveView(view);
                        viewToRemoveIdx = nullptr;
                    }
                }
            }

            //Handle ViewManager Requests
            {
                ViewManager::Request request = ViewManager::Request::None;
                if (lmgr.HasRequest(request)) {
                    switch (request) {
                    case ViewManager::Request::Move_Window:
                        wnd.MoveWindow(static_cast<int>(lmgr.Dx), static_cast<int>(lmgr.Dy));
                        break;
                    case ViewManager::Request::Close_Window:
                        wnd.Close();
                        break;
                    case ViewManager::Request::Minimize_Window:
                        wnd.Minimize();
                        break;
                    case ViewManager::Request::Maximize_Window:
                        wnd.Maximize();
                        break;
                    }
                }
            }


#ifndef NDEBUG
            ImGui::ShowDemoWindow();
#endif

            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            // Handle view requests
            // TODO: Add this to a message handler

            // TODO ex:
            // wnd.AddView(taskpanel)
            // while(wnd.DoEvents()) {
            //  for(IView r : wnd.GetViews()) {
            //   ViewRequest req;
            //   if (r.HasRequest(&req)) {
            //    //Handle all requests from views in core instead of window class
            //   }
            //  }
            // }

            wnd.Render();
        }
    }
}


