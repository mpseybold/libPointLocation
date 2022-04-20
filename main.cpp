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
// #include "seg_tree_example.h"

#include "experiments/vertical_query_exp.h"
#include "experiments/vertical_query_exp_2.h"
#include "experiments/r_tree.h"
#include "experiments/experiments.h"
// #include "data/data_generators.h"

#include "experiments/query_segs.h"

typedef CGAL::Arr_segment_traits_2<Kernel>            Traits_2;
typedef Traits_2::Point_2                             Point_2;
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

    std::vector<std::string> osm_filenames = {
        "albania", "bosnia-herzegovina", "bulgaria",
        "croatia", "kosovo", "macedonia", "montenegro", "serbia"
    };

    for (auto& name: osm_filenames) {
        query_seg_utility::get_query_segs(name+"_clean");
    }
    // std::vector<std::vector<long double>> segments = io::read_segments("data/random_horizontal/128.txt");

    // std::vector<std::vector<double>> d_segs = {
    //     {904196, 381526, 8079083, 381526},
    //     {1276652, 925986, 10596571, 925986},
    //     {1937328, 1217414, 2470665, 1217414},
    //     {1090776, 697544, 3491291, 697544},
    //     {1705212, 53244, 4405210, 53244},
    //     {1679604, 217836, 1834752, 217836},
    //     {1585494, 258364, 7495498, 258364},
    //     {476800, 1303412, 3603755, 1303412},
    //     {298502, 197976, 9461825, 197976},
    //     {376998, 1230944, 9877653, 1230944},
    //     {1658386, 909446, 6469740, 909446},
    //     {945772, 1640382, 6803032, 1640382},
    //     {276808, 1093866, 1419912, 1093866},
    //     {1385532, 1152920, 2192230, 1152920},
    //     {1954480, 328908, 9638701, 328908},
    //     {1719976, 1017880, 8209364, 1017880}
    // };

    // std::vector<std::vector<long double>> l_segs;

    // for (auto seg: d_segs) {
    //     l_segs.push_back({
    //         (long double)seg[0], (long double)seg[1], (long double)seg[2], (long double)seg[3]
    //     });
    // }

    // std::vector<double> qs = {10228342, 930850, 10228342, 536787};

    // R_Tree r_tree = experiments::build_r_tree(l_segs);

    // auto stats = experiments::rtree_query(r_tree, qs);



    // TSD<PointCart, int> tsd;

    // experiments::build_tsd(l_segs, 1000, tsd);

    // Segment<PointCart, int> query = Segment<PointCart, int>(
    //     PointCart(10228342, 930850), PointCart(10228342, 536787),
    //     1000000000
    // );

    // auto p1d = PERSIST1D::Persist1D(d_segs);

    // std::cout << "k: " << p1d.v_query(536787, 930850, 10228342) << std::endl;

    // tsd.affected_subdag_roots(&query, true);

    // std::cout << "tsd_k: " << tsd.get_subdag_roots().size() - 1 << std::endl;


    // auto segs = io::read_segments("data/osm/nc_clean.txt");

    // auto tsd = TSD<PointCart, int>();

    // experiments::build_rs_tsd(segs, 10, tsd);

    // auto segments = io::read_segments("data/random_horizontal/128.txt");
    // TSD<PointCart, int> tsd = TSD<PointCart, int>();
    // std::vector<double> query_seg = {
    //     700000, 600000, 700000, 400000
    // };


    // for (int i = 0; i < segments.size(); ++i) {
    //     auto seg = segments[i];
    //     Segment<PointCart, int>* s =  new Segment<PointCart, int>(
    //         PointCart(seg[0], seg[1]), PointCart(seg[2], seg[3]), i
    //     );

    //     tsd.insert_segment(*s, false);
    // }

    // auto query = new Segment<PointCart, int>(
    //     PointCart(700000, 600000), PointCart(700000, 400000),
    //     10000001
    // );

    // RS_TSD rs_tsd = RS_TSD(segments, query);


    // auto root = tsd.get_root();
    // auto s = root->get_destruction_cuts().get_e();
    // auto x = s->get_segment()->get_source().X_();
    // auto priority = root->get_priority();

    // auto tsd_stats = experiments::tsd_query(tsd, query_seg);
    // auto rs_tsd_stats = experiments::rs_tsd_query(rs_tsd, query_seg);

    // int i = 0;      
//     auto start_time = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < new_calidonia.size(); ++i) {
//         std::cout << i << "\n";
//         if (i == 188049)
//             std::cout << "what's going on here...\n";
//         // auto segment = new_calidonia[i];
//         // std::cout << segment.get_source().x() << " " << segment.get_source().y() 
//         // << " " << segment.get_target().x() << " " << segment.get_target().y() << "\n";
//         tsd.insert_segment(new_calidonia[i]);
//         std::cout << "visits: " << tsd.get_node_visits() << std::endl;
//     }
//     for (int i = 5000; i > -1; --i) {
//         // std::cout << i << "\n";
//         // auto segment = new_calidonia[i];
//         // std::cout << segment.get_source().x() << " " << segment.get_source().y() 
//         // << " " << segment.get_target().x() << " " << segment.get_target().y() << "\n";
//         // tsd.delete_segment(new_calidonia[i]);
//     }
//     auto end_time = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
//     std::cout << "duration: " + std::to_string(duration) + "\n";
//     std::cout << tsd.get_l eaf_count() << std::endl;
//     // int who = RUSAGE_CHILDREN;
//     // struct rusage usage;
//     // int ret;
//     // ret = getrusage(who, &usage);
//     // std::cout << usage.ru_idrss << "\n";
//     auto traps = std::vector<BoundingTrap<PointCart, int>>();
//     write_leaf_traps(tsd.get_root(), traps);
//     std::cout << traps.size() << std::endl;
//     io::write_trapezoids(traps, "plotting/new_calidonia.dat");

}
