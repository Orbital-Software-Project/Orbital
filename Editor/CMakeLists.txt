cmake_minimum_required(VERSION 2.8.9)

project(Editor)

find_package(Cpp-Taskflow CONFIG REQUIRED)

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
	NodeEditor.h
    NodeEditor.cpp
    MapViewer.h
    MapViewer.cpp

	# Nodes
	Nodes/FileNode.h
	Nodes/FileNode.cpp
	Nodes/SlamNode.h
	Nodes/SlamNode.cpp
	Nodes/VideoPrevNode.h
	Nodes/VideoPrevNode.cpp
	Nodes/MapViewNode.h
	Nodes/MapViewNode.cpp

)

if(MSVC)
    add_compile_options(/bigobj /MDd /source-charset:utf-8 /execution-charset:utf-8 /wd4996 /wd4275 /wd4251 /wd4244 /wd4305 /wd4267 /wd4127)
    add_compile_options($<$<CONFIG:Debug>:/MDd> $<$<CONFIG:Release>:/MD>)

    # Set Editor as Visual Studio startup project
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Editor)

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
    
    set(DLL_FILES 
    	"/ThirdParty/g2o/bin/g2o_core_d.dll"
    	"/ThirdParty/g2o/bin/g2o_types_slam3d_d.dll"
    	"/ThirdParty/g2o/bin/g2o_csparse_extension_d.dll"
    	"/ThirdParty/g2o/bin/g2o_solver_csparse_d.dll"
    	"/ThirdParty/g2o/bin/g2o_solver_dense_d.dll"
    	"/ThirdParty/g2o/bin/g2o_solver_eigen_d.dll"
    	"/ThirdParty/g2o/bin/g2o_stuff_d.dll"
    	"/ThirdParty/g2o/bin/g2o_types_sba_d.dll"
    	"/ThirdParty/g2o/bin/g2o_types_sim3_d.dll"
    	"/ThirdParty/DBoW2/lib/dbow2.dll"
    	"/ThirdParty/vcpkg-2020.06/installed/x64-windows/bin/libcxsparse.dll"
    )

foreach(DLL_FILE ${DLL_FILES})
file(COPY ${CMAKE_SOURCE_DIR}${DLL_FILE}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
endforeach()

endif()

add_executable(Editor ${SRC})

target_link_libraries(Editor PRIVATE 
    Engine
    ImGui
    ${OpenCV_LIBS}
    openvslam
    unofficial::nativefiledialog::nfd
    Cpp-Taskflow::Cpp-Taskflow
)




