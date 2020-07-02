# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

project( main )


cmake_minimum_required(VERSION 2.8.11)

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

find_library( MPFR libmpfr.a )
find_library( GMP libgmp.a )
find_library( CGAL libCGAL.a )


# Creating entries for target: main
# ############################

add_executable( main  main.cpp geom.cpp)
message("${CMAKE_CXX_FLAGS}")
message("${MPFR} ${GMP}")
message("${CGAL}")
# Link the executable to CGAL and third-party libraries
target_link_libraries(main  ${MPFR} ${GMP} ${CGAL} )