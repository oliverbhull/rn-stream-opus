cmake_minimum_required(VERSION 3.13)
project(react-native-opus)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(
    -fexceptions
    -frtti
    -std=c++17
)

# Resolve project root and library paths
get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../" ABSOLUTE)
set(SHARED_DIR "${PROJECT_ROOT}/cpp")
set(OPUS_LIB_DIR "${PROJECT_ROOT}/android/libs")

# Add opus library (prebuilt)
add_library(opus INTERFACE)
target_include_directories(opus INTERFACE
    ${SHARED_DIR}
)

# Add the main library
add_library(react-native-opus STATIC
    ${SHARED_DIR}/NativeOpusTurboModule.cpp
)

target_include_directories(react-native-opus
    PUBLIC
    ${SHARED_DIR}
    ${PROJECT_ROOT}/android/generated/jni
)

target_link_libraries(react-native-opus
    ${OPUS_LIB_DIR}/libopus-${ANDROID_ABI}.a
    jsi
    react_nativemodule_core
    react_codegen_RNOpusSpec
)