#include "Core.h"

#include "Editor/Global.h"

#include "Editor/Views/VideoPreview.h"
#include "Editor/Views/MapViewer.h"
#include "Editor/Views/Outliner.h"
#include "Editor/Views/PropertyEditor.h"
#include "Editor/Views/TaskPanel.h"
#include "Editor/Views/Toolbar.h"

#include "Editor/Window.h"

namespace Orb {

Core::Core() {}

Core::~Core() {}

void Core::Run() {

    Window wnd;

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("Shaders/Mesh.vs", "Shaders/Mesh.fs");

    wnd.AddView(std::make_unique<Toolbar>());
    wnd.AddView(std::make_unique<VideoPreview>());
    wnd.AddView(std::make_unique<Outliner>(Global::GetInstance().Renderer));
    wnd.AddView(std::make_unique<MapViewer>(Global::GetInstance().Renderer, shader));
    wnd.AddView(std::make_unique<PropertyEditor>(Global::GetInstance().Renderer));
    wnd.AddView(std::make_unique<TaskPanel>());

    wnd.EnterMsgLoop();

}




}


