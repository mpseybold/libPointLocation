#include "../TSD.h"

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::merge_equal_priority_case(Node<PointType, OrderType>* below, Node<PointType, OrderType>* above) {
    if (below->is_leaf()) {
        auto new_trap = BoundingTrap<PointType, OrderType>::merge(above->get_trapezoid(), below->get_trapezoid());
        auto new_node = new Node<PointType, OrderType>(new_trap);

        delete_node(below);
        delete_node(above);

        return new_node;
    }

    return nullptr;
}

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::merge(Node<PointType, OrderType>* below, Node<PointType, OrderType>* above) {
    if (below->get_priority() == above->get_priority()) {
        return merge_equal_priority_case(below, above);
    }

    return nullptr;
}