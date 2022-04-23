cmake_minimum_required(VERSION 3.6)

project(Orbital.Editor)

Set(SRC
    # Core
    Main.cpp
    Core.h
    Window.h
    Window.cpp
    Core.cpp
    Utils.hpp
    Global.h


    # Base
    Base/Percentage.hpp
    Base/ScopeMutexLock.hpp

    # Tasks
    Tasks/TaskWorker.h
    Tasks/TaskWorker.cpp
    Tasks/OpenVslamTask.h
    Tasks/OpenVslamTask.cpp
    #Tasks/Viso2Task.h
    #Tasks/Viso2Task.cpp
    Tasks/MeshImportTask.h
    Tasks/MeshImportTask.cpp

    # Views
    Views/Toolbar.h
    Views/Toolbar.cpp
    Views/VideoPreview.h
    Views/VideoPreview.cpp
    Views/Outliner.h
    Views/Outliner.cpp
    Views/PropertyEditor.h
    Views/PropertyEditor.cpp
    #Views/NodeEditor.h
    #Views/NodeEditor.cpp
    Views/MapViewer.h
    Views/MapViewer.cpp
    Views/TaskPanel.h
    Views/TaskPanel.cpp
    Views/Sequencer.h
    Views/Sequencer.cpp
    Views/ViewManager.h
    Views/ViewManager.cpp
    Views/ImSplitContainer.h

    # Nodes
    #Nodes/FileNode.h
    #Nodes/FileNode.cpp
    #Nodes/SlamNode.h
    #Nodes/SlamNode.cpp
    #Nodes/VideoPrevNode.h
    #Nodes/VideoPrevNode.cpp
    #Nodes/MapViewNode.h
    #Nodes/MapViewNode.cpp

    # Engine
    Engine/Shader.h
    Engine/Shader.cpp
    Engine/Texture.h
    Engine/Texture.cpp
    Engine/Video.h
    Engine/Video.cpp
    Engine/Vertex.hpp

    Engine/MeshImporter.h
    Engine/MeshImporter.cpp
    Engine/SceneRenderer.h
    Engine/SceneRenderer.cpp
    Engine/PrimitiveFactory.h
    Engine/PrimitiveFactory.cpp
    Engine/CameraController.h
    Engine/CameraController.cpp
    
    # Entities
    Engine/IEntity.h
    Engine/Mesh.h
    Engine/Mesh.cpp
    Engine/Camera.h
    Engine/Camera.cpp
    Engine/EntityGroup.hpp
    Engine/FrameTransformation.hpp
)


add_executable(Orbital.Editor ${SRC})

target_link_libraries(Orbital.Editor PRIVATE 
    ${OpenCV_LIBS}
    unofficial::nativefiledialog::nfd
    stella_vslam
    GLEW::GLEW
    imgui::imgui
    assimp::assimp
    Orbital.ImGuizmo
)


if(MSVC)
    target_compile_options(Orbital.Editor PRIVATE "$<$<CONFIG:Debug>:/MTd>;$<$<CONFIG:Release>:/MT>")
    target_link_options(Orbital.Editor PUBLIC /NODEFAULTLIB:library)
    
    # Set Editor as Visual Studio startup project
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Orbital.Editor)

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
endif()
