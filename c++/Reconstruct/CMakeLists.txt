CMAKE_MINIMUM_REQUIRED(VERSION 3.8.2)

project(PnP)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)
find_package(glog REQUIRED)
find_package(PoseLib REQUIRED)

set(EIGEN_DIR "/usr/local/include/eigen3")
set(CERES_INCLUDE_DIR "/usr/local/include")
set(GTEST_DIR "/usr/local/include/gtest")

FILE(GLOB FILES_C "*.cpp" "*.cc")
FILE(GLOB FILES_CC ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/include/utils/*.cc )
FILE(GLOB FILES_H "*.h" "*.hpp" "*.inl" ${CMAKE_CURRENT_SOURCE_DIR}/includes/*.h)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_definitions("-DGLOG_USE_GLOG_EXPORT")
if(DEFINED glog_VERSION_MAJOR)
  # Older versions of glog don't export version variables.
  add_definitions("-DGLOG_VERSION_MAJOR=${glog_VERSION_MAJOR}")
  add_definitions("-DGLOG_VERSION_MINOR=${glog_VERSION_MINOR}")
endif()
message("glog include = ${GLOG_INCLUDE_DIRS}")
include_directories(${PROJECT_NAME}
	${EIGEN_DIR}
  ${CERES_INCLUDE_DIR}
	${GLOG_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/includes
  ${OpenCV_INCLUDE_DIRS}
  ${GTEST_DIR}
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

link_directories(${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/libs
	)

add_executable(${PROJECT_NAME}
${FILES_C}
${FILES_CC}
${FILES_H}
)

target_link_libraries(${PROJECT_NAME}
	${OpenCV_LIBS}
  glog
  PoseLib
  gtest
  pthread

  CVAruco
)
