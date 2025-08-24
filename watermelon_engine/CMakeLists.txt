add_subdirectory(vendors/SDL)

add_library(glad STATIC vendors/glad/src/gl.c)
target_include_directories(glad PUBLIC vendors/glad/include)

add_subdirectory(vendors/glm)