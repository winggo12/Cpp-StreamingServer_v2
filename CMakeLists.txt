cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Server)

include_directories(include)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Torch REQUIRED)
include_directories(${/home/hkuit104/libtorch151/libtorch/share/cmake/Torch})

set(SOURCES main.cpp Server.cpp PlayVideo.cpp)

add_executable(Server ${SOURCES})
	
target_link_libraries(Server "${TORCH_LIBRARIES}" "${OpenCV_LIBS}")
set_property(TARGET Server PROPERTY CXX_STANDARD 14)
