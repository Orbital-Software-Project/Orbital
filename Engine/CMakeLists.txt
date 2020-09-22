cmake_minimum_required(VERSION 2.8.9)

project(Engine)

Set(IMGUI_VERSION "imgui-1.77")
Set(IMGUI_SRC
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imgui.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imgui.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imgui_internal.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imconfig.h

    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imgui_widgets.cpp

    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imstb_rectpack.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imstb_textedit.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imstb_truetype.h

    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_glfw.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_glfw.cpp

    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_opengl3.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_opengl3.cpp
)

if(MSVC)
    add_compile_options($<$<CONFIG:Debug>:/MDd> $<$<CONFIG:Release>:/MD>)
endif()

add_library(ImGui STATIC ${IMGUI_SRC} )
target_include_directories(ImGui PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION})
target_link_libraries(ImGui PRIVATE glfw GLEW::GLEW)



find_package(assimp CONFIG REQUIRED)


Set(ENGINE_SRC
        IEntity.h
        Texture.h
        Texture.cpp
        Shader.h
        Shader.cpp
        Mesh.h
        Mesh.cpp
        MeshImporter.h
        MeshImporter.cpp
        SceneRenderer.h
        SceneRenderer.cpp
        Camera.h
        Camera.cpp
        CameraController.h
        CameraController.cpp
	PrimitiveFactory.h
        PrimitiveFactory.cpp
)

Set(SHADERS_SRC
        ${CMAKE_SOURCE_DIR}/Shaders/MapViewer.vs
        ${CMAKE_SOURCE_DIR}/Shaders/MapViewer.fs
        ${CMAKE_SOURCE_DIR}/Shaders/Mesh.vs
        ${CMAKE_SOURCE_DIR}/Shaders/Mesh.fs
)

if(MSVC)
    add_compile_options($<$<CONFIG:Debug>:/MDd> $<$<CONFIG:Release>:/MD>)
endif()

add_library(Engine STATIC ${ENGINE_SRC} ${SHADERS_SRC})
target_include_directories(Engine PUBLIC .)
target_link_libraries(Engine PRIVATE ${OpenCV_LIBS} glfw GLEW::GLEW glm assimp::assimp)

