message("Including dependents_unix.cmake ...")

# Seting the C++ compiler flags.
set(APP_CXX_FLAGS "-Wall -fno-elide-constructors")

# Seting the exe linker flags (empty currently).
set(APP_EXE_LINKER_FLAGS)

# Seting the resource file name variable (empty currently).
set(PLATFORM_SPECIFIC_RESOURCE_FILE)

# Defining platform specific header file locations.
set(PLATFORM_SPECIFIC_INCLUDE_DIR "")

# Defining platform specific regexps for excluding h and cpp files
# On Mac OS X all header files that are placed in win or jumplist sub-folders or end in _win.h will be excluded. The similar applies for source files too.
set(excluded_HEADER_FILES_REGEX "^.*(_win|.*/win/.*|.*/jumplist/.*).h$")
set(excluded_SOURCE_FILES_REGEX "^.*(_win|.*/win/.*|.*/jumplist/.*).cpp$")

# Adding 3rd party library paths to the CMake's list.
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}
    )
    
# Defining platform specific libraries list be added in the linking phase.
set(PLATFORM_SPECIFIC_LIBRARIES 
    #pthread
    )
    
set(PLATFORM_SPECIFIC_DEFS 
    #-D_CRT_SECURE_NO_WARNINGS
    )

# add_definitions(-DHAVE_STD_TR1_SHARED_PTR)
