# Orbital





 TODO to first prototype:
 - individual model transformation (translation, rotation, scale) [partially done]
 - Video background in mapviewer
 - View virtual camera


Nice to have:
 - Improve viewport navigation -> imgui guizmos
 - Filter view (show grid, landmarks, keyframes etc.)
 - Draw keyframes [done] - Points need to be visible
 - Export via assimp (assimp is broken with vcpkg)
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




