cmake_minimum_required(VERSION 3.18)
project(my_project LANGUAGES CXX CUDA)

find_package(fmt                REQUIRED)
find_package(Eigen3             REQUIRED)
find_package(fast-cpp-csv-parser REQUIRED)

find_package(CUDAToolkit        REQUIRED)

file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE CU_FILES  src/*.cu)
file(GLOB_RECURSE HPP_FILES src/*.hpp)

add_executable(my_project
    ${CPP_FILES}
    ${CU_FILES}
    ${HPP_FILES}
)

target_link_libraries(my_project PRIVATE
    fmt::fmt
    Eigen3::Eigen
    fast-cpp-csv-parser::fast-cpp-csv-parser
    CUDA::cudart
)

set_target_properties(my_project PROPERTIES
    CXX_STANDARD 17
    CUDA_SEPARABLE_COMPILATION ON
)

