cmake_minimum_required(VERSION 3.12)

project("COMP3421 Assignment 1_lib")

include (ExternalProject)

ExternalProject_Add(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG origin/master
        GIT_PROGRESS ON
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_LIST_DIR}/glm -DBUILD_TESTING=OFF
)

ExternalProject_Add(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad
    GIT_TAG origin/master
    GIT_PROGRESS ON
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_LIST_DIR}/glad -DGLAD_API="gl=3.3" -DGLAD_INSTALL=ON
)

ExternalProject_Add(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG origin/master
        GIT_PROGRESS ON
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_LIST_DIR}/glfw
            -DGLFW_BUILD_EXAMPLES=OFF
            -DGLFW_BUILD_TESTS=OFF
            -DGLFW_BUILD_DOCS=OFF
)

ExternalProject_Add(
        "stb-cmake"
        GIT_REPOSITORY "https://gitlab.cse.unsw.edu.au/COMP3421/stb-cmake-package"
        GIT_TAG origin/master
        GIT_PROGRESS ON
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_LIST_DIR}/stb
)

ExternalProject_Add(
        chicken3421
        GIT_REPOSITORY https://gitlab.cse.unsw.edu.au/COMP3421/chicken3421-package.git # no idea why but this needs .git
        GIT_TAG origin/master
        GIT_PROGRESS ON
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_LIST_DIR}/chicken3421
)
