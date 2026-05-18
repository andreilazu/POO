include(cmake/CustomStdlibAndSanitizers.cmake)

function(set_compiler_flags)
    set(multiValueArgs TARGET_NAMES)
    set(oneValueArgs RUN_SANITIZERS)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "" "${oneValueArgs}" "${multiValueArgs}")

    if(NOT DEFINED ARG_RUN_SANITIZERS)
        set(ARG_RUN_SANITIZERS TRUE)
    endif()

    foreach(TARGET_NAME IN LISTS ARG_TARGET_NAMES)
        if(GITHUB_ACTIONS)
            target_compile_definitions(${TARGET_NAME} PRIVATE GITHUB_ACTIONS)
        endif()

        if(PROJECT_WARNINGS_AS_ERRORS)
            set_property(TARGET ${TARGET_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
        endif()

        if(MSVC)
            target_compile_options(${TARGET_NAME} PRIVATE /W4 /permissive- /wd4244 /wd4267 /wd4996 /external:anglebrackets /external:W0 /utf-8 /MP)
        else()
            target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic)
        endif()

        if("${ARG_RUN_SANITIZERS}" STREQUAL "TRUE")
            set_custom_stdlib_and_sanitizers(${TARGET_NAME} true)
        endif()
    endforeach()
endfunction()
