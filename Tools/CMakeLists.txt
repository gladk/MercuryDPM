cmake_minimum_required(VERSION 2.8)

include(${CMAKE_SOURCE_DIR}/Configuration/MasterMake.cmake)

add_executable( data2pvd data2pvd.cpp MercuryData.cpp )
target_link_libraries( data2pvd MercuryBase )

