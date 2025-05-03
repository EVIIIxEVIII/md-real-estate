# Load the debug and release variables
file(GLOB DATA_FILES "${CMAKE_CURRENT_LIST_DIR}/fast-cpp-csv-parser-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${fast-cpp-csv-parser_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${fast-cpp-csv-parser_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET fast-cpp-csv-parser::fast-cpp-csv-parser)
    add_library(fast-cpp-csv-parser::fast-cpp-csv-parser INTERFACE IMPORTED)
    message(${fast-cpp-csv-parser_MESSAGE_MODE} "Conan: Target declared 'fast-cpp-csv-parser::fast-cpp-csv-parser'")
endif()
# Load the debug and release library finders
file(GLOB CONFIG_FILES "${CMAKE_CURRENT_LIST_DIR}/fast-cpp-csv-parser-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()