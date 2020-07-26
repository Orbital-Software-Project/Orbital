# Orbital





 TODO:
 - Draw camera mesh [done]
 - Draw camera trajectory [done]
 - Improve viewport navigation -> imgui guizmos
 - Draw local landmarks -> needs vertex colors [done]
 - Filter view (show grid, landmarks, keyframes etc.)
 - Draw keyframes [done]
 - Export via assimp (assimp is broken with vcpkg)
 - Mesh importer (obj, gltf)
 - Fix crash when cancel and make restart of slam possible [done]


 - Import 3d model (transformation abilities)


 - Add pause button
 - Add project system
 - Add ImGui Sequencer
 - Add ImGui nodes
 - Add Masking system
 -! optional docking -> imgui docking branch
 -! Windows support


 Design:
 (UI)MapViewer -> SceneRenderer -> Mesh
                               ^-> Shader
                               ^-> Camera
 SceneRenderer
 This classes contains all the entities
 Active camera is selectet to render from the cameras view
 Only one shader can be the active shader


