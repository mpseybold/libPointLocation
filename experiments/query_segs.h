#pragma once

#include "../io.h"
#include "../segment_ds/SegmentDS.h"
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/algorithms/detail/intersects/interface.hpp>

namespace query_seg_utility {

    namespace bg = boost::geometry;
    typedef bg::model::point<long double, 2, bg::cs::cartesian> _point_t;
    using _segment_t = bg::model::segment<_point_t>;
    using R_Tree = bgi::rtree<_segment_t, bgi::quadratic<16>>;

    std::vector<std::vector<long double>> remove_intersections(std::vector<std::vector<long double>> initial_segs, std::string new_file_name) {
        auto output = std::vector<std::vector<long double>>();
        R_Tree rtree;
        std::cout << std::setprecision(20);
        for (int i = 0; i < initial_segs.size(); ++i) {
            auto s = initial_segs[i];

            // std::cout << output.size();

            if (i == 0) {
                output.push_back(s);
                rtree.insert({{s[0], s[1]}, {s[2], s[3]}});
            }
            else {
                std::vector<_segment_t> results;
                _segment_t _query_seg{{s[0], s[1]}, {s[2], s[3]}};
                rtree.query(bgi::intersects(_query_seg), std::back_inserter(results));

                int k = results.size();

                bool common_endpoints_only = true;

                for (auto t: results) {
                    if (!((bg::get<0, 0>(_query_seg) == bg::get<0, 0>(t) && bg::get<0, 1>(_query_seg) == bg::get<0, 1>(t))
                    || (bg::get<0, 0>(_query_seg) == bg::get<1, 0>(t) && bg::get<0, 1>(_query_seg) == bg::get<1, 1>(t))
                    || (bg::get<1, 0>(_query_seg) == bg::get<0, 0>(t) && bg::get<1, 1>(_query_seg) == bg::get<0, 1>(t))
                    || (bg::get<1, 0>(_query_seg) == bg::get<1, 0>(t) && bg::get<1, 1>(_query_seg) == bg::get<1, 1>(t)))) {
                        common_endpoints_only = false;
                    }
                }
                
                // std::cout << k << std::endl;

                if (common_endpoints_only && s[0] != s[2]) {
                    // std::cout << "candidate seg: " << s[0] << " " << s[1] << " " << s[2] << " " << s[3] << "\n";
                    output.push_back(s);
                    rtree.insert({{s[0], s[1]}, {s[2], s[3]}});
                } else {
                    // std::cout << "candidate seg: " << s[0] << " " << s[1] << " " << s[2] << " " << s[3] << "\n";
                    // for (auto _t: results) {
                        // std::vector<double> t = std::vector<double>();
                        // t.push_back(bg::get<0, 0>(_t));
                        // t.push_back(bg::get<0, 1>(_t));
                        // t.push_back(bg::get<1, 0>(_t));
                        // t.push_back(bg::get<1, 1>(_t));
                        

                        // std::cout << "\tintersecting seg: " << t[0] << " " << t[1] << " " << t[2] << " " << t[3] << "\n";
                    // } 
                }
            }
        }

        io::write_segments(output, new_file_name);

        return output;
    }


    void get_query_segs(std::string seg_filename, double scale=1) {
        auto segments = io::read_segments("data/osm/" + seg_filename + ".csv");
        std::cout << seg_filename << "\n";
        // std::cout << "before clean: " << segments.size() << "\n";
        // segments = remove_intersections(segments, seg_filename + "int_free.txt");
        // std::cout << "after clean: " << segments.size() << "\n";
        
        R_Tree rtree;

        auto x_coords = std::vector<double>();
        auto y_coords = std::vector<double>();

        for (int i = 0; i < segments.size(); ++i) {
            auto s = segments[i];
            rtree.insert({{s[0], s[1]}, {s[2], s[3]}});
        
            x_coords.push_back(s[0]);
            x_coords.push_back(s[2]);

            y_coords.push_back(s[1]);
            y_coords.push_back(s[3]);
        }

        double x_min = *std::min_element(x_coords.begin(), x_coords.end());
        double x_max = *std::max_element(x_coords.begin(), x_coords.end());
        double y_min = *std::min_element(y_coords.begin(), y_coords.end());
        double y_max = *std::max_element(y_coords.begin(), y_coords.end());

        std::cout << x_min << "\n";
        std::cout << x_max << "\n";
        std::cout << y_min << "\n";
        std::cout << y_max << "\n";
        

        double total_height = abs(y_max - y_min);
        double total_width = abs(x_max - x_min);

        auto output = std::vector<std::vector<double>>();

        int n = segments.size();

        for (int i = 0; i < 100; ++i) {
            for (int j = 0; j < sqrt(n); ++j) {
                double x  = x_min + i * total_width / 100;
                double y = j * j * (total_height / sqrt(n));

                // std::cout << x << std::endl;

                std::vector<double> q_seg{ x, y_max, x, y_max - j*total_height / sqrt(n)/*y + scale*total_height / n*/};

                std::vector<_segment_t> results;
                _segment_t _query_seg{{q_seg[0], q_seg[1]}, {q_seg[2], q_seg[3]}};
                rtree.query(bgi::intersects(_query_seg), std::back_inserter(results));

                int k = results.size();


                if (k < 2*log(n) && k > .5*log(n)) {
                    std::cout << k << std::endl;
                    output.push_back(q_seg);
                }
            }
        }

        std::cout << output.size() << std::endl;
        std::cout << log(n) << std::endl;

        io::write_segments(output, "data/query_segments/" + seg_filename + "_q.csv");
        std::cout << "data/query_segments/" + seg_filename + "_q.csv" << std::endl;
    }
}