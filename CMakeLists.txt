cmake_minimum_required(VERSION 3.17)
project(sfml-c++)

set(CMAKE_CXX_STANDARD 17)

# Replace this with the path to your SFML installation
set(SFML_DIR "C:/code/2D SFML GAME/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(sfml-c++ src/main.cpp src/character.cpp src/utilities.cpp)
target_link_libraries(sfml-c++ sfml-graphics sfml-audio)