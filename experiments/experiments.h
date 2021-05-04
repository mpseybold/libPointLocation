#pragma once

#include "../TSD.h"
#include "../Point_2D.h"
#include "../segment_ds/SegmentDS.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/algorithms/detail/intersects/interface.hpp>
#include "../io.h"
#include <unordered_set>
#include <chrono>
// #include "query_segs.h"


namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

using point_t = bg::model::point<double, 2, bg::cs::cartesian>;
using segment_t = bg::model::segment<point_t>;
using R_Tree = bgi::rtree<segment_t, bgi::quadratic<3>>;
using R_Tree_Linear = bgi::rtree<segment_t, bgi::linear<3>>;
using R_Tree_Star = bgi::rtree<segment_t, bgi::rstar<3>>;


namespace experiments {

    bool comp(Segment<PointCart, int>* s1, Segment<PointCart, int>* s2) {
        return s1->get_priority() < s2->get_priority();
    }

    struct QueryStatistics {
        int predicate_count = 0;
        int structure_size = 0;
        int output_size = 0;
    };

    // enum DS { L_TREE, R_TREE, TSD };

    R_Tree build_r_tree(std::vector<std::vector<long double>>& segments) {
        bgi::rtree<segment_t, bgi::quadratic<3>> rtree;

        for (int i = 0; i < segments.size(); ++i) {
            auto s = segments[i];
            rtree.insert({{s[0], s[1]}, {s[2], s[3]}});
        }

        return rtree;
    }

    R_Tree_Linear build_r_tree_linear(std::vector<std::vector<long double>>& segments) {
        bgi::rtree<segment_t, bgi::linear<3>> rtree;

        for (int i = 0; i < segments.size(); ++i) {
            auto s = segments[i];
            rtree.insert({{s[0], s[1]}, {s[2], s[3]}});
        }

        return rtree;
    }

    R_Tree_Star build_r_tree_star(std::vector<std::vector<long double>>& segments) {
        bgi::rtree<segment_t, bgi::rstar<3>> rtree;

        for (int i = 0; i < segments.size(); ++i) {
            auto s = segments[i];
            rtree.insert({{s[0], s[1]}, {s[2], s[3]}});
        }

        return rtree;
    }

    SegmentDS::SegmentDS build_layered_tree(std::vector<std::vector<long double>>& segments) {
        auto _segments = std::vector<SegmentDS::Segment*>();

        for (auto s: segments) {
            auto _s = new SegmentDS::Segment(s[0], s[1], s[2], s[3]);
            _segments.push_back(_s);
        }

        auto seg_ds = SegmentDS::SegmentDS(_segments);

        return seg_ds;
    }

    void build_tsd(std::vector<std::vector<long double>>& segments, int seed, TSD<PointCart, int>& tsd) {
        auto _segments = std::vector<Segment<PointCart, int>*>();

        std::cout << segments.size() << std::endl;

        auto priorities = std::vector<int>();

        for (int i = 0; i < segments.size(); ++i)
            priorities.push_back(i);

        std::uniform_real_distribution<double> dis(0.0, 1.0);
        std::mt19937 generator (seed);

        std::shuffle(priorities.begin(), priorities.end(), generator);

        for (int i = 0; i < segments.size(); ++i) {
            // std::cout << i << std::endl;
            auto s = segments[i];
            _segments.push_back(
                new Segment<PointCart, int>(
                    PointCart(s[0], s[1]), PointCart(s[2], s[3]), 
                    priorities[i]
                )
            );
        }

        std::sort(_segments.begin(), _segments.end(), &comp);

        // auto tsd = TSD<PointCart, int>();

        double average_duration = 0;
        std::cout <<  std::setprecision(10);
        std::cout << segments[6175][0] << " " << segments[6175][1] << " " << segments[6175][2] << " " << segments[6175][3] << std::endl;
        for (int i = 0; i < _segments.size(); ++i) {

            // if (i == 6175)
            //     continue;
            auto s = _segments[i];
            // std::cout << (double)TSD<PointCart, int>::intersection_calls / tsd.get_node_visits() << std::endl;
            
            // if (tsd.get_node_visits() > TSD<PointCart, int>::total_search_visits / (i+1))
                // std::cout << i << std::endl;
            
            auto start_time = std::chrono::high_resolution_clock::now();
            tsd.insert_segment(*s);
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            std::cout << i << std::endl;
            // std::cout << segments[i][0] << " " << segments[i][1] << " " << segments[i][2] << " " << segments[i][3] << std::endl; 
            if (i == 0)
                average_duration = duration;
            else 
                average_duration = (i * average_duration + duration) / (i+1);
            
            // if (i % 100 == 0) {
            //     std::cout << i << std::endl;
            //     std::cout << (double)tsd.get_node_visits() << std::endl;
            //     std::cout << (double)tsd.get_node_visits() / (TSD<PointCart, int>::total_search_visits / (i+1)) << std::endl;
            //     std::cout << "predicate/visit ratio: " << TSD<PointCart, int>::intersection_calls / (double)tsd.get_node_visits() << std::endl;
            //     std::cout << duration << std::endl;
            //     std::cout << average_duration << std::endl;
            //     std::cout << "k: " << tsd.get_subdag_roots().size() << std::endl;
            // }
        }

        std::cout << "finished inserting segments...\n";   

        // return tsd;
    }

    QueryStatistics rtree_query(R_Tree& r_tree, std::vector<double> query_seg) {
        auto stats = QueryStatistics();
        std::vector<segment_t> results;
        segment_t _query_seg{{query_seg[0], query_seg[1]}, {query_seg[2], query_seg[3]}};
        r_tree.query(bgi::intersects(_query_seg), std::back_inserter(results));
        stats.output_size = results.size();
        stats.predicate_count = boost::geometry::predicate_count;

        return stats;
    }

    QueryStatistics rtree_linear_query(R_Tree_Linear& r_tree, std::vector<double> query_seg) {
        auto stats = QueryStatistics();
        std::vector<segment_t> results;
        segment_t _query_seg{{query_seg[0], query_seg[1]}, {query_seg[2], query_seg[3]}};
        r_tree.query(bgi::intersects(_query_seg), std::back_inserter(results));
        stats.output_size = results.size();
        stats.predicate_count = boost::geometry::predicate_count;

        return stats;
    }

    QueryStatistics rtree_star_query(R_Tree_Star& r_tree, std::vector<double> query_seg) {
        auto stats = QueryStatistics();
        std::vector<segment_t> results;
        segment_t _query_seg{{query_seg[0], query_seg[1]}, {query_seg[2], query_seg[3]}};
        r_tree.query(bgi::intersects(_query_seg), std::back_inserter(results));
        stats.output_size = results.size();
        stats.predicate_count = boost::geometry::predicate_count;

        return stats;
    }

    
    void rotate_segs(std::vector<std::vector<double>>& segs) {

        double pi = 3.1415926535897932384626433;

        for (int i = 0; i < segs.size(); ++i) {
            double x_1 = cos(pi/4)*segs[i][0] - sin(pi/4)*segs[i][1];
            double y_1 = sin(pi/4)*segs[i][0] + cos(pi/4)*segs[i][1];  
            double x_2 = cos(pi/4)*segs[i][2] - sin(pi/4)*segs[i][3];
            double y_2 = sin(pi/4)*segs[i][2] + cos(pi/4)*segs[i][3];
        
            segs[i] = {x_1, y_1, x_2, y_2};
        }
    }


    QueryStatistics layered_tree_query(SegmentDS::SegmentDS& segment_ds, std::vector<double> query_seg) {
        auto stats = QueryStatistics();

        auto _query_seg = SegmentDS::Segment(query_seg[0], query_seg[1], query_seg[2], query_seg[3]);
        auto output = std::vector<SegmentDS::Segment>();
        SegmentDS::node_visits = 0;
        segment_ds.vertical_query(_query_seg, output);

        stats.output_size = output.size();
        stats.predicate_count = SegmentDS::node_visits;

        return stats;
    }

    QueryStatistics tsd_query(TSD<PointCart, int>& tsd, std::vector<double> query_seg) {
        
        auto stats = QueryStatistics();
        auto _query_seg = Segment<PointCart, int>(
            PointCart(query_seg[0], query_seg[1]), PointCart(query_seg[2], query_seg[3]), 10000000
        );

        tsd.affected_subdag_roots(&_query_seg, false);
        // stats.predicate_count = tsd.get_node_visits();
        stats.predicate_count = TSD<PointCart, int>::intersection_calls;
        stats.output_size = tsd.get_subdag_roots().size() - 1;

        return stats;
    }


    std::vector<std::vector<double>> 
    query_segs_for_osm() {
        auto segs = io::read_segments("data/query_segments/nc_q.txt");

        auto output = std::vector<std::vector<double>>();

        for (int i = 0; i < 300; ++i) {
            output.push_back({segs[i][0], segs[i][1], segs[i][2], segs[i][3]});
        }

        return output;
    }

    std::vector<std::vector<double>> 
    query_segs_for_random_horizontal(
        std::vector<std::vector<long double>> segments,
        int num_of_queries
    ) {

        int seed = 1;

        std::uniform_real_distribution<double> dis(0.0, 1.0);
        std::mt19937 generator (seed);

        auto output = std::vector<std::vector<double>>();

        auto x_coords = std::unordered_set<double>();
        auto y_coords = std::unordered_set<double>();

        for (auto s: segments) {
            x_coords.insert(s[0]);
            x_coords.insert(s[1]);
            y_coords.insert(s[2]);
            y_coords.insert(s[3]);
        }

        for (int i = 0; i < num_of_queries; ++i) {
            // double x = 4000000 + std::floor((double)1000000 * dis(generator));
            double x = 5000000;
            double y1 = std::floor((double)(9999999 - .5*log(segments.size())*(9999999 / segments.size())*log(segments.size())) * dis(generator));
            double y2 = y1 +  .5*log(segments.size())*(9999999 / segments.size())*log(segments.size());
            
            while (x_coords.find(x) != x_coords.end()) {
                x = std::floor((double)9999999 * dis(generator));
            }

            while (y_coords.find(y1) != y_coords.end()) {
                y1 = std::floor((double)(9999999 - .5*log(segments.size())*(9999999 / segments.size())*log(segments.size())) * dis(generator));
            }

            while (y_coords.find(y2) != y_coords.end() && y2 > y1) {
                y2 = y1 + .5*log(segments.size())*(9999999 / segments.size())*log(segments.size());
            }
            
            output.push_back({x, y1, x, y2});
        }

        return output;
    }

    std::vector<std::vector<double>> 
    query_segs_for_random_slanted(
        std::vector<std::vector<long double>> segments,
        int num_of_queries
    ) {
        auto segs = query_segs_for_random_horizontal(segments, num_of_queries);

        double height = segs[0][3] - segs[0][1];
        rotate_segs(segs);

        for (int i = 0; i < segs.size(); ++i) {
            segs[i][2] = segs[i][0];
            segs[i][3] = segs[i][1] + 2*height;
        }

        return segs;
    }

    std::vector<std::vector<double>>
    query_segs_for_seg_tree_worst_case(int size, int query_count) {
        auto output = std::vector<std::vector<double>>();

        int seed = 1;

        std::uniform_real_distribution<double> dis(0.0, 1.0);
        std::mt19937 generator (seed);

        for (int i = 0; i < query_count; ++i) {
            double x = log(size) + std::floor((double)(size - log(size)) * dis(generator)) - 0.1;
            double y = dis(generator) * (size -x) - log(size);
            output.push_back({x, y, x, y+log(size)});
        }


        return output;
    }

    void run_synthetic(int size, std::string instance_type, int num_of_queries, std::string data_structure) {
        auto segments = instance_type != "osm" ? io::read_segments("data/"+instance_type+"/" + std::to_string(size) + ".txt")
        : io::read_segments("data/osm/nc_clean.txt");

        std::vector<std::vector<double>> query_segs;
        std::cout << "fetching query segs...\n";
        if (instance_type == "random_horizontal")
            query_segs = query_segs_for_random_horizontal(segments, num_of_queries);
        if (instance_type == "random_slanted")
            query_segs = query_segs_for_random_slanted(segments, num_of_queries);
        if (instance_type == "seg_tree_worst_case")
            query_segs = query_segs_for_seg_tree_worst_case(size, num_of_queries);
        if (instance_type == "osm")
            query_segs = query_segs_for_osm();


        std::cout << "fetched!\n";

        R_Tree r_tree;
        R_Tree_Linear r_tree_linear;
        R_Tree_Star r_tree_star;

        std::vector<SegmentDS::Segment*> seg_ds_segs{new SegmentDS::Segment(0, 0, 1, 1), new SegmentDS::Segment(0, 0, 2, 1)};
        SegmentDS::SegmentDS seg_ds = SegmentDS::SegmentDS(seg_ds_segs);
        TSD<PointCart, int> tsd = TSD<PointCart, int>();


        // std::cout << "building tsd...\n";
        if (data_structure == "tsd")
            build_tsd(segments, 10, tsd);
        // std::cout << "building r_tree...\n";
        if (data_structure == "r_tree") {
            r_tree = build_r_tree(segments);
            auto alloc = r_tree.get_allocator();
            std::cout << "building r_tree...\n";
        }

        if (data_structure == "r_tree_linear")
            r_tree_linear = build_r_tree_linear(segments);

        if (data_structure == "r_tree_star")
            r_tree_star = build_r_tree_star(segments);
        // std::cout << "building seg_ds...\n";

        if (data_structure == "seg_ds")    
            seg_ds = build_layered_tree(segments);

        std::cout << "built data structures...\n";

        std::string res_string = instance_type != "osm" ? "data/results/" + instance_type + "/" + std::to_string(size) + data_structure + ".csv"
        : "data/results/" + instance_type + "/new_calidonia" + data_structure +".csv";

        std::ofstream results(res_string);

        results << "k,";

        if (data_structure == "tsd")
            results << "tsd\n";

        if (data_structure == "seg_ds")
            results << "seg_ds\n";

        if (data_structure == "r_tree")
            results << "r_tree\n";

        if (data_structure == "r_tree_linear")
            results << "r_tree_linear\n";

        if (data_structure == "r_tree_star")
            results << "r_tree_star\n";

        std::cout << "running queries...\n";

        for (int i = 0; i < query_segs.size(); ++i) {
            auto qs = query_segs[i];
            if (i == 199)
                std::cout << "hello\n";
            // if (i % 50 == 0)
                // std::cout << i << std::endl;
            if (data_structure == "tsd") {  
                auto tsd_stats = tsd_query(tsd, qs);
                auto k = tsd_stats.output_size;
                results << tsd_stats.output_size << ","
                << tsd_stats.predicate_count << "\n";
            } else if (data_structure == "r_tree") {
                auto r_tree_stats = rtree_query(r_tree, qs);
                results << r_tree_stats.output_size << ","
                << r_tree_stats.predicate_count << "\n";
            } else if (data_structure == "seg_ds") {
                auto seg_ds_stats = layered_tree_query(seg_ds, qs);
                results << seg_ds_stats.output_size << ","
                << seg_ds_stats.predicate_count << "\n";
            } else if (data_structure == "r_tree_star") {
                auto seg_ds_stats = rtree_star_query(r_tree_star, qs);
                results << seg_ds_stats.output_size << ","
                << seg_ds_stats.predicate_count << "\n";
            } else if (data_structure == "r_tree_linear") {
                auto seg_ds_stats = rtree_linear_query(r_tree_linear, qs);
                results << seg_ds_stats.output_size << ","
                << seg_ds_stats.predicate_count << "\n";
            } 
        }

        results.close();
        exit(0);
    }

    void random_horizontal_exp() {
        for (int n = 128; n <= 1048576; n *= 2) {
            auto segments = io::read_segments("data/random_horizontal/" + std::to_string(n) + ".txt");
        }
    }

}