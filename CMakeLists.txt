cmake_minimum_required(VERSION 3.18.1)
cmake_policy(SET CMP0054 NEW)

include(ExternalProject)

message(${CMAKE_SOURCE_DIR})
message(${CMAKE_BINARY_DIR})
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source and call cmake from there")
endif()

function(get_source URL FILENAME HASH)
    set(DEST "${CMAKE_SOURCE_DIR}/libraries/${FILENAME}")
    if(EXISTS ${DEST}) 
        file(SHA256 ${DEST} HASH_VALUE)
        if (NOT ${HASH} STREQUAL ${HASH_VALUE})
            file(REMOVE ${DEST})
        endif()
    endif()
    if(NOT EXISTS ${DEST})
        file(
            DOWNLOAD
            ${URL} 
            ${DEST}
            TIMEOUT 60
            TLS_VERIFY ON
            SHOW_PROGRESS
        )
    endif()
    if(EXISTS ${DEST}) 
        file(SHA256 ${DEST} HASH_VALUE)
        if (NOT ${HASH} STREQUAL ${HASH_VALUE})
            file(REMOVE ${DEST})
        endif()
    endif()
    file(ARCHIVE_EXTRACT INPUT ${DEST})
endfunction()

project(synth6 C)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

if (MSVC)
    set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    list(APPEND EXTRA_LIBS vcruntime)
endif()

include_directories(
    ${CMAKE_BINARY_DIR}/asiosdk_2.3.3_2019-06-14/common
    ${CMAKE_BINARY_DIR}/asiosdk_2.3.3_2019-06-14/host
    ${CMAKE_BINARY_DIR}/portaudio/include
    ${CMAKE_BINARY_DIR}/portaudio/src/common
    ${CMAKE_BINARY_DIR}/portaudio/src/os/win
    ${CMAKE_BINARY_DIR}/build/SDL2-2.0.12/include
    ${CMAKE_BINARY_DIR}/SDL2-2.0.12/include
    ${CMAKE_BINARY_DIR}/SDL2_ttf-2.0.15
    ${CMAKE_BINARY_DIR}/DaveGamble-cJSON-39853e5
)

# asiosdk2
# https://www.steinberg.net/en/home.html

get_source(
    https://www.steinberg.net/asiosdk 
    asiosdk2.zip 
    1e044e26c3ce275ee08007d524e97ef5ab36e3aa5efbcb1e44844542d7c13ab2
)

# portaudio
# http://www.portaudio.com/

set(portaudio_VERSION "v190600_20161030")

set(PA_USE_DS OFF CACHE INTERNAL "direct sound")
set(PA_USE_WMME OFF CACHE INTERNAL "wmme")
set(PA_USE_WASAPI OFF CACHE INTERNAL "wasapi")
set(PA_USE_WDMKS OFF CACHE INTERNAL "wdks")
set(PA_USE_ASIO ON CACHE INTERNAL "asio")

get_source(
    http://www.portaudio.com/archives/pa_stable_${portaudio_VERSION}.tgz
    pa_stable_${portaudio_VERSION}.tgz
    f5a21d7dcd6ee84397446fa1fa1a0675bb2e8a4a6dceb4305a8404698d8d1513
)

add_subdirectory(${CMAKE_BINARY_DIR}/portaudio EXCLUDE_FROM_ALL)

# cJSON
# https://github.com/DaveGamble/cJSON

get_source(
    https://github.com/DaveGamble/cJSON/tarball/v1.7.13
    DaveGamble-cJSON-v1.7.13-0-g39853e5.tar.gz
    0901b563429fde89709df13be8dbd69c8c3c736edb0a931590c730f3fa8c9c6e
)
set(ENABLE_CJSON_TEST OFF CACHE INTERNAL "cJSON tests")
add_subdirectory(${CMAKE_BINARY_DIR}/DaveGamble-cJSON-39853e5)

# SDL2
# https://www.libsdl.org/

set(SDL2_VERSION "2.0.12")
set(SDL_STATIC ON CACHE INTERNAL "build static")
set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
set(SDL2_LIBRARIES ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY})
get_source(
    http://www.libsdl.org/release/SDL2-${SDL2_VERSION}.tar.gz
    SDL2-${SDL2_VERSION}.tar.gz
    349268f695c02efbc9b9148a70b85e58cefbbf704abd3e91be654db7f1e2c863
)

add_subdirectory(${CMAKE_BINARY_DIR}/SDL2-${SDL2_VERSION})

# freetype
# https://www.freetype.org/index.html

set(Freetype_VERSION "2.10.2")
set(FREETYPE_LIBRARY ${CMAKE_BINARY_DIR}/freetype-${Freetype_VERSION})
get_source(
    https://download.savannah.gnu.org/releases/freetype/freetype-${Freetype_VERSION}.tar.gz
    freetype-${Freetype_VERSION}.tar.gz
    e09aa914e4f7a5d723ac381420949c55c0b90b15744adce5d1406046022186ab
)

add_subdirectory(${CMAKE_BINARY_DIR}/freetype-${Freetype_VERSION})
unset(Z_HAVE_UNISTD_H)

# SDL2_ttf
# https://www.libsdl.org/projects/SDL_ttf/

set(SDL2_ttf_VERSION "2.0.15")
get_source(
    https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-${SDL2_ttf_VERSION}.tar.gz
    SDL2_ttf-${SDL2_ttf_VERSION}.tar.gz
    a9eceb1ad88c1f1545cd7bd28e7cbc0b2c14191d40238f531a15b01b1b22cd33
)

add_library(SDL2::SDL2 ALIAS SDL2-static)
add_library(Freetype::Freetype ALIAS freetype)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
add_subdirectory(${CMAKE_BINARY_DIR}/SDL2_ttf-${SDL2_ttf_VERSION})

# zlib
# http://zlib.net

set(zlib_VERSION "1.2.11")
get_source(
    http://www.zlib.net/zlib-${zlib_VERSION}.tar.gz
    zlib-${zlib_VERSION}.tar.gz
    c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1
)

add_subdirectory(${CMAKE_BINARY_DIR}/zlib-${zlib_VERSION})

file(GLOB_RECURSE SRC_COMPONENTS
    src/components/mixer.c
    src/components/normalizer.c
    src/components/synth.c
)

file(GLOB_RECURSE HEADERS_COMPONENTS
    src/components/mixer.h
    src/components/normalizer.h
    src/components/synth.h
)

file(GLOB_RECURSE SRC_INSTRUMENTS
    src/instruments/saw.c
    src/instruments/sine.c
    src/instruments/square.c
)

file(GLOB_RECURSE HEADERS_INSTRUMENTS
    src/instruments/saw.h
    src/instruments/sine.h
    src/instruments/square.h
)

file(GLOB_RECURSE SRC_UI
    src/ui/ui.c
    src/ui/window.c
)

file(GLOB_RECURSE HEADERS_UI
    src/ui/ui.h
    src/ui/window.h
)

file(GLOB_RECURSE SRC_UI_CONTROLS
    src/ui/controls/label.c
    src/ui/controls/fps_counter.c
    src/ui/controls/waveform.c
    src/ui/controls/keyboard_ctrl.c
    src/ui/controls/notes_text_ctrl.c
)

file(GLOB_RECURSE HEADERS_UI_CONTROLS
    src/ui/controls/label.h
    src/ui/controls/fps_counter.h
    src/ui/controls/waveform.h
    src/ui/controls/keyboard_ctrl.h
    src/ui/controls/notes_text_ctrl.h
)

file(GLOB_RECURSE SRC_KEYBOARD
    src/keyboard/activekeys.c
    src/keyboard/activenotes.c
    #src/keyboard/keyboard.c
    src/keyboard/keymap.c
)

file(GLOB_RECURSE HEADERS_KEYBOARD
    src/keyboard/activekeys.h
    src/keyboard/activenotes.h
    src/keyboard/keyboard.h
    src/keyboard/keymap.h
)

if (${WIN32})
    file(GLOB_RECURSE SRC_KEYBOARD_OS
        src/keyboard/os/win/keyboard.c
    )
endif()

file(GLOB_RECURSE SRC_MUSIC
    src/music/freqmap.c
    src/music/transpose.c
    src/music/note.c
)

file(GLOB_RECURSE HEADERS_MUSIC
    src/music/freqmap.h
    src/music/transpose.h
    src/music/note.h
)

file(GLOB_RECURSE SRC_MAIN
    src/synthesizer.cpp
)

file(GLOB_RECURSE SRC_SETTINGS
    src/settings/settings.c
)

file(GLOB_RECURSE HEADERS_SETTINGS
    src/settings/settings.h
    src/settings/resources.h
)

file(GLOB_RECURSE SRC_FILEIO
    src/fileio/fileio.c
)

if (${WIN32}) 
    file(GLOB_RECURSE SRC_FILEIO_OS
        src/fileio/os/win/mkdir.c
    )
else()
    file(GLOB_RECURSE SRC_FILEIO_OS
        src/fileio/os/unix/mkdir.c
    )
endif()

file(GLOB_RECURSE HEADERS_FILEIO
    src/fileio/fileio.h
)

include_directories(
    src/components
    src/instruments
    src/ui
    src/ui/controls
    src/keyboard
    src/music
    src/settings
    src/fileio
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    ${SRC_MAIN}

    ${SRC_COMPONENTS}
    ${SRC_INSTRUMENTS}
    ${SRC_UI}
    ${SRC_UI_CONTROLS}
    ${SRC_KEYBOARD}
    ${SRC_KEYBOARD_OS}
    ${SRC_MUSIC}
    ${SRC_SETTINGS}
    ${SRC_FILEIO}
    ${SRC_FILEIO_OS}

    ${HEADERS_COMPONENTS}
    ${HEADERS_INSTRUMENTS}
    ${HEADERS_UI}
    ${HEADERS_UI_CONTROLS}
    ${HEADERS_KEYBOARD}
    ${HEADERS_MUSIC}
    ${HEADERS_SETTINGS}
    ${HEADERS_FILEIO}
)


function(add_resource filename resource_name)
    file(READ "${filename}" hex_content HEX)
    string(LENGTH "${hex_content}" data_length)
    math(EXPR data_length "${data_length}/2")
    string(REPEAT "[0-9a-f]" 64 column_pattern)
    string(REGEX REPLACE "(${column_pattern})" "\t\"\\1\"\n" content "${hex_content}")
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "\\\\x\\1" content "${content}")
    string(REGEX REPLACE "\n\\\\" "\n\t\"\\\\" content "${content}\"")
    set(source "int ${resource_name}_length = ${data_length};\nconst unsigned char *${resource_name} = \n${content};\n\n")
    file(APPEND "src/settings/resources.h" "${source}")
endfunction()

set(source "#pragma once\n\n// Auto generated from CMakeLists.txt\n// Do not commit\n\n")
file(WRITE "src/settings/resources.h" "${source}")
add_resource("fonts/Segoe UI-Light.ttf" "default_font")
add_resource("layouts/US-standard.json" "default_key_layout")
add_resource("settings.json" "default_settings")

file(COPY fonts DESTINATION .)

target_link_libraries(${PROJECT_NAME}
    portaudio_static
    SDL2-static
    SDL2_ttf
    SDL2main
    cjson
)

source_group("Source Files" FILES ${SRC_MAIN})
source_group("Source Files\\components" FILES ${SRC_COMPONENTS})
source_group("Source Files\\instruments" FILES ${SRC_INSTRUMENTS})
source_group("Source Files\\ui" FILES ${SRC_UI})
source_group("Source Files\\ui\\controls" FILES ${SRC_UI_CONTROLS})
source_group("Source Files\\keyboard" FILES ${SRC_KEYBOARD})
source_group("Source Files\\settings" FILES ${SRC_SETTINGS})
source_group("Source Files\\fileio" FILES ${SRC_FILEIO})
if (${WIN32})
    source_group("Source Files\\keyboard\\os\\win" FILES ${SRC_KEYBOARD_OS})
    source_group("Source Files\\fileio\\os\\win" FILES ${SRC_FILEIO_OS})
else()
    source_group("Source Files\\fileio\\os\\unix" FILES ${SRC_FILEIO_OS})
endif()
source_group("Source Files\\music" FILES ${SRC_MUSIC})

source_group("Header Files\\components" FILES ${HEADERS_COMPONENTS})
source_group("Header Files\\instruments" FILES ${HEADERS_INSTRUMENTS})
source_group("Header Files\\ui" FILES ${HEADERS_UI})
source_group("Header Files\\ui\\controls" FILES ${HEADERS_UI_CONTROLS})
source_group("Header Files\\keyboard" FILES ${HEADERS_KEYBOARD})
source_group("Header Files\\music" FILES ${HEADERS_MUSIC})
source_group("Header Files\\settings" FILES ${HEADERS_SETTINGS})
source_group("Header Files\\fileio" FILES ${HEADERS_FILEIO})