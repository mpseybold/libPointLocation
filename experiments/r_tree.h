#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/algorithms/detail/intersects/interface.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

using point_t = bg::model::point<double, 2, bg::cs::cartesian>;
using segment_t = bg::model::segment<point_t>;

void r_tree_exp() {

    bgi::rtree<segment_t, bgi::quadratic<16>> rtree;

    segment_t seg{{0.0, 1.0}, {5.0, 6.0}};

    segment_t query_seg{{50, 0}, {50, 100000}};
    std::vector<segment_t> results;

    std::vector<segment_t> segments = std::vector<segment_t>();

    for (int i = 0; i < 10000; ++i) {
        // segments.push_back({{i/1000, i}, {i/1000 + 100, i}});
        rtree.insert({{i/1000.0, i}, {i/1000.0 + 100.0, i}});
    }

    // for (auto seg: segments) {
    //     std::cout << bg::get<0, 0>(seg) << " " << bg::get<0, 1>(seg) << " " 
    //     << bg::get<1, 0>(seg) << " " << bg::get<1, 1>(seg) << std::endl;
    // }

    rtree.insert(seg);
    rtree.insert(query_seg);
    rtree.insert(query_seg);
    // boost::detail::variant::node_visits = 0;
    int before_q_count = boost::geometry::predicate_count;
    // boost::geometry::detail::intersects::predicate_count = 0;
    rtree.query(bgi::intersects(query_seg), std::back_inserter(results));
    int after_q_count = boost::geometry::predicate_count;
    rtree.query(bgi::intersects(seg), std::back_inserter(results));

    // for (auto seg: results) {
    //     std::cout << bg::get<0, 0>(seg) << " " << bg::get<0, 1>(seg) << " " 
    //     << bg::get<1, 0>(seg) << " " << bg::get<1, 1>(seg) << std::endl;
    // }

    std::cout << "k: " << results.size() << std::endl;
    std::cout << "predicate_count: " << after_q_count - before_q_count << std::endl;
}