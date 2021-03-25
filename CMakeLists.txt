
cmake_minimum_required(VERSION 2.8)

project(FourierImage)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
endif()

add_executable(MedianFilter MedianFilter.cxx)
target_link_libraries(MedianFilter ${ITK_LIBRARIES})

add_executable(AdaptativeMedianFilter AdaptativeMedianFilter.cxx)
target_link_libraries(AdaptativeMedianFilter ${ITK_LIBRARIES})
