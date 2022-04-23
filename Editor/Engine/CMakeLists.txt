cmake_minimum_required(VERSION 3.6)

project(Engine)


if(MSVC)
    add_compile_options($<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT>)
endif()

find_package(OpenCV REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)


add_library(ImGui STATIC 
    #${IMGUI_SRC} 
    #${IMGUI_NODE_ED_SRC} 
    #${IMGUI_SEQUENCER_GUIZMO_SRC}
)

target_include_directories(ImGui PUBLIC
    #${CMAKE_SOURCE_DIR}/ThirdParty/${IMGUI_VERSION}
    #${CMAKE_SOURCE_DIR}/ThirdParty/imgui-node-editor
    #${CMAKE_SOURCE_DIR}/ThirdParty/ImGuizmo
)

target_link_libraries(ImGui PRIVATE glfw GLEW::GLEW)

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
    FrameTransformation.hpp

)

Set(SHADERS_SRC
    ${CMAKE_SOURCE_DIR}/Shaders/MapViewer.vs
    ${CMAKE_SOURCE_DIR}/Shaders/MapViewer.fs
    ${CMAKE_SOURCE_DIR}/Shaders/Mesh.vs
    ${CMAKE_SOURCE_DIR}/Shaders/Mesh.fs
)


add_library(Engine STATIC ${ENGINE_SRC} ${SHADERS_SRC})
target_include_directories(Engine PUBLIC .)
target_link_libraries(Engine PRIVATE ${OpenCV_LIBS} glfw GLEW::GLEW glm assimp::assimp)

if(MSVC)
    target_compile_options(Engine PRIVATE "$<$<CONFIG:Debug>:/MTd>;$<$<CONFIG:Release>:/MT>")
endif()