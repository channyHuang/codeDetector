CMAKE_MINIMUM_REQUIRED(VERSION 3.8.2)

set(projectName PnP)
project(${projectName})

find_package(OpenCV REQUIRED)

FILE(GLOB FILES_C "*.cpp")
FILE(GLOB FILES_H "*.h" "*.inl")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${projectName}
	${OpenCV_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	)

# Check C++17 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(COMPILER_SUPPORTS_CXX17)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  add_definitions(-DCOMPILEDWITHC17)
  message(STATUS "Using flag -std=c++17.")
else()
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
endif()

link_directories(${projectName}
	${CMAKE_CURRENT_SOURCE_DIR}/libs
	)

add_executable(${projectName}
${FILES_C}
${FILES_H}
)

target_link_libraries(${projectName}
	${OpenCV_LIBS}
	apriltag
)
