cmake_minimum_required(VERSION 3.9)
project(ImageListCreator)

set(CMAKE_CXX_STANDARD 11)

#opencv
find_package(OpenCV REQUIRED)

add_executable(ImageListCreator
        imagelist_creator.cpp
        )

target_link_libraries(ImageListCreator ${OpenCV_LIBS})
