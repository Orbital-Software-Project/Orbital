cmake_minimum_required(VERSION 3.6)

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
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imgui_tables.cpp

    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imstb_rectpack.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imstb_textedit.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/imstb_truetype.h

    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/backends/imgui_impl_glfw.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/backends/imgui_impl_glfw.cpp

    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/backends/imgui_impl_opengl3.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}/backends/imgui_impl_opengl3.cpp

)

Set(IMGUI_NODE_ED_SRC 

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

Set(IMGUI_SEQUENCER_GUIZMO_SRC
    ${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo/ImCurveEdit.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo/ImCurveEdit.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo/ImGradient.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo/ImGradient.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo/ImGuizmo.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo/ImGuizmo.h
    ${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo/ImSequencer.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo/ImSequencer.h
)

if(MSVC)
    add_compile_options($<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT>)
endif()

find_package(OpenCV REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)


add_library(ImGui STATIC 
    ${IMGUI_SRC} 
    #${IMGUI_NODE_ED_SRC} 
    ${IMGUI_SEQUENCER_GUIZMO_SRC}
)

target_include_directories(ImGui PUBLIC
    ${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor
    ${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo
)

target_link_libraries(ImGui PRIVATE glfw GLEW::GLEW)

if(MSVC)
    target_compile_options(ImGui PRIVATE "$<$<CONFIG:Debug>:/MTd>;$<$<CONFIG:Release>:/MT>")
endif()

Set(ENGINE_SRC

    Shader.h
    Shader.cpp
    Texture.h
    Texture.cpp
    Video.h
    Video.cpp
    Vertex.hpp

    MeshImporter.h
    MeshImporter.cpp
    SceneRenderer.h
    SceneRenderer.cpp
	PrimitiveFactory.h
    PrimitiveFactory.cpp
    CameraController.h
    CameraController.cpp
    
    # Entities
    IEntity.h
    Mesh.h
    Mesh.cpp
    Camera.h
    Camera.cpp
    EntityGroup.hpp

)

Set(SHADERS_SRC
    ${CMAKE_SOURCE_DIR}/Shaders/MapViewer.vs
    ${CMAKE_SOURCE_DIR}/Shaders/MapViewer.fs
    ${CMAKE_SOURCE_DIR}/Shaders/Mesh.vs
    ${CMAKE_SOURCE_DIR}/Shaders/Mesh.fs
)



#add_subdirectory(Examples)




add_library(Engine STATIC ${ENGINE_SRC} ${SHADERS_SRC})
target_include_directories(Engine PUBLIC .)
target_link_libraries(Engine PRIVATE ${OpenCV_LIBS} glfw GLEW::GLEW glm assimp::assimp)

if(MSVC)
    target_compile_options(Engine PRIVATE "$<$<CONFIG:Debug>:/MTd>;$<$<CONFIG:Release>:/MT>")
endif()