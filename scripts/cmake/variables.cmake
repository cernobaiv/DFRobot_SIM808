message(STATUS "Including variables.cmake ...")

# Setting the APP_COMPILE_TYPE (release or debug), used for constructing file paths depending on the CMAKE_BUILD_TYPE.
if(CMAKE_BUILD_TYPE)
    if(CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo OR CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
        set(APP_COMPILE_TYPE release)
    else()
        set(APP_COMPILE_TYPE debug)
    endif()
else()
    set(APP_COMPILE_TYPE debug)
endif()

if(WIN32)
    set(PLATFORM_DIR win)
elseif(APPLE)
    set(PLATFORM_DIR mac)
else(APPLE)
    set(PLATFORM_DIR unix)
endif()

set(TARGET_POSTFIX "")

if(APP_COMPILE_TYPE STREQUAL debug)
   set(TARGET_POSTFIX "d")
   set(LONG_TARGET_POSTFIX "debug")
else()
   set(LONG_TARGET_POSTFIX "release")
endif()

set(EXTERNALS_ROOT "Q/Victor_space/oDesk/externals")
