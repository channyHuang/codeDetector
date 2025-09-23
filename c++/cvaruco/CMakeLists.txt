CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(Lib False)
project(CVAruco VERSION 1.0.0)

find_package(OpenCV REQUIRED)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/*.cc )

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${PROJECT_NAME}
	${OpenCV_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
message(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 17)
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

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs)

link_directories(${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/libs
	)

if (Lib)
add_library(${PROJECT_NAME} SHARED
${HEADERS}
${FILES}
)
else()
add_executable(${PROJECT_NAME}
${HEADERS}
${FILES}
)
endif()

set_target_properties(${PROJECT_NAME} 
                      PROPERTIES 
                      VERSION 1.0.0 )
                      # SOVERSION 1 )    

target_link_directories(${PROJECT_NAME} PUBLIC
  ${LIB_PATH}
)

if (Lib)
target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
)
else()
target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
)
endif()
