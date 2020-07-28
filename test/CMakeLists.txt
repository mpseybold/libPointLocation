project(test)
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

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

add_executable(Google_Tests_run 
  ../Segment.hpp 
  geom_tests.cpp
  ../Cut.cpp
  cut_tests.cpp
  ../BoundingTrap.cpp
  bounding_trap_tests.cpp
  ../TSD.cpp
  tsd_tests.cpp
  ../TSD_algorithms/v_partition.cpp
)

#target_link_libraries(Google_Tests_run main_lib)
target_link_libraries(Google_Tests_run gtest gtest_main ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})