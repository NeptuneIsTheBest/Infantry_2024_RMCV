cmake_minimum_required(VERSION 3.25)
project(Infantry_2024_CV)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

add_compile_options(-Wall -Werror)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -flto -march=native)
elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-O3 -flto -march=native -g)
elseif (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_options(-Os -flto -march=native)
else ()
    add_compile_options(-O0 -g)
endif ()

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(./include)

if (APPLE)
    include_directories(./includes/MindVision/macOS/include)
    link_directories(./includes/MindVision/macOS/lib)
elseif (UNIX)
    include_directories(./includes/MindVision/Linux/include)
    link_directories(./includes/MindVision/Linux/lib)
endif ()

add_executable(${PROJECT_NAME}
        main.cpp
        src/ImageLoader.cpp
        src/ArmoDetector.cpp
        )

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} mvsdk)

