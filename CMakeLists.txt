cmake_minimum_required(VERSION 3.28)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)

project(c4recover)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Detected compiler: ${CMAKE_CXX_COMPILER_ID}")

set(C4RECOVER_VERSION "01.00.00")
configure_file(c4recover-version.hpp.in c4recover-version.hpp)

set(Boost_VERBOSE ON)
set(Boost_NO_WARN_NEW_VERSIONS ON)
set(BOOST_IOSTREAMS_NO_LIB ON)
if (POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif ()
find_package(Boost 1.85.0 COMPONENTS system filesystem REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost found")
endif ()

set(ZLIB_USE_STATIC_LIBS ON)
find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    message(STATUS "ZLIB found")
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(STATIC_LINK_FLAGS "-static-libgcc -static-libstdc++")
    set(C4_LIBRARY "${C4_LIB_DIR}/libc4-clang.a")
    string(REPLACE "zlib.lib" "libz.a" ZLIB_LIBRARIES "${ZLIB_LIBRARIES}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(STATIC_LINK_FLAGS "-static-libgcc -static-libstdc++")
    set(C4_LIBRARY "${C4_LIB_DIR}/libc4-gnu.a")
    string(REPLACE "zlib.lib" "libz.a" ZLIB_LIBRARIES "${ZLIB_LIBRARIES}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(C4_LIBRARY "${C4_LIB_DIR}/c4.lib")
    else ()
        set(C4_LIBRARY "${C4_LIB_DIR}/c4d.lib")
    endif ()
else ()
    message(FATAL_ERROR, "Unsupported compiler ${CMAKE_CXX_COMPILER_ID}.")
endif ()

set(C4RECOVER_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(C4RECOVER_INCLUDE ${C4RECOVER_ROOT})

set(INCLUDE_SOURCE_FILES
        include/c4lib.hpp
        include/exceptions.hpp
        include/logger.hpp
        include/node-attributes.hpp
        include/node-type.hpp
)

set(EXE_SOURCE_FILES
        src/limits.hpp
        src/main.cpp
        src/text.hpp
        src/util.hpp
        src/util.cpp
        src/exceptions.hpp
        src/constants.hpp
)

add_executable(c4recover ${EXE_SOURCE_FILES})
target_include_directories(c4recover SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(c4recover PRIVATE ${C4RECOVER_INCLUDE} ${C4_INCLUDE} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(c4recover ${C4_LIBRARY} ${ZLIB_LIBRARIES} ${STATIC_LINK_FLAGS})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -Werror -Wextra -Wpedantic -Wshadow -Wno-unknown-pragmas")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=3 -fstack-protector-all -mshstk -fcf-protection=full")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -Werror -Wextra -Wpedantic -Wshadow -Wno-unknown-pragmas")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=3 -fstack-protector-all -mshstk -fcf-protection=full")
    target_compile_options(c4recover PRIVATE -Wnrvo)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /wd4068 /WX /Zc:__cplusplus")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS /guard:cf /sdl")
    target_compile_options(c4recover PRIVATE /analyze:external-)
    add_link_options(/guard:cf)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /RTCs")
    endif ()
endif ()

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

configure_file(${C4RECOVER_ROOT}/doc/BTS.schema ${C4RECOVER_ROOT}/dist/c4recover/BTS.schema COPYONLY)
configure_file(${C4RECOVER_ROOT}/src/config.xml ${C4RECOVER_ROOT}/dist/c4recover/config.xml COPYONLY)
configure_file(${C4RECOVER_ROOT}/LICENSE ${C4RECOVER_ROOT}/dist/c4recover/LICENSE COPYONLY)
configure_file(${C4RECOVER_ROOT}/QUICKSTART.md ${C4RECOVER_ROOT}/dist/c4recover/QUICKSTART.md COPYONLY)

if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(C4RECOVER_DIST_NAME "c4recover.exe")
        endif ()
    endif ()
endif ()

if (DEFINED C4RECOVER_DIST_NAME)
    add_custom_command(TARGET c4recover
            POST_BUILD
            COMMENT "Copying c4recover to dist"
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c4recover> ${C4RECOVER_ROOT}/dist/c4recover/${C4RECOVER_DIST_NAME}
    )
endif ()

cmake_path(CONVERT "${C4RECOVER_ROOT}/dist/release/" TO_NATIVE_PATH_LIST RELEASE_NATIVE)
cmake_path(CONVERT "${C4RECOVER_ROOT}/dist/c4recover" TO_NATIVE_PATH_LIST DIST_C4RECOVER_NATIVE)
add_custom_target(make_release
        COMMENT "Creating release"
        COMMAND 7z a -r ${RELEASE_NATIVE}c4recover-${C4RECOVER_VERSION}.zip ${DIST_C4RECOVER_NATIVE}
)
