message(STATUS "Including utils.cmake ...")

# Returns a list of all distinct header files in the given root_dir
MACRO(HEADER_DIRECTORIES return_list root_dir)
    FILE(GLOB_RECURSE new_list ${root_dir}/*.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()


# Returns a list of distinct files that matches the CMake's matching_pattern pattern, but excluding the files that matches the excluding_pattern regexp pattern.
MACRO(FILES_LIST return_list root_dir matching_pattern excluding_pattern)
    FILE(GLOB_RECURSE new_list ${root_dir}/${matching_pattern})
    SET(temp_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        if( NOT (${file_path} MATCHES ${excluding_pattern}) )
            SET(temp_list ${temp_list} ${file_path})
        endif()
    ENDFOREACH()
    SET(${return_list} ${temp_list})
ENDMACRO()

function (print_error DESCRIPTION FILE)
  message (FATAL_ERROR
    "Can't find ${FILE}. ${DESCRIPTION}")
endfunction ()
