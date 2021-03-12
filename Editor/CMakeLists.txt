cmake_minimum_required(VERSION 3.6)

project(Editor)

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

    # Nodes
    #Nodes/FileNode.h
    #Nodes/FileNode.cpp
    #Nodes/SlamNode.h
    #Nodes/SlamNode.cpp
    #Nodes/VideoPrevNode.h
    #Nodes/VideoPrevNode.cpp
    #Nodes/MapViewNode.h
    #Nodes/MapViewNode.cpp

)



find_package(unofficial-nativefiledialog CONFIG REQUIRED)
find_package(SuiteSparse CONFIG REQUIRED)
#find_package(libpng CONFIG REQUIRED)

find_package(openvslam CONFIG REQUIRED)
find_package(fbow CONFIG REQUIRED)

# For CMake < 3.9, we need to make the target ourselves
if(NOT TARGET OpenMP::OpenMP_CXX)
    find_package(Threads REQUIRED)
    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
    # Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)
endif()



add_executable(Editor ${SRC})


target_link_libraries(Editor PRIVATE 
    Engine
    ImGui
    ${OpenCV_LIBS}
    Threads::Threads
    OpenMP::OpenMP_CXX
    fbow::fbow
    openvslam::openvslam 
    unofficial::nativefiledialog::nfd
    ${SUITESPARSE_LIBRARIES} 
    #libviso2
    #libmv
    #${PNG_LIBRARIES}
)



if(MSVC)
    target_compile_options(Editor PRIVATE /bigobj /source-charset:utf-8 /execution-charset:utf-8 /wd4996 /wd4275 /wd4251 /wd4244 /wd4305 /wd4267 /wd4127)
    target_compile_options(Editor PRIVATE "$<$<CONFIG:Debug>:/MTd>;$<$<CONFIG:Release>:/MT>")
   
    # Set Editor as Visual Studio startup project
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Editor)

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
endif()
