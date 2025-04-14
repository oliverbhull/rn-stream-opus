cmake_minimum_required(VERSION 3.13)
project(react-native-opus)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(
    -fexceptions
    -frtti
    -std=c++17
)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../" ABSOLUTE)
set(SHARED_DIR "${PROJECT_ROOT}/cpp")
set(OPUS_LIB_DIR "${PROJECT_ROOT}/android/libs")

# Remove the IMPORTED library definition
# add_library(opus STATIC IMPORTED)
# set_target_properties(opus PROPERTIES ...)

add_library(react-native-opus STATIC
    ${SHARED_DIR}/NativeOpusTurboModule.cpp
)

target_include_directories(react-native-opus
    PUBLIC
    ${SHARED_DIR}
    # Add opus include path directly
    ${SHARED_DIR}/opus
)

target_link_libraries(react-native-opus
    # Link the absolute path to the prebuilt library again
    ${OPUS_LIB_DIR}/libopus-${ANDROID_ABI}.a
    jsi
    react_nativemodule_core
    react_codegen_RNOpusSpec
)