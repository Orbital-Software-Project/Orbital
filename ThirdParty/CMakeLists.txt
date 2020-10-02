cmake_minimum_required(VERSION 3.1)

set(g2o_DIR   ./g2o/lib/cmake/g2o)
set(DBoW2_DIR ./DBoW2/lib/cmake/DBoW2)

# spdlog
set(spdlog_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openvslam/3rd/spdlog/include)

# json
set(json_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openvslam/3rd/json/include)

project(openvslam LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------

# Compile options
if(MSVC)
    # C4251: DLL export, C4244: floating->integer, C4305: double->float, C4267: size_t->any, C4127: constant condition
    add_compile_options(/bigobj /MDd /source-charset:utf-8 /execution-charset:utf-8 /wd4275 /wd4251 /wd4244 /wd4305 /wd4267 /wd4127)
    add_compile_options("$<$<CONFIG:Debug>:/Zo>;$<$<CONFIG:Release>:/O2>")
else()
    add_compile_options(-Wall -Wextra)
    add_compile_options("$<$<CONFIG:Debug>:-Og>;$<$<CONFIG:Release>:-O3;-ffast-math>")
endif()

set(BUILD_WITH_MARCH_NATIVE OFF CACHE BOOL "Enable architecture-aware optimization")
if(BUILD_WITH_MARCH_NATIVE AND NOT MSVC)
    add_compile_options(-mtune=native -march=native)
    message(STATUS "Architecture-aware optimization: ENABLED")
else()
    message(STATUS "Architecture-aware optimization: DISABLED")
endif()

# ---------------------------------------------------------
# Packages

# Threads
find_package(Threads REQUIRED)

# OpenMP
#find_package(OpenMP REQUIRED)
#if(NOT TARGET OpenMP::OpenMP_CXX)
#    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
#    set_property(TARGET OpenMP::OpenMP_CXX
#                 PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)
#endif()

# Eigen
find_package(Eigen3 3.3 REQUIRED)

# yaml-cpp
find_package(yaml-cpp REQUIRED)

# OpenCV
find_package(OpenCV 3.3.1 QUIET
             COMPONENTS
             core imgcodecs videoio features2d calib3d highgui)
if(NOT OpenCV_FOUND)
    find_package(OpenCV 4.0 REQUIRED
                 COMPONENTS
                 core imgcodecs videoio features2d calib3d highgui)
    if(NOT OpenCV_FOUND)
        message(FATAL_ERROR "OpenCV >= 3.3.1 not found")
    endif()
endif()
message(STATUS "Use OpenCV ${OpenCV_VERSION}")


find_package(g2o CONFIG REQUIRED
             COMPONENTS
             g2o::core
             g2o::stuff
             g2o::types_sba
             g2o::types_sim3
             g2o::solver_dense
             g2o::solver_eigen
             g2o::solver_csparse
             g2o::csparse_extension
             OPTIONAL_COMPONENTS
             g2o::csparse)

# Check first if CSparse is built from g2o
if(TARGET g2o::csparse)
    set(${CXSPARSE_LIBRARIES} g2o::csparse)
else()
    # CXSparse for g2o
    find_package(CXSparse)
    if(CXSPARSE_FOUND)
        include_directories(${CXSPARSE_INCLUDE_DIRS})
    endif()
    # SuiteSparse for g2o
    find_package(SuiteSparse CONFIG REQUIRED)
    if(SUITESPARSE_FOUND)
        include_directories(${SUITESPARSE_INCLUDE_DIRS})
    endif()
endif()




set(USE_SSE_ORB OFF CACHE BOOL "Enable SSE3 instruction for ORB extraction")
if(USE_SSE_ORB)
    target_compile_options(openvslam PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse3>)
    target_compile_definitions(openvslam PUBLIC USE_SSE_ORB)
    message(STATUS "SSE3 for ORB extraction: ENABLED")
else()
    message(STATUS "SSE3 for ORB extraction: DISABLED")
endif()

set(USE_SSE_FP_MATH OFF CACHE BOOL "Enable SSE instruction for floating-point operation")
if(USE_SSE_FP_MATH)
    target_compile_options(openvslam PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-mfpmath=sse>)
    message(STATUS "SSE for floating-point operation: ENABLED")
else()
    message(STATUS "SSE for floating-point operation: DISABLED")
endif()


# ---------------------------------------------------------
# Project options
set(USE_PANGOLIN_VIEWER OFF CACHE BOOL "Enable Pangolin Viewer")
set(USE_SOCKET_PUBLISHER OFF CACHE BOOL "Enable Socket Publisher")
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(BUILD_TESTS OFF CACHE BOOL "Build tests")
set(USE_STACK_TRACE_LOGGER ON CACHE BOOL "OpenVSLAM use stack trace logger")
set(BOW_FRAMEWORK DBoW2 CACHE STRING "G2O use DBow2 framework")

# ---------------------------------------------------------
# Source files

set(OpenVSLAM_SRC
    ./openvslam/src/openvslam/optimize/internal/se3/perspective_reproj_edge.h
    ./openvslam/src/openvslam/optimize/internal/se3/pose_opt_edge_wrapper.h
    ./openvslam/src/openvslam/optimize/internal/se3/reproj_edge_wrapper.h
    ./openvslam/src/openvslam/optimize/internal/sim3/backward_reproj_edge.h
    ./openvslam/src/openvslam/optimize/internal/sim3/forward_reproj_edge.h
    ./openvslam/src/openvslam/optimize/internal/sim3/graph_opt_edge.h
    ./openvslam/src/openvslam/solve/pnp_solver.h
    ./openvslam/src/openvslam/optimize/internal/sim3/shot_vertex.h
    ./openvslam/src/openvslam/tracking_module.h
    ./openvslam/src/openvslam/optimize/internal/sim3/transform_vertex.h
    ./openvslam/src/openvslam/type.h
    ./openvslam/src/openvslam/optimize/internal/landmark_vertex.h
    ./openvslam/src/openvslam/optimize/internal/landmark_vertex_container.h
    ./openvslam/src/openvslam/module/local_map_updater.h
    ./openvslam/src/openvslam/feature/orb_extractor_node.h
    ./openvslam/src/openvslam/optimize/internal/se3/perspective_pose_opt_edge.h
    ./openvslam/src/openvslam/optimize/internal/se3/shot_vertex.h
    ./openvslam/src/openvslam/optimize/internal/se3/shot_vertex_container.h
    ./openvslam/src/openvslam/camera/base.h
    ./openvslam/src/openvslam/camera/radial_division.h
    ./openvslam/src/openvslam/solve/common.h
    ./openvslam/src/openvslam/util/converter.h
    ./openvslam/src/openvslam/solve/essential_solver.h
    ./openvslam/src/openvslam/util/fancy_index.h
    ./openvslam/src/openvslam/solve/fundamental_solver.h
    ./openvslam/src/openvslam/solve/homography_solver.h
    ./openvslam/src/openvslam/util/image_converter.h
    ./openvslam/src/openvslam/util/random_array.h
    ./openvslam/src/openvslam/solve/sim3_solver.h
    ./openvslam/src/openvslam/util/stereo_rectifier.h
    ./openvslam/src/openvslam/system.h
    ./openvslam/src/openvslam/solve/triangulator.h
    ./openvslam/src/openvslam/util/trigonometric.h
    ./openvslam/src/openvslam/publish/frame_publisher.h
    ./openvslam/src/openvslam/optimize/global_bundle_adjuster.h
    ./openvslam/src/openvslam/optimize/graph_optimizer.h
    ./openvslam/src/openvslam/optimize/local_bundle_adjuster.h
    ./openvslam/src/openvslam/publish/map_publisher.h
    ./openvslam/src/openvslam/optimize/pose_optimizer.h
    ./openvslam/src/openvslam/optimize/transform_optimizer.h
    ./openvslam/src/openvslam/module/type.h
    ./openvslam/src/openvslam/match/angle_checker.h
    ./openvslam/src/openvslam/match/area.h
    ./openvslam/src/openvslam/match/base.h
    ./openvslam/src/openvslam/initialize/base.h
    ./openvslam/src/openvslam/initialize/bearing_vector.h
    ./openvslam/src/openvslam/match/bow_tree.h
    ./openvslam/src/openvslam/module/frame_tracker.h
    ./openvslam/src/openvslam/match/fuse.h
    ./openvslam/src/openvslam/module/initializer.h
    ./openvslam/src/openvslam/module/keyframe_inserter.h
    ./openvslam/src/openvslam/module/local_map_cleaner.h
    ./openvslam/src/openvslam/module/loop_bundle_adjuster.h
    ./openvslam/src/openvslam/module/loop_detector.h
    ./openvslam/src/openvslam/io/map_database_io.h
    ./openvslam/src/openvslam/mapping_module.h
    ./openvslam/src/openvslam/initialize/perspective.h
    ./openvslam/src/openvslam/match/projection.h
    ./openvslam/src/openvslam/module/relocalizer.h
    ./openvslam/src/openvslam/match/robust.h
    ./openvslam/src/openvslam/match/stereo.h
    ./openvslam/src/openvslam/io/trajectory_io.h
    ./openvslam/src/openvslam/module/two_view_triangulator.h
    ./openvslam/src/openvslam/data/bow_database.h
    ./openvslam/src/openvslam/data/bow_vocabulary.h
    ./openvslam/src/openvslam/data/camera_database.h
    ./openvslam/src/openvslam/data/common.h
    ./openvslam/src/openvslam/config.h
    ./openvslam/src/openvslam/camera/equirectangular.h
    ./openvslam/src/openvslam/camera/fisheye.h
    ./openvslam/src/openvslam/data/frame.h
    ./openvslam/src/openvslam/data/frame_statistics.h
    ./openvslam/src/openvslam/global_optimization_module.h
    ./openvslam/src/openvslam/data/graph_node.h
    ./openvslam/src/openvslam/data/keyframe.h
    ./openvslam/src/openvslam/data/landmark.h
    ./openvslam/src/openvslam/data/map_database.h
    ./openvslam/src/openvslam/feature/orb_extractor.h
    ./openvslam/src/openvslam/feature/orb_params.h
    ./openvslam/src/openvslam/feature/orb_point_pairs.h
    ./openvslam/src/openvslam/camera/perspective.h
    ./openvslam/src/openvslam/optimize/internal/se3/equirectangular_pose_opt_edge.h
    ./openvslam/src/openvslam/optimize/internal/se3/equirectangular_reproj_edge.h
    ./openvslam/src/openvslam/optimize/internal/sim3/mutual_reproj_edge_wrapper.h

    ./openvslam/src/openvslam/solve/common.cc
    ./openvslam/src/openvslam/optimize/local_bundle_adjuster.cc
    ./openvslam/src/openvslam/solve/pnp_solver.cc
    ./openvslam/src/openvslam/optimize/pose_optimizer.cc
    ./openvslam/src/openvslam/util/random_array.cc
    ./openvslam/src/openvslam/solve/sim3_solver.cc
    ./openvslam/src/openvslam/system.cc
    ./openvslam/src/openvslam/tracking_module.cc
    ./openvslam/src/openvslam/optimize/transform_optimizer.cc
    ./openvslam/src/openvslam/match/area.cc
    ./openvslam/src/openvslam/match/bow_tree.cc
    ./openvslam/src/openvslam/module/frame_tracker.cc
    ./openvslam/src/openvslam/match/fuse.cc
    ./openvslam/src/openvslam/optimize/global_bundle_adjuster.cc
    ./openvslam/src/openvslam/optimize/graph_optimizer.cc
    ./openvslam/src/openvslam/module/initializer.cc
    ./openvslam/src/openvslam/module/keyframe_inserter.cc
    ./openvslam/src/openvslam/module/local_map_updater.cc
    ./openvslam/src/openvslam/module/loop_detector.cc
    ./openvslam/src/openvslam/initialize/perspective.cc
    ./openvslam/src/openvslam/match/projection.cc
    ./openvslam/src/openvslam/module/relocalizer.cc
    ./openvslam/src/openvslam/match/robust.cc
    ./openvslam/src/openvslam/match/stereo.cc
    ./openvslam/src/openvslam/camera/base.cc
    ./openvslam/src/openvslam/data/camera_database.cc
    ./openvslam/src/openvslam/config.cc
    ./openvslam/src/openvslam/camera/fisheye.cc
    ./openvslam/src/openvslam/data/frame.cc
    ./openvslam/src/openvslam/data/keyframe.cc
    ./openvslam/src/openvslam/feature/orb_extractor.cc
    ./openvslam/src/openvslam/camera/radial_division.cc
    ./openvslam/src/openvslam/util/converter.cc
    ./openvslam/src/openvslam/solve/essential_solver.cc
    ./openvslam/src/openvslam/solve/fundamental_solver.cc
    ./openvslam/src/openvslam/solve/homography_solver.cc
    ./openvslam/src/openvslam/util/image_converter.cc
    ./openvslam/src/openvslam/util/stereo_rectifier.cc
    ./openvslam/src/openvslam/publish/frame_publisher.cc
    ./openvslam/src/openvslam/publish/map_publisher.cc
    ./openvslam/src/openvslam/initialize/base.cc
    ./openvslam/src/openvslam/initialize/bearing_vector.cc
    ./openvslam/src/openvslam/module/local_map_cleaner.cc
    ./openvslam/src/openvslam/module/loop_bundle_adjuster.cc
    ./openvslam/src/openvslam/io/map_database_io.cc
    ./openvslam/src/openvslam/mapping_module.cc
    ./openvslam/src/openvslam/io/trajectory_io.cc
    ./openvslam/src/openvslam/module/two_view_triangulator.cc
    ./openvslam/src/openvslam/data/bow_database.cc
    ./openvslam/src/openvslam/data/common.cc
    ./openvslam/src/openvslam/camera/equirectangular.cc
    ./openvslam/src/openvslam/data/frame_statistics.cc
    ./openvslam/src/openvslam/global_optimization_module.cc
    ./openvslam/src/openvslam/data/graph_node.cc
    ./openvslam/src/openvslam/data/landmark.cc
    ./openvslam/src/openvslam/data/map_database.cc
    ./openvslam/src/openvslam/feature/orb_extractor_node.cc
    ./openvslam/src/openvslam/feature/orb_params.cc
    ./openvslam/src/openvslam/camera/perspective.cc
)

add_library(openvslam ${OpenVSLAM_SRC})

# Check BoW Framework
set(BOW_FRAMEWORK "DBoW2" CACHE STRING "DBoW2 or FBoW")
set_property(CACHE BOW_FRAMEWORK PROPERTY STRINGS "DBoW2" "FBoW")
if(BOW_FRAMEWORK MATCHES "DBoW2")
    find_package(DBoW2 CONFIG REQUIRED)
    set(BoW_INCLUDE_DIR ${DBoW2_INCLUDE_DIRS})
    set(BoW_LIBRARY ${DBoW2_LIBS})
    target_compile_definitions(openvslam PUBLIC USE_DBOW2)
elseif(BOW_FRAMEWORK MATCHES "FBoW")
    find_package(fbow REQUIRED)
    set(BoW_INCLUDE_DIR ${fbow_INCLUDE_DIRS})
    set(BoW_LIBRARY ${fBoW_LIBS})
else()
    message(FATAL_ERROR "Invalid BoW framework: ${BOW_FRAMEWORK}")
endif()
message(STATUS "BoW framework: ${BOW_FRAMEWORK} (found in ${BoW_INCLUDE_DIR})")

target_include_directories(openvslam
                           PUBLIC
                           $<BUILD_INTERFACE:${json_INCLUDE_DIR}>
                           $<BUILD_INTERFACE:${spdlog_INCLUDE_DIR}>
                           $<BUILD_INTERFACE:${BoW_INCLUDE_DIR}>
                           $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/openvslam/src/>
                           $<INSTALL_INTERFACE:openvslam/3rd/json/include>
                           $<INSTALL_INTERFACE:openvslam/3rd/spdlog/include>
                           $<INSTALL_INTERFACE:${BoW_INCLUDE_DIR}>
                           $<INSTALL_INTERFACE:include/>)

target_link_libraries(openvslam
                      PUBLIC
                      Threads::Threads
                      #OpenMP::OpenMP_CXX
                      Eigen3::Eigen
                      yaml-cpp
                      opencv_core
                      opencv_features2d
                      opencv_calib3d
                      g2o::core
                      g2o::stuff
                      g2o::types_sba
                      g2o::types_sim3
                      g2o::solver_dense
                      g2o::solver_eigen
                      g2o::solver_csparse
                      g2o::csparse_extension
                      ${CXSPARSE_LIBRARIES}
                      ${SUITESPARSE_LIBRARIES}
                      ${BoW_LIBRARY}
                      PRIVATE
                      ${LAPACK_LIBRARIES})


# OpenMP

set(USE_OPENMP OFF CACHE BOOL "Use OpenMP")
if(USE_OPENMP)
    target_compile_options(openvslam PRIVATE ${OpenMP_CXX_FLAGS})
    target_compile_definitions(openvslam PUBLIC USE_OPENMP)
    message(STATUS "OpenMP: ENABLED")
else()
    message(STATUS "OpenMP: DISABLED")
endif()





