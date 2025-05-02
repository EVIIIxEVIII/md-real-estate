########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(fast-cpp-csv-parser_FIND_QUIETLY)
    set(fast-cpp-csv-parser_MESSAGE_MODE VERBOSE)
else()
    set(fast-cpp-csv-parser_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/fast-cpp-csv-parserTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${fast-cpp-csv-parser_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(fast-cpp-csv-parser_VERSION_STRING "20191004")
set(fast-cpp-csv-parser_INCLUDE_DIRS ${fast-cpp-csv-parser_INCLUDE_DIRS_DEBUG} )
set(fast-cpp-csv-parser_INCLUDE_DIR ${fast-cpp-csv-parser_INCLUDE_DIRS_DEBUG} )
set(fast-cpp-csv-parser_LIBRARIES ${fast-cpp-csv-parser_LIBRARIES_DEBUG} )
set(fast-cpp-csv-parser_DEFINITIONS ${fast-cpp-csv-parser_DEFINITIONS_DEBUG} )


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${fast-cpp-csv-parser_BUILD_MODULES_PATHS_DEBUG} )
    message(${fast-cpp-csv-parser_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


