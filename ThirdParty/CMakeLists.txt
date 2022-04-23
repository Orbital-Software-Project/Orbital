add_subdirectory(stella_vslam)

set(IMGUIZMO_SRC
	./ImGuizmo/ImGuizmo.h
	./ImGuizmo/ImGuizmo.cpp
	./ImGuizmo/ImSequencer.h
	./ImGuizmo/ImSequencer.cpp
	./ImGuizmo/ImCurveEdit.h
	./ImGuizmo/ImCurveEdit.cpp
)

add_library(Orbital.ImGuizmo ${IMGUIZMO_SRC})

target_compile_options(Orbital.ImGuizmo PRIVATE "$<$<CONFIG:Debug>:/MTd>;$<$<CONFIG:Release>:/MT>")
target_link_libraries(Orbital.ImGuizmo imgui::imgui)
target_include_directories(Orbital.ImGuizmo PUBLIC ./ImGuizmo)