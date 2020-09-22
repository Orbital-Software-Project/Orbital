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
	PropertyEditor.h
        PropertyEditor.cpp
        MapViewer.h
        MapViewer.cpp
)

if(MSVC)
    add_compile_options($<$<CONFIG:Debug>:/MDd> $<$<CONFIG:Release>:/MD>)
endif()

add_executable(Editor ${SRC})

target_link_libraries(Editor PUBLIC 
    Engine
    ImGui
    ${OpenCV_LIBS}
    openvslam
    unofficial::nativefiledialog::nfd
)

