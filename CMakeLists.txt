cmake_minimum_required(VERSION 3.15)
project(lib_sokol C)

# Options
set(SOKOL_DIR "" CACHE PATH "Path to sokol headers (folder containing sokol_*.h)")
if(NOT SOKOL_DIR)
  if(DEFINED ENV{SOKOL_DIR})
    set(SOKOL_DIR "$ENV{SOKOL_DIR}")
  else()
    set(SOKOL_DIR "${CMAKE_CURRENT_LIST_DIR}/third_party/sokol")
  endif()
endif()

message(STATUS "Using SOKOL_DIR=${SOKOL_DIR}")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Single combined shared library
add_library(sokol SHARED src/sokol_all.c)
target_include_directories(sokol PUBLIC ${SOKOL_DIR})
target_compile_definitions(sokol PRIVATE SOKOL_DLL)
if(MSVC)
  target_compile_definitions(sokol PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Platform links
if(WIN32)
  # Direct3D11 backend and Win32 platform
  target_link_libraries(sokol PRIVATE d3d11 dxgi d3dcompiler user32 shell32 kernel32 gdi32 ole32 mmdevapi avrt)
elseif(APPLE)
  # Metal + Cocoa
  find_library(COCOA_FRAMEWORK Cocoa)
  find_library(QUARTZCORE_FRAMEWORK QuartzCore)
  find_library(METAL_FRAMEWORK Metal)
  find_library(METALKIT_FRAMEWORK MetalKit)
  find_library(AUDIO_TOOLBOX_FRAMEWORK AudioToolbox)
  find_library(AVFOUNDATION_FRAMEWORK AVFoundation)
  find_library(FOUNDATION_FRAMEWORK Foundation)
  find_library(COREAUDIO_FRAMEWORK CoreAudio)

  target_link_libraries(sokol PRIVATE ${COCOA_FRAMEWORK} ${QUARTZCORE_FRAMEWORK} ${METAL_FRAMEWORK} ${METALKIT_FRAMEWORK} ${AUDIO_TOOLBOX_FRAMEWORK} ${AVFOUNDATION_FRAMEWORK} ${FOUNDATION_FRAMEWORK} ${COREAUDIO_FRAMEWORK})
elseif(UNIX)
  # GL + X11, ALSA on Linux
  find_package(Threads REQUIRED)
  find_library(X11_LIB X11)
  find_library(XI_LIB Xi)
  find_library(XCURSOR_LIB Xcursor)
  find_library(GL_LIB GL)
  find_library(DL_LIB dl)
  find_library(M_LIB m)
  find_library(ASOUND_LIB asound)

  if(X11_LIB)
    target_link_libraries(sokol PRIVATE ${X11_LIB} ${XI_LIB} ${XCURSOR_LIB})
  endif()
  if(GL_LIB)
    target_link_libraries(sokol PRIVATE ${GL_LIB})
  endif()
  if(DL_LIB)
    target_link_libraries(sokol PRIVATE ${DL_LIB})
  endif()
  if(M_LIB)
    target_link_libraries(sokol PRIVATE ${M_LIB})
  endif()
  target_link_libraries(sokol PRIVATE Threads::Threads)
  if(ASOUND_LIB)
    target_link_libraries(sokol PRIVATE ${ASOUND_LIB})
  endif()
endif()

# Install
include(GNUInstallDirs)
install(TARGETS sokol
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
