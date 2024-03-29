// Constructing an arrangements of intersecting line segments using the
// predefined kernel with exact constructions and exact predicates.
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Timer.h>
#include <list>
#include <fstream>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel  Kernel;
typedef Kernel::FT                                         Number_type;
typedef CGAL::Arr_segment_traits_2<Kernel>                 Traits_2;
typedef Traits_2::Point_2                                  Point_2;
typedef Traits_2::X_monotone_curve_2                       Segment_2;
typedef CGAL::Arrangement_2<Traits_2>                      Arrangement_2;
typedef CGAL::Arr_walk_along_line_point_location<Arrangement_2> Walk_pl;
typedef CGAL::Arr_naive_point_location<Arrangement_2>           Naive_pl;
typedef CGAL::Arr_landmarks_point_location<Arrangement_2>       Landmarks_pl;
typedef CGAL::Arr_trapezoid_ric_point_location<Arrangement_2>   Trap_pl;

int main (int argc, char *argv[])
{
  // Get the name of the input file from the command line, or use the default
  // fan_grids.dat file if no command-line parameters are given.
  const char * filename = (argc > 1) ? argv[1] : "../data/new-caledonia-200601.plot.lonlat";
  // Open the input file.
  std::ifstream     in_file (filename);
  if (! in_file.is_open()) {
    std::cerr << "Failed to open " << filename << " ..." << std::endl;
    return (1);
  }
  // Read the segments from the file.
  // The input file format should be (all coordinate values are integers):
  // <n>                                 // number of segments.
  // <sx_1> <sy_1>  <tx_1> <ty_1>        // source and target of segment #1.
  // <sx_2> <sy_2>  <tx_2> <ty_2>        // source and target of segment #2.
  //   :      :       :      :
  // <sx_n> <sy_n>  <tx_n> <ty_n>        // source and target of segment #n.
  
  std::list<Segment_2>  segments;
  unsigned int n;
  in_file >> n;
  unsigned int i;
  for (i = 0; i < n; ++i) {
    double sx, sy, tx, ty;
    in_file >> sx >> sy >> tx >> ty;
    segments.push_back (Segment_2 (Point_2 (Number_type(sx), Number_type(sy)),
                                   Point_2 (Number_type(tx), Number_type(ty))));
  }
  in_file.close();
  // Construct the arrangement by aggregately inserting all segments.
  Arrangement_2                  arr;
  Walk_pl                        pl(arr);
  Trap_pl          trap_pl;
  CGAL::Timer                    timer;

  std::cout << "Performing aggregated insertion of " 
            << n << " segments." << std::endl;
  timer.start();
  insert (arr, segments.begin(), segments.end());
  trap_pl.attach (arr);
  timer.stop();

  // std::cout << "Performing incremental insertion of " 
  //           << n << " segments." << std::endl;
  // timer.start();
  // for( auto& s : segments )
  //   insert (arr, s, pl );
  // timer.stop();

  // Print the arrangement dimensions.
  std::cout << "V = " << arr.number_of_vertices()
        << ",  E = " << arr.number_of_edges() 
        << ",  F = " << arr.number_of_faces() << std::endl;
  std::cout << "Construction took " << timer.time() 
        << " seconds." << std::endl;
  
  return 0;
}
