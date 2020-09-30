# Project definition
cmake_minimum_required(VERSION 3.1)
project(InitGL VERSION 1.0
        DESCRIPTION "Very nice project"
        LANGUAGES C)

# Source files
include_directories(include)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resource")

file(COPY "${RES_DIR}/" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
# Executable definition and properties
add_executable(${PROJECT_NAME} "${SOURCES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)


add_executable(main ${SOURCES})


# Pulling down SDL2
include(FetchContent)
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY "https://github.com/SDL-mirror/SDL.git"
        GIT_TAG release-2.0.12
)
FetchContent_GetProperties(SDL2)
if (NOT sdl2_POPULATED)
    FetchContent_Populate(SDL2)
    add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR})
endif ()
target_link_libraries(${PROJECT_NAME} SDL2main SDL2-static m)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# mathc
#set(mathc "${LIB_DIR}/mathc")
#target_include_directories(${PROJECT_NAME} PRIVATE "${mathc}")

#cglm
target_include_directories(${PROJECT_NAME} PRIVATE "${LIB_DIR}/cglm/include")
add_subdirectory(${LIB_DIR}/cglm)
target_link_libraries(${LIBRARY_NAME}  cglm)



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


