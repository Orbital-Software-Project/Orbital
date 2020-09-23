cmake_minimum_required(VERSION 2.8.9)

project(Engine)

Set(IMGUI_VERSION "imgui")
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

Set(IMGUI_NODE_ED_SRC 

    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imgui_widgets.cpp
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imstb_rectpack.h
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imstb_textedit.h
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imstb_truetype.h
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imconfig.h
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imgui.cpp
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imgui.h
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imgui_demo.cpp
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imgui_draw.cpp
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/external/imgui/imgui_internal.h
    
    #${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_glfw.h
    #${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_glfw.cpp
    #
    #${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_opengl3.h
    #${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/examples/imgui_impl_opengl3.cpp

    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_bezier_math.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_bezier_math.inl
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_canvas.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_canvas.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_extra_math.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_extra_math.inl
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_node_editor.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_node_editor.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_node_editor_api.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_node_editor_internal.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/imgui_node_editor_internal.inl
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/crude_json.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor/crude_json.h

)

if(MSVC)
    add_compile_options($<$<CONFIG:Debug>:/MDd> $<$<CONFIG:Release>:/MD>)
endif()


add_library(ImGui STATIC ${IMGUI_SRC} ${IMGUI_NODE_ED_SRC})
target_include_directories(ImGui PUBLIC
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor
)

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

