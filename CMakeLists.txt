cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
        simple_wav_encoder
        SHARED
        Wav.cpp)
target_include_directories(simple_wav_encoder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(simple_wav_encoder PRIVATE -Ofast)