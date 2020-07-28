#include "../TSD.h"

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_leaf_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut) {
    assert(node != nullptr);
    assert(e_cut.get_cut_type() == EDGE);

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(e_cut);
    auto A = new Node<PointType, OrderType>(pos_neg.first);
    auto B = new Node<PointType, OrderType>(pos_neg.second);

    node->set_A(A);
    node->set_B(B);
    node->set_e(e_cut);
    node->toggle_partition_visited();
    partition_visited_nodes.push_back(node);
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_E_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut) {
    assert(node != nullptr);
    assert(e_cut.get_cut_type() == EDGE);
    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_e().get_cut_type() == EDGE);

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(e_cut);
    auto A = new Node<PointType, OrderType>(pos_neg.first);
    auto B = new Node<PointType, OrderType>(pos_neg.second);

    bool intersects_A = node->get_A()
    ->get_trapezoid()
    .intersects_segment(e_cut.get_segment());

    bool intersects_B = node->get_B()
    ->get_trapezoid()
    .intersects_segment(e_cut.get_segment());

    assert(intersects_A || intersects_B);

    if (intersects_B && intersects_A) {
        auto intersection_cut = Cut<PointType, OrderType>(
            INTERSECTION, e_cut, node->get_e()
        );

        if (!node->get_A()->is_partition_visited()) {
            v_partition(node->get_A(), intersection_cut);
            node->get_A()->toggle_partition_visited();
            partition_visited_nodes.push_back(node->get_A());
        }

        if (!node->get_B()->is_partition_visited())
    } else if(intersects_B) {
        
    } else if(intersects_A) {

    }
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_V_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut) {

}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_VV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut) {

}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_EV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut) {

}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_VE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut) {

}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_VVE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut) {

}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& cut) {
    //TODO: Implement this function.

    assert(node != nullptr);
    DestructionPattern pattern = node->get_dest_pattern();

    switch(pattern) {
        case NO_DESTRUCTION: {
            partition_leaf_case(node, cut);
            break;
        }
        case E: {
            partition_E_case(node, cut);
            break;
        }
        case VE: {
            partition_VE_case(node, cut);
            break;
        }
        case EV: {
            partition_EV_case(node, cut);
            break;
        }
        case VVE: {
            partition_VVE_case(node, cut);
            break;
        }
        case V: {
            partition_V_case(node, cut);
        }
        case VV: {
            partition_VV_case(node, cut);
            break;
        }
    }
}

template class TSD<PointCart, int>;