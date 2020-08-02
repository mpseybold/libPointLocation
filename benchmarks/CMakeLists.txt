# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

project( benchmarks )
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CGAL_DIR "/home/martin/Downloads/CGAL-5.0.2")
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE "TRUE")


# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

# include for local directory

# include for local package



# Creating entries for target: main
# ############################

add_executable( main  
	cgal-2d-arr-intersect-batch.cpp
)

set_property(TARGET main PROPERTY CXX_STANDARD 17)

add_to_cached_list( CGAL_EXECUTABLE_TARGETS main )

# Link the executable to CGAL and third-party libraries
target_link_libraries(main   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
