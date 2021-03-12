cmake_minimum_required(VERSION 3.10)
project(vox LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)


include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${THIRDPARTY_DIR}/GLFW/include
    ${THIRDPARTY_DIR}/GLEW/include
    ${THIRDPARTY_DIR}/glm
    ${THIRDPARTY_DIR}/TinyObj
)

file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB INC ${CMAKE_SOURCE_DIR}/include/*.h)

add_executable(run ${SRC} ${INC})
target_link_libraries(run
    opengl32
    ${THIRDPARTY_DIR}/GLFW/glfw3.lib
    ${THIRDPARTY_DIR}/GLEW/glew32s.lib
)