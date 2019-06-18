message(STATUS "Including dependents_win.cmake ...")

if(MSVC)
    SET(APP_C_FLAGS "/MP /FS /Od /Zi /GS-")
    SET(APP_CXX_FLAGS "/MP /FS /Od /Zi /EHsc /wd4355 /wd4800")
    
    string(REPLACE "/O2" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "/Ob1" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(APP_EXE_LINKER_FLAGS "/machine:x64")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /machine:x64")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /machine:x64")
    message(STATUS "Building for x64")
endif()

set(PLATFORM_SPECIFIC_INCLUDE_DIR "")
set(excluded_HEADER_FILES_REGEX "^.*(_mac|_unix|.*/mac/.*).h$")
set(excluded_SOURCE_FILES_REGEX "^.*(_mac|_unix|.*/mac/.*).cpp$")

set(PLATFORM_SPECIFIC_SOURCE_FILES "")

set(PLATFORM_SPECIFIC_LIBRARIES 

    )
    
set(PLATFORM_SPECIFIC_LIB_DIRS "")


