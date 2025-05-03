# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(fast-cpp-csv-parser_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(fast-cpp-csv-parser_FRAMEWORKS_FOUND_RELEASE "${fast-cpp-csv-parser_FRAMEWORKS_RELEASE}" "${fast-cpp-csv-parser_FRAMEWORK_DIRS_RELEASE}")

set(fast-cpp-csv-parser_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET fast-cpp-csv-parser_DEPS_TARGET)
    add_library(fast-cpp-csv-parser_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET fast-cpp-csv-parser_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${fast-cpp-csv-parser_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${fast-cpp-csv-parser_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### fast-cpp-csv-parser_DEPS_TARGET to all of them
conan_package_library_targets("${fast-cpp-csv-parser_LIBS_RELEASE}"    # libraries
                              "${fast-cpp-csv-parser_LIB_DIRS_RELEASE}" # package_libdir
                              "${fast-cpp-csv-parser_BIN_DIRS_RELEASE}" # package_bindir
                              "${fast-cpp-csv-parser_LIBRARY_TYPE_RELEASE}"
                              "${fast-cpp-csv-parser_IS_HOST_WINDOWS_RELEASE}"
                              fast-cpp-csv-parser_DEPS_TARGET
                              fast-cpp-csv-parser_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "fast-cpp-csv-parser"    # package_name
                              "${fast-cpp-csv-parser_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${fast-cpp-csv-parser_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Release ########################################
    set_property(TARGET fast-cpp-csv-parser::fast-cpp-csv-parser
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Release>:${fast-cpp-csv-parser_OBJECTS_RELEASE}>
                 $<$<CONFIG:Release>:${fast-cpp-csv-parser_LIBRARIES_TARGETS}>
                 )

    if("${fast-cpp-csv-parser_LIBS_RELEASE}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET fast-cpp-csv-parser::fast-cpp-csv-parser
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     fast-cpp-csv-parser_DEPS_TARGET)
    endif()

    set_property(TARGET fast-cpp-csv-parser::fast-cpp-csv-parser
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Release>:${fast-cpp-csv-parser_LINKER_FLAGS_RELEASE}>)
    set_property(TARGET fast-cpp-csv-parser::fast-cpp-csv-parser
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Release>:${fast-cpp-csv-parser_INCLUDE_DIRS_RELEASE}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET fast-cpp-csv-parser::fast-cpp-csv-parser
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Release>:${fast-cpp-csv-parser_LIB_DIRS_RELEASE}>)
    set_property(TARGET fast-cpp-csv-parser::fast-cpp-csv-parser
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Release>:${fast-cpp-csv-parser_COMPILE_DEFINITIONS_RELEASE}>)
    set_property(TARGET fast-cpp-csv-parser::fast-cpp-csv-parser
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Release>:${fast-cpp-csv-parser_COMPILE_OPTIONS_RELEASE}>)

########## For the modules (FindXXX)
set(fast-cpp-csv-parser_LIBRARIES_RELEASE fast-cpp-csv-parser::fast-cpp-csv-parser)
