#pragma once

#include "../TSD.h"
#include "../Point_2D.h"
#include "../segment_ds/SegmentDS.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/index/rtree.hpp>
#include "../io.h"
#include <unordered_set>
#include <chrono>
#include "../RS_TSD.h"
#include "../persist_range/persist1D.h"
// #include "query_segs.h"

// #define PATCHED_BOOST true // Enable if patched version present
#define VERBOSITY 100

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
        int time = 0;
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

    RS_TSD build_rs_tsd(std::vector<std::vector<long double>>& segments) {
        return RS_TSD(segments, nullptr);
    }

    PERSIST1D::Persist1D build_persist1d(std::vector<std::vector<long double>>& segments) {

        auto d_segs = std::vector<std::vector<double>>();

        for (auto seg: segments) {
            d_segs.push_back({
                (double)seg[0], (double)seg[1], (double)seg[2], (double)seg[3]
            });
        }

        auto p1d = PERSIST1D::Persist1D(d_segs);

        return p1d;
    }

    double variance(std::vector<int>& x, double mean) {
        double sum = 0;

        for (auto xi: x) {
            sum += (xi - mean) * (xi - mean);
        }

        return sum / x.size();
    }

    void build_rs_tsd(std::vector<std::vector<long double>>& segments, int seed, TSD<PointCart, int>& tsd) {
        
        std::cout << "building rs tsd...\n";
        
        auto _segments = std::vector<Segment<PointCart, int>*>();

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

        int n = _segments.size();

        // int m = n / log(n);
        int m = (int)sqrt(n);


        std::vector<Segment<PointCart, int>*> reg_segments = std::vector<Segment<PointCart, int>*>();

        std::vector<Segment<PointCart, int>*> conflict_segs = std::vector<Segment<PointCart, int>*>();

        for (int i = 0; i < m; ++i) {
            reg_segments.push_back(_segments[i]);
        }

        for (int i = m; i < n; ++i) {
            conflict_segs.push_back(_segments[i]);
        }

        // auto tsd = new TSD<PointCart, int>();

        for (int i = 0; i < m; ++i) {
            // std::cout << "inserting regular segs: " << reg_segments[i]->get_priority() << std::endl;
            tsd.insert_segment(*reg_segments[i]);
        }

        for (int i = m; i < n; ++i) {
            // std::cout << "inserting conflict segs: " << i << std::endl;
            tsd.insert_segment(*conflict_segs[i - m], true);
        }

        auto leaves = tsd.get_leaves();
        std::cout << "rs_tsd has " << leaves.size() << " leaves.\n";

        double mean_conflict_list = 0;

        int min = 100000000;
        int max = 0;

        std::vector<int> list_sizes = std::vector<int>();

        for (auto leaf: leaves) {
            mean_conflict_list += leaf->conflict_list.size();
            list_sizes.push_back(leaf->conflict_list.size());

            if (min > leaf->conflict_list.size())
                min = leaf->conflict_list.size();

            if (max < leaf->conflict_list.size())
                max = leaf->conflict_list.size();
        }

        mean_conflict_list /= leaves.size();

        auto var = variance(list_sizes, mean_conflict_list);

        std::cout << "mean list size: " << mean_conflict_list << std::endl;
        std::cout << "variance: " << var << std::endl;
        std::cout << "min: " << min << std::endl;
        std::cout << "max: " << max << std::endl;
    }

    void build_tsd(std::vector<std::vector<long double>>& segments, int seed, TSD<PointCart, int>& tsd) {
        auto _segments = std::vector<Segment<PointCart, int>*>();

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
        // std::cout << segments[6175][0] << " " << segments[6175][1] << " " << segments[6175][2] << " " << segments[6175][3] << std::endl;
        for (int i = 0; i < _segments.size(); ++i) {

            // if (i == 6175)
            //     continue;f
            auto s = _segments[i];
            // std::cout << (double)TSD<PointCart, int>::intersection_calls / tsd.get_node_visits() << std::endl;
            
            // if (tsd.get_node_visits() > TSD<PointCart, int>::total_search_visits / (i+1))
                // std::cout << i << std::endl;
            
            auto start_time = std::chrono::high_resolution_clock::now();
            tsd.insert_segment(*s);
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
            // std::cout << i << std::endl;
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

    QueryStatistics persist1d_query(PERSIST1D::Persist1D& p1d, std::vector<double> query_seg) {
        
        auto stats = QueryStatistics();

        auto start_time = std::chrono::high_resolution_clock::now();
        if( VERBOSITY>=100 ){
            std::cout << "y1: " << (int)query_seg[1] << std::endl;
            std::cout << "y2: " << (int)query_seg[3] << std::endl;
            std::cout << "x: " << (int)query_seg[0] << std::endl;
        }
        int k = p1d.v_query((int)query_seg[1], (int)query_seg[3], (int)query_seg[0]);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        
        stats.time = duration;
        stats.output_size = k;
        stats.predicate_count = MPS_COUNTER;

        if( VERBOSITY>=100 )
            std::cout << "k: " << k <<std::endl;

        return stats;
    }

    QueryStatistics rtree_query(R_Tree& r_tree, std::vector<double> query_seg) {
        auto stats = QueryStatistics();

        #ifdef PATCHED_BOOST 
            boost::geometry::predicate_count    = 0;
            boost::geometry::index::node_visits = 0;
        #endif

        std::vector<segment_t> results;
        segment_t _query_seg{{query_seg[0], query_seg[1]}, {query_seg[2], query_seg[3]}};
        auto start_time = std::chrono::high_resolution_clock::now();
        r_tree.query(
            bgi::intersects(_query_seg), 
            std::back_inserter(results));
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        stats.output_size = results.size();
        #ifdef PATCHED_BOOST
            stats.predicate_count = boost::geometry::predicate_count;
        #endif
        if( VERBOSITY>=1000 )
            std::cout << duration << std::endl;
        stats.time = duration;
        return stats;
    }

    QueryStatistics rtree_linear_query(R_Tree_Linear& r_tree, std::vector<double> query_seg) {
        auto stats = QueryStatistics();
        std::vector<segment_t> results;
        segment_t _query_seg{{query_seg[0], query_seg[1]}, {query_seg[2], query_seg[3]}};
        r_tree.query(bgi::intersects(_query_seg), std::back_inserter(results));
        stats.output_size = results.size();
        #ifdef PATCHED_BOOST
            stats.predicate_count = boost::geometry::predicate_count;
        #endif
        return stats;
    }

    QueryStatistics rtree_star_query(R_Tree_Star& r_tree, std::vector<double> query_seg) {
        auto stats = QueryStatistics();
        #ifdef PATCHED_BOOST
            boost::geometry::predicate_count = 0;
        #endif
        boost::geometry::index::my_node_visits = 0;
        std::vector<segment_t> results;
        segment_t _query_seg{{query_seg[0], query_seg[1]}, {query_seg[2], query_seg[3]}};
        auto start_time = std::chrono::high_resolution_clock::now();
        r_tree.query(bgi::intersects(_query_seg), std::back_inserter(results));
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        stats.output_size = results.size();
        #ifdef PATCHED_BOOST
            stats.predicate_count = boost::geometry::predicate_count;
        #endif
        stats.predicate_count = boost::geometry::index::my_node_visits;
        stats.time = duration;

        return stats;
    }

    QueryStatistics rs_tsd_query(std::vector<std::vector<long double>> segments, std::vector<double> query_seg) {
        if( VERBOSITY>=1000 )
            std::cout << "starting new query...\n";

        auto query = new Segment<PointCart, int>(
            PointCart(query_seg[0], query_seg[1]), PointCart(query_seg[2], query_seg[3]),
            100000001
        );
        auto stats = QueryStatistics();
        auto start_time = std::chrono::high_resolution_clock::now();
        auto rs_tsd = RS_TSD(segments, query);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        stats.time = rs_tsd.query_time;
        stats.output_size = rs_tsd.intersections.size();

        if( VERBOSITY>=1000 )
            std::cout << "returning query stats...\n";

        return stats;
    }

    bool ver_hor_int_(Segment<PointCart, int> vert, Segment<PointCart, int> hor) {
        
        Cut<PointCart, int> c1 = Cut<PointCart, int>(EDGE, &vert, nullptr);
        Cut<PointCart, int> c2 = Cut<PointCart, int>(EDGE, &hor, nullptr);

        return c1.has_seg_on_neg_side(&hor) && c1.has_seg_on_pos_side(&hor) &&
        c2.has_seg_on_neg_side(&vert) && c2.has_seg_on_pos_side(&vert);
        
        if (
            vert.get_source().Y_() > hor.get_source().Y_() &&
            vert.get_target().Y_() < hor.get_source().Y_() &&
            vert.get_source().X_() < hor.get_target().X_() &&
            vert.get_source().X_() > hor.get_source().X_()
        )
            return true;

        return false;
    }

    QueryStatistics rs_tsd_query_beta(TSD<PointCart, int>& tsd, std::vector<double> query_seg) {
        
        int count = 0;

        auto stats = QueryStatistics();
        auto output = std::vector<std::vector<long double>>();

        auto query = new Segment<PointCart, int>(
            PointCart(query_seg[0], query_seg[1]), PointCart(query_seg[2], query_seg[3]), 100000001
        );
        
        auto start_time = std::chrono::high_resolution_clock::now();


        tsd.affected_subdag_roots(query, true, true);
        auto leaves = tsd.get_subdag_roots();

        for (int i = 0; i < leaves.size() - 1; ++i) {
            
            auto top_seg = leaves[i]->get_trapezoid().get_top()->get_segment();

            output.push_back(
                {
                    top_seg->get_source().X_(), top_seg->get_source().Y_(),
                    top_seg->get_target().X_(), top_seg->get_target().Y_()
                }
            );
        }

        for (int i = 0; i < leaves.size(); ++i) {
            for (auto seg: leaves[i]->conflict_list) {
                count++;
                if (ver_hor_int_(*query, seg)) {
                    output.push_back(
                        {
                            seg.get_source().X_(), seg.get_source().Y_(),
                            seg.get_target().X_(), seg.get_target().Y_()
                        }
                    );
                }
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        stats.predicate_count = count + tsd.get_node_visits();
        stats.output_size = output.size();
        stats.time = duration;
        delete query;
        
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
        auto start_time = std::chrono::high_resolution_clock::now();
        segment_ds.vertical_query(_query_seg, output);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        if( VERBOSITY>=100 )
            std::cout << "output_size: " << output.size() << std::endl;
        stats.output_size = output.size();
        stats.predicate_count = SegmentDS::node_visits;
        stats.time = duration;
        return stats;
    }

    QueryStatistics tsd_query(TSD<PointCart, int>& tsd, std::vector<double> query_seg) {
        
        auto stats = QueryStatistics();
        auto _query_seg = Segment<PointCart, int>(
            PointCart(query_seg[0], query_seg[1]), PointCart(query_seg[2], query_seg[3]), 10000002
        );

        auto start_time = std::chrono::high_resolution_clock::now();
        tsd.affected_subdag_roots(&_query_seg, true);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        stats.predicate_count = tsd.get_node_visits();
        // stats.predicate_count = TSD<PointCart, int>::intersection_calls;
        stats.output_size = tsd.get_subdag_roots().size() - 1;
        stats.time = duration;
        return stats;
    }

    std::vector<std::vector<double>>
    query_segs_for_bad_conflict() {
        auto segs = io::read_segments("data/query_segments/bad_conflict.txt");
        auto output = std::vector<std::vector<double>>();

        for (auto s: segs) {
            output.push_back({
                (double)s[0], (double)s[1], (double)s[2], (double)s[3]
            });
        }

        return output;
    }


    std::vector<std::vector<double>> 
    query_segs_for_osm(std::string dataset) {
        std::cout << "data/query_segments/"+dataset+"_q.csv\n";
        auto segs = io::read_segments("data/query_segments/"+dataset+"_q.csv");
        std::cout << segs[0][0] << " " << segs[0][1] << " " << segs[0][2] << " " << segs[0][3] << std::endl;
        // exit(0);
        auto output = std::vector<std::vector<double>>();

        // Hot vs Cold caching.
        // std::uniform_real_distribution<double> dis(0.0, 1.0);
        // std::mt19937 generator (10);
        // std::cout << "Putting queries in a random order" << std¨endl;
        // std::shuffle(segs.begin(), segs.end(), generator);

        for( int i=0; i<segs.size(); ++i /*300*/)
            output.push_back({(double)segs[i][0], (double)segs[i][1],
                              (double)segs[i][2], (double)segs[i][3]});

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
        if( VERBOSITY>=100 )
            std::cout << "IN query_segs_for_seg_tree_worst_case\t Generating Random Queries...\n";
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
        std::vector<std::vector<long double>> segments;

        if (instance_type == "random_horizontal" 
        || instance_type == "random_slanted" 
        || instance_type == "seg_tree_worst_case") {
            segments = io::read_segments("data/"+instance_type+"/" + std::to_string(size) + ".txt");
        } else {
            segments = io::read_segments("data/osm/"+instance_type+".csv");
        }
        
        // auto segments = instance_type != "osm" ? io::read_segments("data/"+instance_type+"/" + std::to_string(size) + ".txt")
        // : io::read_segments("data/osm/"+instance_type+"_clean.csv");

        std::vector<std::vector<double>> query_segs;
        std::cout << "fetching query segs...\n";
        if (instance_type == "random_horizontal")
            query_segs = query_segs_for_random_horizontal(segments, num_of_queries);
        else if (instance_type == "random_slanted")
            query_segs = query_segs_for_random_slanted(segments, num_of_queries);
        else if (instance_type == "seg_tree_worst_case")
            query_segs = query_segs_for_seg_tree_worst_case(size, num_of_queries);
        else if (instance_type == "bad_conflict")
            query_segs = query_segs_for_bad_conflict();
        else
            query_segs = query_segs_for_osm(instance_type);

        std::cout << "fetched nofQueries=" << query_segs.size() << std::endl;

        R_Tree r_tree;
        R_Tree_Linear r_tree_linear;
        R_Tree_Star r_tree_star;

        std::vector<SegmentDS::Segment*> seg_ds_segs{new SegmentDS::Segment(0, 0, 1, 1), new SegmentDS::Segment(0, 0, 2, 1)};
        SegmentDS::SegmentDS seg_ds = SegmentDS::SegmentDS(seg_ds_segs);
        TSD<PointCart, int> tsd = TSD<PointCart, int>();
        TSD<PointCart, int> rs_tsd = TSD<PointCart, int>();
        PERSIST1D::Persist1D p1d;
        // RS_TSD rs_tsd = RS_TSD(std::vector<std::vector<long double>>(), nullptr);


        if( VERBOSITY>=100 )
            std::cout << "building tsd...\n";
        if (data_structure == "tsd")
            build_tsd(segments, 10, tsd);
        if( VERBOSITY>=100 )
            std::cout << "building r_tree...\n";
        if (data_structure == "r_tree") {
            r_tree = build_r_tree(segments);
            auto alloc = r_tree.get_allocator();
        }

        if (data_structure == "r_tree_linear")
            r_tree_linear = build_r_tree_linear(segments);

        if (data_structure == "r_tree_star")
            r_tree_star = build_r_tree_star(segments);

        if (data_structure == "seg_ds")    
            seg_ds = build_layered_tree(segments);

        if (data_structure == "rs_tsd") {
            build_rs_tsd(segments, 10, rs_tsd); 
            // rs_tsd = build_rs_tsd(segments); // random seed
        }

        if (data_structure == "persist_1d") {
            std::cout << "building 1d persist...\n";
            p1d = build_persist1d(segments);    // random seed
        }

        std::cout << "built data structures...\n";

        std::string res_string;

        if (instance_type == "random_horizontal" 
        || instance_type == "random_slanted" 
        || instance_type == "seg_tree_worst_case") {
            res_string = "data/results/" + instance_type + "/" + std::to_string(size) + data_structure + ".csv";
        } else {
            res_string = "data/results/osm/" + instance_type + "_" + data_structure +".csv";
        }


        std::ofstream results(res_string);

        results << "k,time,comp_count\n";

        // if (data_structure == "tsd")
        //     results << "tsd\n";

        // if (data_structure == "seg_ds")
        //     results << "seg_ds\n";

        // if (data_structure == "r_tree")
        //     results << "r_tree\n";

        // if (data_structure == "r_tree_linear")
        //     results << "r_tree_linear\n";

        // if (data_structure == "r_tree_star")
        //     results << "r_tree_star\n";

        // if (data_structure == "rs_tsd")
        //     results << "rs_tsd\n";

        if( VERBOSITY>=1 )
            std::cout << "running all queries on " << data_structure << ", writing results to file...\n";

        for (int i = 0; i < query_segs.size(); ++i) {
            auto qs = query_segs[i];
            QueryStatistics result;
            std::cout << i << ":\t" << qs[0] <<",\t"<< qs[1] 
                           << ",\t" << qs[2] <<",\t"<< qs[3] << "\t:\t";
            if (data_structure == "tsd") {  
                result = tsd_query(tsd, qs); /*
                auto tsd_stats = tsd_query(tsd, qs);
                auto k = tsd_stats.output_size;
                    results << tsd_stats.output_size << ", "
                    << tsd_stats.time << ", " <<
                    tsd_stats.predicate_count << "\n"; */
            } else if (data_structure == "r_tree") {
                result = rtree_query(r_tree, qs); /*
                auto r_tree_stats = rtree_query(r_tree, qs);
                // if (r_tree_stats.output_size > 0)
                    results << r_tree_stats.output_size << ","
                << r_tree_stats.time << "," << r_tree_stats.predicate_count << "\n"; */
            } else if (data_structure == "seg_ds") {
                result = layered_tree_query(seg_ds, qs);/*
                auto seg_ds_stats = layered_tree_query(seg_ds, qs);
                results << seg_ds_stats.output_size << ","
                << seg_ds_stats.time << "," << seg_ds_stats.predicate_count << "\n";
                std::cout << "predicate_count: " << seg_ds_stats.predicate_count << std::endl; */
            } else if (data_structure == "r_tree_star") { 
                result = rtree_star_query(r_tree_star, qs);/*
                auto rtree_star_stats = rtree_star_query(r_tree_star, qs);
                results << rtree_star_stats.output_size << "," << rtree_star_stats.time << ","
                << rtree_star_stats.predicate_count << "\n"; */
            } else if (data_structure == "r_tree_linear") {
                result = rtree_linear_query(r_tree_linear, qs); /*
                auto seg_ds_stats = rtree_linear_query(r_tree_linear, qs);
                if (seg_ds_stats.output_size > 0)
                    results << seg_ds_stats.output_size << ","
                << seg_ds_stats.predicate_count << "\n"; */
            } else if (data_structure == "rs_tsd") {
                result = rs_tsd_query_beta(rs_tsd, qs); /*
                auto rs_tsd_stats = rs_tsd_query_beta(rs_tsd, qs);
                // if (rs_tsd_stats.output_size > 0) {
                    results << rs_tsd_stats.output_size << ", " << rs_tsd_stats.time << ", " 
                    << rs_tsd_stats.predicate_count << "\n";
                // }
                // auto rs_tsd_stats = rs_tsd_query(segments, qs);
                // results << rs_tsd_stats.output_size << ", " << rs_tsd_stats.time << "\n";
                */
            } else if (data_structure == "persist_1d") {
                result = persist1d_query(p1d, qs); /*
                auto p1d_stats = persist1d_query(p1d, qs);
                results << p1d_stats.output_size << ", " << p1d_stats.time << ", " 
                    << p1d_stats.predicate_count << "\n"; */
            }
            results << result.output_size << ", " << result.time << ", "<< result.predicate_count << "\n";
            if( VERBOSITY>=100 )
                std::cout << result.output_size << ", " << result.time << ", "<< result.predicate_count << "\n";
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
