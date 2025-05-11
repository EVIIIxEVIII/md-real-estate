cmake_minimum_required(VERSION 3.18)
project(real-estate-price-prediction LANGUAGES CXX CUDA)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CUDA_FLAGS_RELEASE "-O3 --use_fast_math -DNDEBUG")

find_package(Eigen3             REQUIRED)
find_package(fast-cpp-csv-parser REQUIRED)
find_package(raylib REQUIRED)
find_package(CUDAToolkit        REQUIRED)

file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE CU_FILES  src/*.cu)
file(GLOB_RECURSE HPP_FILES src/*.hpp)

add_executable(run
    ${CPP_FILES}
    ${CU_FILES}
    ${HPP_FILES}
)

target_link_libraries(run PRIVATE
    Eigen3::Eigen
    fast-cpp-csv-parser::fast-cpp-csv-parser
    raylib
    CUDA::cudart
)

set_target_properties(run PROPERTIES
    CXX_STANDARD 17
    CUDA_SEPARABLE_COMPILATION ON
)

