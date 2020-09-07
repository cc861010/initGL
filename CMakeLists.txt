# Project definition
cmake_minimum_required(VERSION 3.1)
project(InitGL)

# Source files
include_directories(include)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
#set(SOURCES "${SRC_DIR}/main.cpp")
file(GLOB SOURCES  "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")
file(GLOB RESOURCE "${SRC_DIR}/*.frag" "${SRC_DIR}/*.vert" "${SRC_DIR}/*.png")
file(COPY ${RESOURCE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)


# GLFW
# set(GLFW_DIR "${LIB_DIR}/glfw")
# set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
# set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
# set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
# set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
# add_subdirectory("${GLFW_DIR}")
# target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
# target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
# target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# SDL2
set(SDL_DIR "${LIB_DIR}/SDL2")
add_subdirectory("${SDL_DIR}")
target_link_libraries (${PROJECT_NAME} SDL2-static)
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "SDL_INCLUDE_NONE")

# luajit
add_subdirectory("${LIB_DIR}/luajit")

# angle
set(ANGLE_DIR "${LIB_DIR}/angle")
target_include_directories(${PROJECT_NAME} PRIVATE "${ANGLE_DIR}/include")
link_directories(${ANGLE_DIR})
target_link_libraries(${PROJECT_NAME} EGL)
target_link_libraries(${PROJECT_NAME} GLESv2)

# glad
# set(GLAD_DIR "${LIB_DIR}/glad")
# add_library("glad" "${GLAD_DIR}/src/glad.c")
# target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
# target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
# target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")


