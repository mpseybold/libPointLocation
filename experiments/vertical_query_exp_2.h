#include "../TSD.h"
#include <random>
#include <math.h>
#include <algorithm>
#include <omp.h>


void vertical_query_exp_2() {
    std::ofstream results("vert_2_results.csv");
    // if (!results.is_open())
    //     exit(0);
    // else
    //     std::cout << "file open...\n";
    results << "n,visits\n";
    for (int seg_count = 500; seg_count <= 10000; seg_count+=500) {
        int total = 0;
        #pragma omp parallel for
        for (int seed = 1; seed <= 10; seed++) {
            std::vector<Segment<PointCart, int>*> segments;

            // size_t seg_count = 10000;
            size_t long_segs = (size_t)sqrt(seg_count);

            std::uniform_real_distribution<double> dis(0.0, 1.0);
            std::mt19937 generator (seed);

            auto priorities = std::vector<int>();
            for (size_t i = 0; i < seg_count + long_segs; ++i) {
                priorities.push_back(i);
            }

            std::shuffle(priorities.begin(), priorities.end(), generator);

            for (size_t i = 0; i < seg_count; ++i) {
                auto seg  = new Segment<PointCart, int>(PointCart(0, i), PointCart(100, i), priorities[i]);
                segments.push_back(seg);
            }

            for (size_t i = 1; i <= long_segs; ++i) {
                // std::cout << 2*i*(seg_count/50) + 0.5 << std::endl;
                auto seg = new Segment<PointCart, int>(
                    PointCart(0, i*(seg_count/long_segs) + 0.5), PointCart(200, i*(seg_count/long_segs) + 0.5), priorities[seg_count + i - 1]
                );
                segments.push_back(seg);
            }

            std::shuffle(segments.begin(), segments.end(), generator);


            auto tsd = TSD<PointCart, int>();

            for (size_t i = 0; i < seg_count + long_segs; ++i) {
                // std::cout << i << std::endl;
                // std::cout << segments[i]->get_priority() << std::endl;
                tsd.insert_segment(*segments[i]);
            }

            auto query_seg = new Segment<PointCart, int>(
                PointCart(150, 100000.5), PointCart(150, -.5), 100001
            );

            tsd.affected_subdag_roots(query_seg, false);
            std::cout << tsd.get_node_visits() << std::endl;
            total += tsd.get_node_visits();
        }
        results << seg_count << "," << total/10 << "\n";
    }
    results.close();
}

