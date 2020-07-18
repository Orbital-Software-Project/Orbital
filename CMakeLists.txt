cmake_minimum_required(VERSION 2.8.9)

# path to dependencies
set(CMAKE_TOOLCHAIN_FILE ./ThirdParty/vcpkg-2020.06/scripts/buildsystems/vcpkg.cmake)
set(openvslam_DIR ./ThirdParty/openvslam/lib/cmake/openvslam)
set(g2o_DIR ./ThirdParty/g2o/lib/cmake/g2o)
set(OpenCV_DIR ./ThirdParty/opencv/share/OpenCV)
project(Orbital)

# Required for openvslam
find_package(suitesparse CONFIG REQUIRED)
find_package(openvslam REQUIRED)

# Window and OpenGL
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

find_package(OpenCV REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)


Set(IMGUI_VERSION "imgui-1.77")
Set(IMGUI_SRC

    ThirdParty/${IMGUI_VERSION}/imgui.h
    ThirdParty/${IMGUI_VERSION}/imgui.cpp
    ThirdParty/${IMGUI_VERSION}/imgui_internal.h
    ThirdParty/${IMGUI_VERSION}/imconfig.h

    ThirdParty/${IMGUI_VERSION}/imgui_demo.cpp
    ThirdParty/${IMGUI_VERSION}/imgui_draw.cpp
    ThirdParty/${IMGUI_VERSION}/imgui_widgets.cpp

    ThirdParty/${IMGUI_VERSION}/imstb_rectpack.h
    ThirdParty/${IMGUI_VERSION}/imstb_textedit.h
    ThirdParty/${IMGUI_VERSION}/imstb_truetype.h

    ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_glfw.h
    ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_glfw.cpp

    ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_opengl3.h
    ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_opengl3.cpp

)

add_library(ImGui ${IMGUI_SRC})
target_include_directories(ImGui PUBLIC ThirdParty/${IMGUI_VERSION})
target_link_libraries(ImGui PRIVATE glfw)


Set(SRC
	Editor/Main.cpp
        Editor/Core.h
        Editor/Core.cpp
        Editor/VideoPreview.h
        Editor/VideoPreview.cpp
        Editor/MapViewer.h
        Editor/MapViewer.cpp
        Editor/Global.h
        Editor/Utils.hpp
        Editor/Texture.hpp
        Editor/Shader.h
        Editor/Shader.cpp
        Editor/Mesh.h
        Editor/Mesh.cpp

)
Set(SHADERS
        Shaders/MapViewer.vs
        Shaders/MapViewer.fs
)

add_executable(Orbital ${SRC} ${SHADERS})

target_link_libraries(Orbital PRIVATE openvslam::openvslam ImGui GLEW::GLEW ${OpenCV_LIBS} unofficial::nativefiledialog::nfd)

# Copy shaders
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
