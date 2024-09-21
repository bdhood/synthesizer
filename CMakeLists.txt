cmake_minimum_required(VERSION 3.18.1)
cmake_policy(SET CMP0054 NEW)

include(ExternalProject)

find_package(PkgConfig REQUIRED)

message(${CMAKE_SOURCE_DIR})
message(${CMAKE_BINARY_DIR})
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source and call cmake from there")
endif()

function(get_source URL FILENAME)
    set(DEST "${CMAKE_SOURCE_DIR}/libraries/${FILENAME}")
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
    file(ARCHIVE_EXTRACT INPUT ${DEST})
endfunction()

project(synth6 C)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

if (MSVC)
    set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    list(APPEND EXTRA_LIBS vcruntime)
endif()

set(SDL2_VERSION "2.30.7")
set(SDL2_ttf_VERSION "2.22.0")
set(portaudio_VERSION "v190700_20210406")
set(zlib_VERSION "1.3.1")
set(cJSON_VERSION "1.7.18")

include_directories(
    ${CMAKE_BINARY_DIR}/asiosdk_2.3.3_2019-06-14/common
    ${CMAKE_BINARY_DIR}/asiosdk_2.3.3_2019-06-14/host
    ${CMAKE_BINARY_DIR}/portaudio/include
    ${CMAKE_BINARY_DIR}/portaudio/src/common
    ${CMAKE_BINARY_DIR}/portaudio/src/os/win
    ${CMAKE_BINARY_DIR}/build/SDL2-${SDL2_VERSION}/include/SDL2
    ${CMAKE_BINARY_DIR}/SDL2_ttf-${SDL2_ttf_VERSION}
    ${CMAKE_BINARY_DIR}/cJSON-${cJSON_VERSION}
)

# asiosdk2
# https://www.steinberg.net/en/home.html
get_source(
    https://www.steinberg.net/asiosdk 
    asiosdk2.zip 
)

# portaudio
# http://www.portaudio.com/
set(PA_DISABLE_INSTALL ON)
set(PA_USE_DS OFF CACHE INTERNAL "direct sound")
set(PA_USE_WMME OFF CACHE INTERNAL "wmme")
set(PA_USE_WASAPI OFF CACHE INTERNAL "wasapi")
set(PA_USE_WDMKS OFF CACHE INTERNAL "wdks")
set(PA_USE_ASIO ON CACHE INTERNAL "asio")

get_source(
    https://files.portaudio.com/archives/pa_stable_${portaudio_VERSION}.tgz
    pa_stable_${portaudio_VERSION}.tgz
)

add_subdirectory(${CMAKE_BINARY_DIR}/portaudio EXCLUDE_FROM_ALL)

# cJSON
# https://github.com/DaveGamble/cJSON

get_source(
    https://github.com/DaveGamble/cJSON/archive/refs/tags/v${cJSON_VERSION}.tar.gz
    cJSON-v${cJSON_VERSION}.tar.gz
)
set(ENABLE_CJSON_TEST OFF CACHE INTERNAL "cJSON tests")
set(ENABLE_CJSON_UNINSTALL OFF CACHE INTERNAL "cJSON uninstall")
add_subdirectory(${CMAKE_BINARY_DIR}/cJSON-${cJSON_VERSION})

# SDL2
# https://www.libsdl.org/

set(SDL_STATIC ON CACHE INTERNAL "build static")
set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
set(SDL2_LIBRARIES ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY})
get_source(
    http://www.libsdl.org/release/SDL2-${SDL2_VERSION}.tar.gz
    SDL2-${SDL2_VERSION}.tar.gz
)

add_subdirectory(${CMAKE_BINARY_DIR}/SDL2-${SDL2_VERSION})

# freetype
# https://www.freetype.org/index.html

# set(Freetype_VERSION "2.13.2")
# set(FREETYPE_LIBRARY ${CMAKE_BINARY_DIR}/freetype-${Freetype_VERSION})
# get_source(
#     https://download.savannah.gnu.org/releases/freetype/freetype-${Freetype_VERSION}.tar.gz
#     freetype-${Freetype_VERSION}.tar.gz
# )

# add_subdirectory(${CMAKE_BINARY_DIR}/freetype-${Freetype_VERSION})
# unset(Z_HAVE_UNISTD_H)

# SDL2_ttf
# https://github.com/libsdl-org/SDL_ttf
get_source(
    https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-${SDL2_ttf_VERSION}.tar.gz
    SDL2_ttf-${SDL2_ttf_VERSION}.tar.gz
)

# add_library(SDL2::SDL2 ALIAS SDL2-static)
# add_library(Freetype::Freetype ALIAS freetype)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
add_subdirectory(${CMAKE_BINARY_DIR}/SDL2_ttf-${SDL2_ttf_VERSION})

# zlib
# http://zlib.net

get_source(
    https://www.zlib.net/zlib-${zlib_VERSION}.tar.gz
    zlib-${zlib_VERSION}.tar.gz
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
    set(source "int ${resource_name}_length = ${data_length};\nunsigned char *${resource_name} = \n${content};\n\n")
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