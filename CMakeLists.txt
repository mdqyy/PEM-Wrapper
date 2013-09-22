cmake_minimum_required (VERSION 2.8)

project (pem-wrapper)

####### Build binaries files #########
add_definitions (-std=c++0x)
add_definitions (-fopenmp)
add_definitions (-O3)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
else()
    message("OpenMP not found, you may want to use gnu compilers")
    return()
endif()

IF(DEFINED ENV{MKLROOT})
    message("...using MKL found in $ENV{MKLROOT}")
ELSE()
    message("MKLROOT is not defined. You must install MKL and source its configuration file to tell CMake where to find MKL")
    # exit early 
    return()
ENDIF()

find_package(OpenCV)
IF(OpenCV_FOUND)
    message("...using OpenCV")
    include_directories(${OpenCV_INCLUDE_DIR})
ELSE()
    message("OpenCV is not found. You must install OpenCV")
    # exit early 
    return()
ENDIF()

find_package(Boost COMPONENTS filesystem system program_options)
IF (Boost_FOUND)
    message("...using Boost")
    include_directories(${BOOST_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIRS})
    set (LD_FLAGS_BOOST ${Boost_LIBRARIES})
ELSE()
    message("BOOST is not found. You must install BOOST")
    # exit early 
    return()
ENDIF()

include_directories (
    ${CMAKE_SOURCE_DIR}/include
    $ENV{MKLROOT}/include 
    $ENV{OPENCVROOT}/include)

set (PEM_LIB ${CMAKE_SOURCE_DIR}/lib/linux/)
if (APPLE) 
    set (PEM_LIB ${CMAKE_SOURCE_DIR}/lib/mac/)
endif()
link_directories (
    ${PEM_LIB}
    $ENV{MKLROOT}/lib/intel64 
    $ENV{OPENCVROOT}/lib)
set (LD_FLAGS_MKL mkl_intel_lp64 mkl_intel_thread mkl_core iomp5)
set (LD_FLAGS_OPENCV opencv_core opencv_imgproc opencv_highgui opencv_ml opencv_video opencv_features2d opencv_calib3d opencv_objdetect opencv_contrib opencv_legacy opencv_flann)

# binaries
add_executable (wrapper_main main.cc)
target_link_libraries (wrapper_main mp_wrapper
    ${LD_FLAGS_MKL} 
    ${LD_FLAGS_OPENCV} 
    ${LD_FLAGS_BOOST})
