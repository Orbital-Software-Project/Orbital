cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(Core)

Set(SRC

    # Base
    Base/Percentage.hpp

    # Tasks
    Tasks/TaskWorker.h
    Tasks/TaskWorker.cpp
    Tasks/OpenVslamTask.h
    Tasks/OpenVslamTask.cpp
    Tasks/Viso2Task.h
    Tasks/Viso2Task.cpp


)


add_library(Core ${SRC})



target_link_libraries(Core PRIVATE 
    ${OpenCV_LIBS}
    openvslam
	libviso2
)




