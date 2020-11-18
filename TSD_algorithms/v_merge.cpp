#include "../TSD.h"

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge_left_lower_priority_case(Node<PointType, OrderType>*& left, Node<PointType, OrderType>*& right) {
    
    
    assert(left != nullptr);
    assert(right != nullptr);
    assert(left->get_trapezoid().get_right() == right->get_trapezoid().get_left());
    assert(left->get_R() != nullptr);

    auto new_trap = BoundingTrap<PointType, OrderType>::merge(left->get_trapezoid(), right->get_trapezoid());
    auto new_node = new Node<PointType, OrderType>(new_trap);
    
    auto R = v_merge(left->get_R(), right);

    new_node->set_desc(left->get_L(), R, left->get_A(), left->get_B());
    new_node->copy_cuts(left);

    delete left;
    left = NULL;
    delete right;
    right = NULL;

    return new_node;
}

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge_right_lower_priority_case(Node<PointType, OrderType>*& left, Node<PointType, OrderType>*& right) {
    assert(left != nullptr);
    assert(right != nullptr);
    assert(left->get_trapezoid().get_right() == right->get_trapezoid().get_left());
    assert(right->get_L() != nullptr);
    
    auto new_trap = BoundingTrap<PointType, OrderType>::merge(left->get_trapezoid(), right->get_trapezoid());
    auto new_node = new Node<PointType, OrderType>(new_trap);

    auto L = v_merge(left, right->get_L());

    new_node->set_desc(L, right->get_R(), right->get_A(), right->get_B());

    delete left;
    left = NULL;
    delete right;
    right = NULL;

    return new_node;
}

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge_equal_priority_case(Node<PointType, OrderType>*& left, Node<PointType, OrderType>*& right) {
    
    assert(left != nullptr);
    assert(right != nullptr);
    assert(left->get_trapezoid().get_right() == right->get_trapezoid().get_left());

    auto new_trap = BoundingTrap<PointType, OrderType>::merge(left->get_trapezoid(), right->get_trapezoid());
    auto new_node = new Node<PointType, OrderType>(new_trap);

    if (left->is_leaf()) {
        assert(right->is_leaf());
        delete left;
        left = NULL;
        delete right;
        right = NULL;
        leaf_count--;
        return new_node;
    }
    
    assert(right->get_e() == left->get_e());

    auto v_cut = left->get_trapezoid().get_right();
    auto e_cut = left->get_e();

    if (left->get_B() == right->get_B()) {
        auto A = v_merge(left->get_A(), right->get_B());
        new_node->set_desc(left->get_L(), right->get_R(), A, left->get_B());
    } else if (left->get_A() == right->get_A()) {
        auto B = v_merge(left->get_B(), right->get_B());
        new_node->set_desc(left->get_L(), right->get_R(), left->get_A(), B);
    } else {
        throw std::logic_error("nodes should share an above or below descendant");
    }

    new_node->set_v_1(left->get_v_1());
    new_node->set_v_2(right->get_v_2());
    new_node->set_e(e_cut);

    delete left;
    left = NULL;
    delete right;
    right = NULL;
    
    return new_node;
}

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge(Node<PointType, OrderType>*& left, Node<PointType, OrderType>*& right) {
    // TODO: Implement this function

    assert(left != nullptr);
    assert(right != nullptr);

    if (left->get_priority() < right->get_priority()) {
        return v_merge_left_lower_priority_case(left, right);
    }

    if (left->get_priority() > right->get_priority()) {
        return v_merge_right_lower_priority_case(left, right);
    }

    if (left->get_priority() == right->get_priority()) {
        return v_merge_equal_priority_case(left, right);
    }

    assert(false);
}