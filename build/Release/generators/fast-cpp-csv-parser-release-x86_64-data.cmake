########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(fast-cpp-csv-parser_COMPONENT_NAMES "")
if(DEFINED fast-cpp-csv-parser_FIND_DEPENDENCY_NAMES)
  list(APPEND fast-cpp-csv-parser_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES fast-cpp-csv-parser_FIND_DEPENDENCY_NAMES)
else()
  set(fast-cpp-csv-parser_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(fast-cpp-csv-parser_PACKAGE_FOLDER_RELEASE "/home/alderson/.conan2/p/fast-10f1bd0aed5fe/p")
set(fast-cpp-csv-parser_BUILD_MODULES_PATHS_RELEASE )


set(fast-cpp-csv-parser_INCLUDE_DIRS_RELEASE "${fast-cpp-csv-parser_PACKAGE_FOLDER_RELEASE}/include"
			"${fast-cpp-csv-parser_PACKAGE_FOLDER_RELEASE}/include/fast-cpp-csv-parser")
set(fast-cpp-csv-parser_RES_DIRS_RELEASE )
set(fast-cpp-csv-parser_DEFINITIONS_RELEASE )
set(fast-cpp-csv-parser_SHARED_LINK_FLAGS_RELEASE )
set(fast-cpp-csv-parser_EXE_LINK_FLAGS_RELEASE )
set(fast-cpp-csv-parser_OBJECTS_RELEASE )
set(fast-cpp-csv-parser_COMPILE_DEFINITIONS_RELEASE )
set(fast-cpp-csv-parser_COMPILE_OPTIONS_C_RELEASE )
set(fast-cpp-csv-parser_COMPILE_OPTIONS_CXX_RELEASE )
set(fast-cpp-csv-parser_LIB_DIRS_RELEASE )
set(fast-cpp-csv-parser_BIN_DIRS_RELEASE )
set(fast-cpp-csv-parser_LIBRARY_TYPE_RELEASE UNKNOWN)
set(fast-cpp-csv-parser_IS_HOST_WINDOWS_RELEASE 0)
set(fast-cpp-csv-parser_LIBS_RELEASE )
set(fast-cpp-csv-parser_SYSTEM_LIBS_RELEASE pthread)
set(fast-cpp-csv-parser_FRAMEWORK_DIRS_RELEASE )
set(fast-cpp-csv-parser_FRAMEWORKS_RELEASE )
set(fast-cpp-csv-parser_BUILD_DIRS_RELEASE )
set(fast-cpp-csv-parser_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(fast-cpp-csv-parser_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${fast-cpp-csv-parser_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${fast-cpp-csv-parser_COMPILE_OPTIONS_C_RELEASE}>")
set(fast-cpp-csv-parser_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${fast-cpp-csv-parser_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${fast-cpp-csv-parser_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${fast-cpp-csv-parser_EXE_LINK_FLAGS_RELEASE}>")


set(fast-cpp-csv-parser_COMPONENTS_RELEASE )