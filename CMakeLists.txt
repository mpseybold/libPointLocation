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


add_subdirectory(test)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
# SET(RELEASE_OPTIONS "-pg")
# add_compile_options("$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")

# Creating entries for target: main
# ############################

add_executable( main  
  main.cpp
  Point_2D.cpp 
  Cut.cpp
  BoundingTrap.cpp
)

add_to_cached_list( CGAL_EXECUTABLE_TARGETS main )

# Link the executable to CGAL and third-party libraries
target_link_libraries(main   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )

target_compile_options(main PRIVATE -pg)