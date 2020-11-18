#include <iostream>
#include <CGAL/Point_2.h>
// #include <CGAL/Exact_predicates_exact_constructions_kernel.h>
// #include <CGAL/Line_2.h>
#include "V_Cut.h"
// #include "Point_2D.h"
#include "Node.h"
#include "io.h"
#include "TSD.h"
#include <random>
#include <chrono>
#include <sys/resource.h>
#include <sys/time.h>

#include <CGAL/Cartesian.h>
#include <CGAL/Quotient.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <list>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
// #include "Point_S2ratss.hpp"

typedef CGAL::Arr_segment_traits_2<Kernel>            Traits_2;
typedef Traits_2::Point_2                             Point_2;
typedef Traits_2::X_monotone_curve_2                  Segment_2;
typedef CGAL::Arrangement_2<Traits_2>                 Arrangement_2;
typedef CGAL::Arr_trapezoid_ric_point_location<Arrangement_2> Ric;


void write_leaf_traps(Node<PointCart, int>* node, std::vector<BoundingTrap<PointCart, int>>& traps) {
    if (node->is_leaf()) {
        traps.push_back(node->get_trapezoid());
    } else {
        if (node->get_L() != nullptr) {
            write_leaf_traps(node->get_L(), traps);
        }

        if (node->get_R() != nullptr) {
            write_leaf_traps(node->get_R(), traps);
        }

        if (node->get_A() != nullptr) {
            write_leaf_traps(node->get_A(), traps);
        }

        if (node->get_B() != nullptr) {
            write_leaf_traps(node->get_B(), traps);
        }
    }
}


int main() {
    
;
    
    auto new_calidonia = io::read_segments<PointCart, int>("new-caledonia-200601.plot.lonlat");
    
    TSD<PointCart, int> tsd = TSD<PointCart, int>();

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < new_calidonia.size(); ++i) {
        std::cout << i << "\n";
        // auto segment = new_calidonia[i];
        // std::cout << segment.get_source().x() << " " << segment.get_source().y() 
        // << " " << segment.get_target().x() << " " << segment.get_target().y() << "\n";
        tsd.insert_segment(new_calidonia[i]);
    }
    for (int i = new_calidonia.size() - 1; i > -1; --i) {
        std::cout << i << "\n";
        // auto segment = new_calidonia[i];
        // std::cout << segment.get_source().x() << " " << segment.get_source().y() 
        // << " " << segment.get_target().x() << " " << segment.get_target().y() << "\n";
        tsd.delete_segment(new_calidonia[i]);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "duration: " + std::to_string(duration) + "\n";
    std::cout << tsd.get_leaf_count() << std::endl;
    // int who = RUSAGE_CHILDREN;
    // struct rusage usage;
    // int ret;
    // ret = getrusage(who, &usage);
    // std::cout << usage.ru_idrss << "\n";
    auto traps = std::vector<BoundingTrap<PointCart, int>>();
    write_leaf_traps(tsd.get_root(), traps);
    std::cout << traps.size() << std::endl;
    io::write_trapezoids(traps, "plotting/new_calidonia.dat");

    // std::vector<BoundingTrap<PointCart, int>> traps; 
    // write_leaf_traps(tsd.get_root(), traps);

    // io::write_trapezoids(traps, "plotting/new_calidionia,dat");
    // Construct the arrangement of five intersecting segments.
    // Arrangement_2 arr;
    // std::list<Segment_2> segments;

    // std::mt19937 generator (1234);
    // std::uniform_real_distribution<double> dis(0.0, 1.0);


    // std::cout << "start...\n";
    // for (int i = 1; i <= 100; ++i) {

    //     std::cout << i << std::endl;
        
    //     Point_2 s = Point_2(std::floor((double)10* dis(generator)) * 10, std::floor((double)10 * dis(generator)) * 10);
    //     Point_2 t = Point_2(std::floor((double)10* dis(generator)) * 10, std::floor((double)10 * dis(generator)) * 10);

    //     while (s.x() == t.x() && s.y() == t.y())
    //         t = Point_2(std::floor((double)10* dis(generator)) * 10, std::floor((double)10 * dis(generator)) * 10);

    //     segments.push_back(Segment_2(s, t));
    // }

    // insert(arr, segments.begin(), segments.end());
    // // Print the size of the arrangement.
    // std::cout << "The arrangement size:" << std::endl
    //         << "   V = " << arr.number_of_vertices()
    //         << ",  E = " << arr.number_of_edges()
    //         << ",  F = " << arr.number_of_faces() << std::endl;

    // Ric ric;
    // std::cout << "start" << std::endl;
    // ric.attach(arr);
    // std::cout << "end" << std::endl;
    // Point_2 q = Point_2(0, 1.1);
    // auto feature = ric.ray_shoot_up(q);
    // return 0;
}
