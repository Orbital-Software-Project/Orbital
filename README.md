# Orbital





 TODO to first prototype:
 - [done] individual model transformation (translation, rotation, scale) 
 - [done] Video background in mapviewer 
 - [done] View virtual camera
 - Improve performance
 - Finish node system
 - Video playback
 - Track untracked frames
 - Render to video
 - Add Tasksystem (taskflow, imgui code style)

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
 -! optional docking -> imgui docking branch [done]
 -! Windows support [done]


 Design:
 (UI)MapViewer -> SceneRenderer -> Mesh
                               ^-> Shader
                               ^-> Camera

 SceneRenderer
 This classes contains all the entities
 Active camera is selectet to render from the cameras view
 Only one shader can be the active shader




