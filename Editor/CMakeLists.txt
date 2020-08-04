cmake_minimum_required(VERSION 2.8.9)

project(Editor)

Set(SRC
        # Core
	Main.cpp
        Core.h
        Core.cpp
        Global.h
        Utils.hpp

        # Tasks
        SlamTask.h
        SlamTask.cpp

        # Editors
        VideoPreview.h
        VideoPreview.cpp
        Outliner.h
        Outliner.cpp
        MapViewer.h
        MapViewer.cpp
)

add_executable(Editor ${SRC})
target_link_libraries(Editor PRIVATE Engine ImGui ${OpenCV_LIBS} openvslam unofficial::nativefiledialog::nfd)

