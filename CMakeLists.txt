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

# Common helper
function(sokol_shared name src)
  add_library(${name} SHARED ${src})
  target_include_directories(${name} PUBLIC ${SOKOL_DIR})
  target_compile_definitions(${name} PRIVATE SOKOL_DLL SOKOL_IMPL)
  if(MSVC)
    target_compile_definitions(${name} PRIVATE _CRT_SECURE_NO_WARNINGS)
  endif()
endfunction()

# Targets
sokol_shared(sokol_gfx        src/sokol_gfx.c)
sokol_shared(sokol_app        src/sokol_app.c)
sokol_shared(sokol_audio      src/sokol_audio.c)
sokol_shared(sokol_time       src/sokol_time.c)
sokol_shared(sokol_args       src/sokol_args.c)

# Platform links
if(WIN32)
  # Direct3D11 backend and Win32 platform
  target_link_libraries(sokol_gfx   PRIVATE d3d11 dxgi d3dcompiler)
  target_link_libraries(sokol_app   PRIVATE user32 shell32 kernel32 gdi32)
  target_link_libraries(sokol_audio PRIVATE ole32 mmdevapi avrt)
  target_link_libraries(sokol_time  PRIVATE kernel32)
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

  target_link_libraries(sokol_gfx   PRIVATE ${METAL_FRAMEWORK} ${QUARTZCORE_FRAMEWORK})
  target_link_libraries(sokol_app   PRIVATE ${COCOA_FRAMEWORK} ${QUARTZCORE_FRAMEWORK})
  target_link_libraries(sokol_audio PRIVATE ${AUDIO_TOOLBOX_FRAMEWORK} ${AVFOUNDATION_FRAMEWORK} ${FOUNDATION_FRAMEWORK} ${COREAUDIO_FRAMEWORK})
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
    target_link_libraries(sokol_app PRIVATE ${X11_LIB} ${XI_LIB} ${XCURSOR_LIB})
  endif()
  if(GL_LIB)
    target_link_libraries(sokol_gfx PRIVATE ${GL_LIB})
  endif()
  if(DL_LIB)
    target_link_libraries(sokol_gfx PRIVATE ${DL_LIB})
    target_link_libraries(sokol_app PRIVATE ${DL_LIB})
  endif()
  if(M_LIB)
    target_link_libraries(sokol_gfx PRIVATE ${M_LIB})
    target_link_libraries(sokol_app PRIVATE ${M_LIB})
  endif()
  target_link_libraries(sokol_app PRIVATE Threads::Threads)
  if(ASOUND_LIB)
    target_link_libraries(sokol_audio PRIVATE ${ASOUND_LIB})
  endif()
endif()

# Install
include(GNUInstallDirs)
install(TARGETS sokol_gfx sokol_app sokol_audio sokol_time sokol_args
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
