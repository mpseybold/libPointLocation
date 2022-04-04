#include "RS_TSD.h"




bool ver_hor_int(Segment<PointCart, int> vert, Segment<PointCart, int> hor) {
    if (
        vert.get_source().Y_() > hor.get_source().Y_() &&
        vert.get_target().Y_() < hor.get_source().Y_() &&
        vert.get_source().X_() < hor.get_target().X_() &&
        vert.get_source().X_() > hor.get_source().X_()
    )
        return true;

    std::cout << "don't intersect\n";

    return false;
}

RS_TSD::RS_TSD(std::vector<std::vector<long double>> segments, Segment<PointCart, int>* query) {
    int n = segments.size();

    int seed = 1001;
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    std::mt19937 generator (seed);

    std::shuffle(segments.begin(), segments.end(), generator);

    int m = n / log(n);

    std::vector<Segment<PointCart, int>> reg_segments = std::vector<Segment<PointCart, int>>();

    std::vector<Segment<PointCart, int>> conflict_segs = std::vector<Segment<PointCart, int>>();

    for (int i = 0; i < m; ++i) {
        reg_segments.push_back(
            Segment<PointCart, int>(PointCart(segments[i][0], segments[i][1]),
            PointCart(segments[i][2], segments[i][3]), i)
        );
    }

    for (int i = m; i < n; ++i) {
        conflict_segs.push_back(
            Segment<PointCart, int>(PointCart(segments[i][0], segments[i][1]),
            PointCart(segments[i][2], segments[i][3]), i)
        );
    }

    auto tsd = new TSD<PointCart, int>();

    for (int i = 0; i < m; ++i) {
        std::cout << "inserting regular segs: " << reg_segments[i].get_priority() << std::endl;
        tsd->insert_segment(reg_segments[i]);
    }

    for (int i = m; i < n; ++i) {
        // std::cout << "inserting conflict segs...\n";
        tsd->insert_segment(conflict_segs[i - m], true);
    }

    std::cout << "RS_TSD query!!\n";

    std::vector<std::vector<long double>> output = std::vector<std::vector<long double>>();

    // Segment<PointCart, int>* query = new Segment<PointCart, int>(
    //     PointCart(q[0], q[1]), PointCart(q[2], q[3]), 10000001);

    // std::cout << "before query: " << tsd->get_root()->get_priority() << std::endl;    
    auto start_time = std::chrono::high_resolution_clock::now();

    if (query != nullptr) {
        tsd->affected_subdag_roots(query, true, true);
        auto leaves = tsd->get_subdag_roots();

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
                if (ver_hor_int(*query, seg)) {
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
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        intersections = output;
        query_time = duration;
        delete query;
    }
}

std::vector<std::vector<long double>> RS_TSD::query(/*std::vector<double> q*/
Segment<PointCart, int>* query) {

    std::vector<std::vector<long double>> output = std::vector<std::vector<long double>>();

    // Segment<PointCart, int>* query = new Segment<PointCart, int>(
    //     PointCart(q[0], q[1]), PointCart(q[2], q[3]), 10000001);

    // std::cout << "before query: " << tsd->get_root()->get_priority() << std::endl;    

    tsd->affected_subdag_roots(query, true, true);
    auto leaves = tsd->get_subdag_roots();


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
        std::cout << "conflict_list size: " << leaves[i]->conflict_list.size() <<  std::endl;

        for (auto seg: leaves[i]->conflict_list) {
            if (ver_hor_int(*query, seg)) {
                output.push_back(
                    {
                        seg.get_source().X_(), seg.get_source().Y_(),
                        seg.get_target().X_(), seg.get_target().Y_()
                    }
                );
            }
        }
    }

    delete query;
    return output;
}