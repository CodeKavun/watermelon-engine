add_subdirectory(vendors/SDL)

add_library(glad STATIC vendors/glad/src/gl.c)
target_include_directories(glad PUBLIC vendors/glad/include)

add_subdirectory(vendors/glm)

add_library(
    WatermelonEngine
    core.hpp core.cpp
    gfx.hpp gfx.cpp
    utils.hpp utils.cpp
    engine_types.hpp
    input.hpp input.cpp
)

target_link_libraries(WatermelonEngine PUBLIC opengl32 SDL3::SDL3 glad glm::glm)
target_include_directories(WatermelonEngine PUBLIC ${CMAKE_SOURCE_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/SDL3.dll DESTINATION ${CMAKE_BINARY_DIR}/testgame/)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/testgame/)