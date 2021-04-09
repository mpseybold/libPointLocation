#include "../TSD.h"


template <class PointType, class OrderType>
Node<PointType, OrderType>* 
TSD<PointType, OrderType>::left_endpoint_leaf(
    Segment<PointType, OrderType>* query_seg,
    Node<PointType, OrderType>* root_of_search) {
    
    naive_node_visits++;

    if (root_of_search->is_leaf())
        if (root_of_search->contains_endpoint(query_seg, 0))
            return root_of_search;

    if (root_of_search->get_L() != nullptr
    && root_of_search->get_L()->contains_endpoint(query_seg, 0)) {
        return left_endpoint_leaf(query_seg, root_of_search->get_L());
    } else if (root_of_search->get_R() != nullptr
    && root_of_search->get_R()->contains_endpoint(query_seg, 0)) {
        return left_endpoint_leaf(query_seg, root_of_search->get_R());
    } else if (root_of_search->get_A() != nullptr
    && root_of_search->get_A()->contains_endpoint(query_seg, 0)) {
        return left_endpoint_leaf(query_seg, root_of_search->get_A());
    } else if (root_of_search->get_B() != nullptr
    && root_of_search->get_B()->contains_endpoint(query_seg, 0)) {
        return left_endpoint_leaf(query_seg, root_of_search->get_B());
    } else {
        assert(false);
    }

    return nullptr;
}