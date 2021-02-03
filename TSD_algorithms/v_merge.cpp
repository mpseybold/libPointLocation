#include "../TSD.h"

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge_left_lower_priority_case(Node<PointType, OrderType>*& left, Node<PointType, OrderType>*& right) {
    
    
    assert(left != nullptr);
    assert(right != nullptr);
    assert(left->get_trapezoid().get_right() == right->get_trapezoid().get_left());
    assert(left->get_R() != nullptr);


    auto new_trap = BoundingTrap<PointType, OrderType>::merge(left->get_trapezoid(), right->get_trapezoid());
    auto new_node = new Node<PointType, OrderType>(new_trap);

    auto right_merge_node = right->get_right();

    auto right_merge_neighbour = right->get_right();
    auto left_merge_neighbour = left->get_left();

    bool update_right_merge_pointer = right->get_priority() < left->get_R()->get_priority();

    auto R = v_merge(left->get_R(), right);

    new_node->set_desc(left->get_L(), R, left->get_A(), left->get_B());
    new_node->copy_cuts(left);

    new_node->get_R()->set_right(right_merge_node);
    if (right_merge_node != nullptr)
        right_merge_node->set_left(new_node->get_R());

    delete_node(left);
    left = NULL;
    // delete right;
    // right = NULL;

    if (right_merge_neighbour != nullptr && update_right_merge_pointer) {
        right_merge_neighbour->set_left(new_node->get_R());
        new_node->get_R()->set_right(right_merge_neighbour);
    }

    if (left_merge_neighbour != nullptr) {
        left_merge_neighbour->set_right(new_node);
        new_node->set_left(left_merge_neighbour);
    }

    assert(new_node != nullptr);
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

    auto left_merge_node = right->get_left();

    auto right_merge_neighbour = right->get_right();
    auto left_merge_neighbour = left->get_left();

    bool update_left_merge_pointer = left->get_priority() < right->get_L()->get_priority();

    auto L = v_merge(left, right->get_L());

    new_node->set_desc(L, right->get_R(), right->get_A(), right->get_B());
    new_node->copy_cuts(right);

    // delete left;
    delete_node(right);
    

    if (left_merge_neighbour != nullptr && update_left_merge_pointer) {
        left_merge_neighbour->set_right(new_node);
        new_node->set_left(left_merge_neighbour);
    }

    if (right_merge_neighbour != nullptr) {
        right_merge_neighbour->set_left(new_node);
        new_node->set_right(right_merge_neighbour);
    }

    assert(new_node != nullptr);
    return new_node;
}

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge_equal_priority_case(Node<PointType, OrderType>*& left, Node<PointType, OrderType>*& right) {
    
    assert(left != nullptr);
    assert(right != nullptr);
    if (left == right)
        return left;
    if (left->get_trapezoid().get_right() != right->get_trapezoid().get_left())
        std::cout << "hello\n";
    assert(left->get_trapezoid().get_right() == right->get_trapezoid().get_left());

    std::cout << "v_merge_equal_priority\n";

    auto common_boundary_cut = left->get_trapezoid().get_right();

    auto new_trap = BoundingTrap<PointType, OrderType>::merge(left->get_trapezoid(), right->get_trapezoid());
    auto new_node = new Node<PointType, OrderType>(new_trap);

    if (left->is_leaf()) {
        assert(right->is_leaf());
        delete_node(left);
        // left = NULL;
        delete_node(right);
        // right = NULL;
        leaf_count--;
        assert(new_node != nullptr);
        return new_node;
    }

    auto old_left_A = left->get_A();
    auto old_left_B = left->get_B();
    auto old_right_A = right->get_A();
    auto old_right_B = right->get_B();

    bool left_A_will_vanish = old_left_A->get_trapezoid().get_right()
    ->defining_point_cut_comparison(*common_boundary_cut) == 0;
    bool left_B_will_vanish = old_left_B->get_trapezoid().get_right()
    ->defining_point_cut_comparison(*common_boundary_cut) == 0;
    bool right_A_will_vanish = old_right_A->get_trapezoid().get_left()
    ->defining_point_cut_comparison(*common_boundary_cut) == 0;
    bool right_B_will_vanish = old_right_B->get_trapezoid().get_left()
    ->defining_point_cut_comparison(*common_boundary_cut) == 0;


    auto right_merge_neighbour = right->get_right();
    auto left_merge_neighbour = left->get_left();
    
    assert(right->get_e() == left->get_e());

    auto v_cut = left->get_trapezoid().get_right();
    auto e_cut = left->get_e();

    Node<PointType, OrderType>* A;
    Node<PointType, OrderType>* B;

    if (left->get_B() == right->get_B()) {
        A = v_merge(left->get_A(), right->get_A());
        new_node->set_desc(left->get_L(), right->get_R(), A, left->get_B());
    } else if (left->get_A() == right->get_A()) {
        B = v_merge(left->get_B(), right->get_B());
        new_node->set_desc(left->get_L(), right->get_R(), left->get_A(), B);
    } else {
        assert(common_boundary_cut->defining_point_cut_comparison(*left->get_e()) == 0);
        A = v_merge(left->get_A(), right->get_A());
        B = v_merge(left->get_B(), right->get_B());
        new_node->set_desc(left->get_L(), right->get_R(), A, B);
        // throw std::logic_error("nodes should share an above or below descendant");
    }

    auto next_left = left->get_left();

    while (next_left != nullptr) {
        std::cout << "...l\n";
        if (left_A_will_vanish && next_left->get_A() == old_left_A)
            next_left->set_A(A);
        if (left_B_will_vanish && next_left->get_B() == old_left_B)
            next_left->set_B(B);

        next_left = next_left->get_left();
    }

    auto next_right = right->get_right();

    while (next_right != nullptr) {
        std::cout << "...r\n";
        if (right_A_will_vanish && next_right->get_A() == old_right_A)
            next_right->set_A(A);
        if (right_B_will_vanish && next_right->get_B() == old_right_B)
            next_right->set_B(B);

        next_right = next_right->get_right();
    }

    new_node->set_v_1(left->get_v_1());
    new_node->set_v_2(right->get_v_2());
    new_node->set_e(e_cut);

    delete_node(left);
    // left = NULL;
    delete_node(right);
    // right = NULL;

    if (left_merge_neighbour != nullptr) {
        left_merge_neighbour->set_right(new_node);
        new_node->set_left(left_merge_neighbour);
    }

    if (right_merge_neighbour != nullptr) {
        right_merge_neighbour->set_left(new_node);
        new_node->set_right(right_merge_neighbour);
    }
    
    assert(new_node != nullptr);
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