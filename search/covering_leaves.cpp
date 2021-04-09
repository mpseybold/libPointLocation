#include "../TSD.h"

/*
* NOTE: Currently only works for vertical query segments
* for a tsd containing only horizontal segments
*/
template <class PointType, class OrderType>
std::vector<Node<PointType, OrderType>*> 
TSD<PointType, OrderType>::covering_leaves(Segment<PointType,  OrderType>* query_seg) {
    
    naive_node_visits = 0;

    std::vector<Node<PointType, OrderType>*> leaves {
        left_endpoint_leaf(query_seg, root)
    };

    Segment<PointType, OrderType> last_seg = *query_seg;

    while (!leaves.back()->contains_endpoint(&last_seg, 1)) {
        auto last_trap = leaves.back()
        ->get_trapezoid();

        auto top_cut = last_trap.get_top();
        auto bottom_cut = last_trap.get_bottom();
        auto top_seg = top_cut->get_segment();
        auto bottom_seg = bottom_cut->get_segment();

        auto query_cut = Cut<PointType, OrderType>(
            EDGE, &last_seg, nullptr
        );

        auto top_int = Cut<PointType, OrderType>(
            INTERSECTION, &last_seg, top_cut->get_segment()
        );

        auto bottom_int = Cut<PointType, OrderType>(
            INTERSECTION, &last_seg, bottom_cut->get_segment()
        );

        Segment<PointType, OrderType> new_seg =
        Segment<PointType, OrderType>(bottom_int.get_defining_point(), last_seg.get_target(), 0);

        leaves.push_back(
            left_endpoint_leaf(&new_seg, root)
        );

        last_seg = new_seg;
    }

    return leaves; 
}