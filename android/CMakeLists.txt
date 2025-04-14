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

# Define opus as an IMPORTED library since it's prebuilt
add_library(opus STATIC IMPORTED)

# Set the location of the prebuilt library and the include directories
set_target_properties(opus PROPERTIES
    IMPORTED_LOCATION "${OPUS_LIB_DIR}/libopus-${ANDROID_ABI}.a"
    INTERFACE_INCLUDE_DIRECTORIES "${SHARED_DIR}/opus"
)

add_library(react-native-opus STATIC
    ${SHARED_DIR}/NativeOpusTurboModule.cpp
)

target_include_directories(react-native-opus
    PUBLIC
    ${SHARED_DIR}
)

target_link_libraries(react-native-opus
    # Link the opus target name now
    opus
    jsi
    react_nativemodule_core
    react_codegen_RNOpusSpec
)