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
find_package(glm CONFIG REQUIRED)

find_package(tinyobjloader CONFIG REQUIRED)


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
target_link_libraries(ImGui PRIVATE glfw GLEW::GLEW)


Set(ENGINE_SRC
        Engine/Texture.h
        Engine/Texture.cpp
        Engine/Shader.h
        Engine/Shader.cpp
        Engine/Mesh.h
        Engine/Mesh.cpp
        Engine/MeshExporter.h
        Engine/MeshExporter.cpp
        Engine/SceneRenderer.h
        Engine/SceneRenderer.cpp
        Engine/Camera.h
        Engine/Camera.cpp
)

Set(SHADERS
        Shaders/MapViewer.vs
        Shaders/MapViewer.fs
)

add_library(Engine ${ENGINE_SRC} ${SHADERS})
target_include_directories(Engine PUBLIC Engine/)
target_link_libraries(Engine PRIVATE ${OpenCV_LIBS} GLEW::GLEW glm tinyobjloader::tinyobjloader)


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
        Editor/SlamTask.h
        Editor/SlamTask.cpp
)

add_executable(Orbital ${SRC})
target_link_libraries(Orbital PRIVATE Engine ImGui ${OpenCV_LIBS} openvslam::openvslam unofficial::nativefiledialog::nfd)

# Copy shaders
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
