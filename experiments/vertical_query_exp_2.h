#include "../TSD.h"
#include "../segment_ds/SegmentDS.h"
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
    results << "n,visits,seg_ds\n";
    for (int seg_count = 10000; seg_count <= 400000; seg_count+=10000) {
        int total = 0;
        int total_seg_ds = 0;
        std::cout << "seg_count: " << seg_count << std::endl;
        #pragma omp parallel for
        for (int seed = 1; seed <= 10; seed++) {
            std::vector<Segment<PointCart, int>*> segments;
            std::vector<SegmentDS::Segment*> o_segs = std::vector<SegmentDS::Segment*>();

            // size_t seg_count = 10000;
            size_t long_segs = (size_t)sqrt(seg_count);

            std::uniform_real_distribution<double> dis(0.0, 1.0);
            std::mt19937 generator (seed);

            auto priorities = std::vector<int>();
            for (size_t i = 0; i <= seg_count; ++i) {
                priorities.push_back(i);
            }

            std::shuffle(priorities.begin(), priorities.end(), generator);

            for (size_t i = 0; i < seg_count; ++i) {
                if (i == 0 || i % (seg_count / long_segs) != 0) {
                    auto seg  = new Segment<PointCart, int>(PointCart(0, i), PointCart(100, i), priorities[i]);
                    segments.push_back(seg);
                    if (seed == 1)
                        o_segs.push_back(new SegmentDS::Segment(0, i, 100, i));
                }
            }

            for (size_t i = 1; i <= long_segs; ++i) {
                // std::cout << 2*i*(seg_count/50) + 0.5 << std::endl;
                auto seg = new Segment<PointCart, int>(
                    PointCart(0, i*(seg_count/long_segs)), PointCart(200, i*(seg_count/long_segs)), priorities[i*(seg_count/long_segs)]
                );
                segments.push_back(seg);
                if (seed == 1)
                    o_segs.push_back(new SegmentDS::Segment(0, i*(seg_count/long_segs), 200, i*(seg_count/long_segs)));
            }

            std::sort(segments.begin(), segments.end());


            auto tsd = TSD<PointCart, int>();

            for (size_t i = 0; i < seg_count; ++i) {
                // std::cout << i << std::endl;
                // std::cout << segments[i]->get_priority() << std::endl;
                tsd.insert_segment(*segments[i]);
            }

            auto query_seg = new Segment<PointCart, int>(
                PointCart(150, 100000.5), PointCart(150, -.5), 100001
            );

            if (seed == 1) {
                auto _query_seg = SegmentDS::Segment(150, -.5, 150, 100000.5);
                auto seg_ds = SegmentDS::SegmentDS(o_segs);
                auto output = std::vector<SegmentDS::Segment>();
                seg_ds.vertical_query(_query_seg, output);
                total_seg_ds = SegmentDS::node_visits;
            }
            tsd.affected_subdag_roots(query_seg, false);
            std::cout << tsd.get_node_visits() << std::endl;
            total += tsd.get_node_visits();

            for (auto s: o_segs)
                delete s;
            std::cout << "long_segs: " << long_segs << std::endl;
        }
        
        results << seg_count << "," << total/10 << "," << total_seg_ds << "\n";
    }
    results.close();
}

