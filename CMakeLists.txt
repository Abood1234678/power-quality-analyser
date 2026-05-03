cmake_minimum_required(VERSION 3.21)
project(Portfolio C)

set(CMAKE_C_STANDARD 99)

add_executable(Portfolio main.c waveform.h waveform.c io.h io.c)
target_link_libraries(Portfolio m)