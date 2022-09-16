cmake_minimum_required(VERSION 3.12)

project("COMP3421 Assignment 1")

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(FETCH_DEPS ON CACHE BOOL "Should the build system fetch, build, and install this project's dependencies")
if (FETCH_DEPS)
    set(BUILD_DIR ${PROJECT_SOURCE_DIR}/lib/tmp)

    # Configure the "lib" directory as a separate project (whose sole purpose is to fetch dependencies)
    execute_process(
            COMMAND ${CMAKE_COMMAND}
            -S ${PROJECT_SOURCE_DIR}/lib
            -B ${BUILD_DIR}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    )

    # Build it (actually downloads)
    execute_process(COMMAND ${CMAKE_COMMAND} --build ${BUILD_DIR})

    # Install the binaries (the actual things needed)
    execute_process(COMMAND ${CMAKE_COMMAND} --install ${BUILD_DIR})

    # Clean up after ourselves
    file(REMOVE_RECURSE ${BUILD_DIR})

    # Don't need to refetch the dependencies
    set(FETCH_DEPS OFF CACHE BOOL "" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include (copy_resources)
copy_resources(${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)

find_package(glad REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/glad)
find_package(glfw3 REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/glfw)
find_package(glm REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/glm)
find_package(stb REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/stb)
find_package(chicken3421 REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib/chicken3421)

set(COMMON_LIBS glad::glad glfw glm::glm stb chicken3421)

add_executable(ass1)
target_include_directories(ass1 PUBLIC include)
target_sources(ass1 PRIVATE ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(ass1 PUBLIC ${COMMON_LIBS})


