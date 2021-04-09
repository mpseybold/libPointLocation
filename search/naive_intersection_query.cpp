#include "../TSD.h"

template <class PointType, class OrderType>
std::vector<Segment<PointType, OrderType>*>
TSD<PointType, OrderType>::naive_intersection_query(Segment<PointType, OrderType>* query_seg) {
    auto leaves = covering_leaves(query_seg);

    std::vector<Segment<PointType, OrderType>*> segs {};

    for (int i = 0; i < leaves.size() - 1; ++i) {
        segs.push_back(leaves[i]
        ->get_trapezoid()
        .get_top()->get_segment());
    }

    return segs;
}