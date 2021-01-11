#include "../TSD.h"

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_leaf_case(Node<PointType, OrderType>* node, 
V_Cut<PointType, OrderType>* v_cut, int side) {
    assert(node->get_A() == nullptr);
    assert(node->get_B() == nullptr);
    assert(node->get_L() == nullptr);
    assert(node->get_R() == nullptr);
    assert(node->is_leaf());

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(v_cut, side);
    auto L = new Node<PointType, OrderType>(pos_neg.second);
    auto R = new Node<PointType, OrderType>(pos_neg.first);

    L->set_leaf_right(R);
    R->set_leaf_left(L);

    node->set_v_1(v_cut);
    node->set_L(L);
    node->set_R(R);

    leaf_count++;
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_V_case(Node<PointType, OrderType>* node, 
V_Cut<PointType, OrderType>* v_cut, int side) {
    assert(node != nullptr);

    if (node->is_flat() && node->get_v_1()->defining_point_cut_comparison(*v_cut) == 0) {
        return;
    }

    assert(node->get_A() == nullptr);
    assert(node->get_B() == nullptr);
    assert(node->get_L() != nullptr);
    assert(node->get_R() != nullptr);
    assert(node->get_e() == nullptr);
    assert(v_cut->defining_point_cut_comparison(*node->get_v_1()) == 1);

    assert(node->get_right() == nullptr);
    assert(node->get_left() == nullptr);

    

    auto r_trap = node->get_R()->get_trapezoid();
    auto pos_neg = r_trap.destroy(v_cut, side);

    auto R = new Node<PointType, OrderType>(pos_neg.first);
    auto A = new Node<PointType, OrderType>(pos_neg.second);

    v_partition(node->get_R(), v_cut, side);

    delete node->get_R();

    node->set_R(R);
    node->set_A(A);

    if (!(v_cut->defining_point_cut_comparison(*node->get_v_1()) == 1)) {
        node->set_v_2(node->get_v_1());
        node->set_v_1(v_cut);
    } else 
        node->set_v_2(v_cut);
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_E_case(Node<PointType, OrderType>* node, 
V_Cut<PointType, OrderType>* v_cut, int side) {

    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_L() == nullptr);
    assert(node->get_R() == nullptr);
    assert(node->get_e()->get_cut_type() == EDGE);
    // assert(node->get_v_1() != nullptr);
    // assert(node->get_v_2() != nullptr);


    int left_merge_side = 0;
    int right_merge_side = 0;

    if (node->get_left() != nullptr) {
        assert(node->get_left()->get_right() == node);
        left_merge_side = node->get_A() == node->get_left()->get_A() ?
        1 : -1;

        if (left_merge_side == -1) {
            assert(node->get_B() == node->get_left()->get_B());
        }
    }

    if (node->get_right() != nullptr) {
        assert(node->get_right()->get_left() == node);
        right_merge_side = node->get_A() == node->get_right()->get_A() ?
        1 : -1;

        if (right_merge_side == -1) {
            assert(node->get_B() == node->get_right()->get_B());
        }
    }

    BoundingTrap<PointType, OrderType> trap = node->get_trapezoid();
    int def_point_orientation = Cut<PointType, OrderType>::v_cut_edge_orientation(*v_cut->get_cut(side), *node->get_e());
    auto pos_neg = trap.destroy(v_cut->get_cut(side));
    auto pos_trap = pos_neg.first;
    auto neg_trap = pos_neg.second;
    Node<PointType, OrderType>* L = new Node<PointType, OrderType>(neg_trap);
    Node<PointType, OrderType>* R = new Node<PointType, OrderType>(pos_trap);
    
    L->set_e(node->get_e());
    R->set_e(node->get_e());

    switch(def_point_orientation) {
        case 1: {
            v_partition(node->get_A(), v_cut, side);

            L->set_A(node->get_A()->get_L());
            L->set_B(node->get_B());

            R->set_A(node->get_A()->get_R());
            R->set_B(node->get_B());

            L->set_right(R);
            R->set_left(L);

            delete node->get_A();
            break;
        }
        case -1: {
            v_partition(node->get_B(), v_cut, side);

            L->set_A(node->get_A());
            L->set_B(node->get_B()->get_L());

            R->set_A(node->get_A());
            R->set_B(node->get_B()->get_R());

            L->set_right(R);
            R->set_left(L);

            delete node->get_B();
            break;
        }
        case 0: {
            v_partition(node->get_A(), v_cut, side);
            v_partition(node->get_B(), v_cut, side);

            L->set_A(node->get_A()->get_L());
            L->set_B(node->get_B()->get_L());

            R->set_A(node->get_A()->get_R());
            R->set_B(node->get_B()->get_R());

            delete node->get_A();
            delete node->get_B();
            break;
        }
    }


    /*
    * We update the nodes merge pointers
    * and update the nodes in the corresponding linked lists
    */
    if (node->get_right() != nullptr) {
        node->get_right()->set_left(node->get_R());
        auto next = node->get_right();
        while (next != nullptr) {
            if (right_merge_side == 1) {
                next->set_A(node->get_R()->get_A());
            } else {
                next->set_B(node->get_R()->get_B());
            }

            next = next->get_right();
        }
        node->get_R()->set_right(node->get_right());
        node->set_right(nullptr);
    }

    if (node->get_left() != nullptr) {
        node->get_left()->set_right(node->get_L());
        auto next = node->get_left();
        while (next != nullptr) {
            if (left_merge_side == 1) {
                next->set_A(node->get_L()->get_A());
            } else {
                next->set_B(node->get_L()->get_B());
            }

            next = next->get_left();
        }
        node->get_L()->set_left(node->get_left());
        node->set_left(nullptr);
    }

    

    node->clear_e();
    node->set_v_1(v_cut);
    node->set_desc(L, R, nullptr, nullptr);
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_VE_case(Node<PointType, OrderType>* node, 
V_Cut<PointType, OrderType>* v_cut, int side) {
    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_L() != nullptr);
    assert(node->get_R() == nullptr);
    assert(node->get_e()->get_cut_type() == EDGE);
    // assert(node->get_v_1() != nullptr && node->get_v_1()->get_cut_type() != EDGE);
    assert(node->get_v_2() == nullptr);

    assert(node->get_left() == nullptr);

    int right_merge_side = 0;

    if (node->get_right() != nullptr) {
        assert(node->get_right()->get_left() == node);
        right_merge_side = node->get_A() == node->get_right()->get_A() ?
        1 : -1;

        if (right_merge_side == -1) {
            assert(node->get_B() == node->get_right()->get_B());
        }
    }

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(v_cut, side);
    Node<PointType, OrderType>* L;// = new Node<PointType, OrderType>(pos_neg.second);
    auto R = new Node<PointType, OrderType>(pos_neg.first);

    if (v_cut->defining_point_cut_comparison(*node->get_v_1()) == 1) {
        L = new Node<PointType, OrderType>(pos_neg.second);
        
        L->set_e(node->get_e());
        L->set_v_1(node->get_v_1());
        R->set_e(node->get_e());

        L->set_L(node->get_L());

        int def_point_orientation = Cut<PointType, OrderType>::v_cut_edge_orientation(*v_cut->get_cut(side), *node->get_e());
        switch(def_point_orientation) {
            case 1: {
                v_partition(node->get_A(), v_cut, side);

                R->set_A(node->get_A()->get_R());
                R->set_B(node->get_B());

                L->set_A(node->get_A()->get_L());
                L->set_B(node->get_B());
                
                L->set_right(R);
                R->set_left(L);

                delete node->get_A();
                break;
            }
            case -1: {
                v_partition(node->get_B(), v_cut, side);

                R->set_A(node->get_A());
                R->set_B(node->get_B()->get_R());

                L->set_A(node->get_A());
                L->set_B(node->get_B()->get_L());

                L->set_right(R);
                R->set_left(L);
                
                delete node->get_B();
                break;
            }
            case 0: {
                v_partition(node->get_A(), v_cut, side);
                v_partition(node->get_B(), v_cut, side);

                R->set_A(node->get_A()->get_R());
                R->set_B(node->get_B()->get_R());

                L->set_A(node->get_A()->get_L());
                L->set_B(node->get_B()->get_L());

                delete node->get_A();
                delete node->get_B();
                break;
            }
        }
    } else {
        
        v_partition(node->get_L(), v_cut, side);

        L = node->get_L()->get_L();
        R->set_L(node->get_L()->get_R());
        R->set_A(node->get_A());
        R->set_B(node->get_B());

        R->set_e(node->get_e());
        R->set_v_1(node->get_v_1());

        delete node->get_L();
    }

    /*
    * We update the nodes merge pointers
    * and update the nodes in the corresponding linked lists
    */
    if (node->get_right() != nullptr) {
        node->get_right()->set_left(R);
        auto next = node->get_right();
        while (next != nullptr) {
            if (right_merge_side == 1) {
                next->set_A(R->get_A());
            } else {
                next->set_B(R->get_B());
            }

            next = next->get_right();
        }
        R->set_right(node->get_right());
        node->set_right(nullptr);
    }

    node->set_L(L);
    node->set_R(R);
    node->set_v_1(v_cut);
    node->clear_e();
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_EV_case(Node<PointType, OrderType>* node, 
V_Cut<PointType, OrderType>* v_cut, int side) {
    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_L() == nullptr);
    assert(node->get_R() != nullptr);
    assert(node->get_e()->get_cut_type() == EDGE);
    assert(node->get_v_2() != nullptr && node->get_v_2()->get_cut_type() != EDGE);
    assert(node->get_v_1() == nullptr);

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(v_cut, side);

    auto L = new Node<PointType, OrderType>(pos_neg.second);
    Node<PointType, OrderType>* R;

    assert(node->get_right() == nullptr);

    int left_merge_side = 0;

    if (node->get_left() != nullptr) {
        assert(node->get_left()->get_right() == node);
        left_merge_side = node->get_A() == node->get_left()->get_A() ?
        1 : -1;

        if (left_merge_side == -1) {
            assert(node->get_B() == node->get_left()->get_B());
        }
    }

    if (v_cut->defining_point_cut_comparison(node->get_v_2()) == -1) {
        R = new Node<PointType, OrderType>(pos_neg.first);

        L->set_e(node->get_e());
        R->set_e(node->get_e());
        R->set_v_2(node->get_v_2());
        R->set_R(node->get_R());

        int def_point_orientation = Cut<PointType, OrderType>::v_cut_edge_orientation(*v_cut->get_cut(side), *node->get_e());
        switch(def_point_orientation) {
            case 1: {
                v_partition(node->get_A(), v_cut->get_cut(side));

                L->set_A(node->get_A()->get_L());
                L->set_B(node->get_B());

                R->set_A(node->get_A()->get_R());
                R->set_B(node->get_B());

                L->set_R(R);
                R->set_L(L);

                delete node->get_A();
                break;
            }
            case -1: {
                v_partition(node->get_B(), v_cut->get_cut(side));

                L->set_A(node->get_A());
                L->set_B(node->get_B()->get_L());

                R->set_A(node->get_A());
                R->set_B(node->get_B()->get_R());

                L->set_R(R);
                R->set_L(L);

                delete node->get_B();
                break;
            }
            case 0: {
                v_partition(node->get_A(), v_cut->get_cut(side));
                v_partition(node->get_B(), v_cut->get_cut(side));

                L->set_A(node->get_A()->get_L());
                L->set_B(node->get_B()->get_L());

                R->set_A(node->get_A()->get_R());
                R->set_B(node->get_B()->get_R());

                delete node->get_A();
                delete node->get_B();
                break;
            }
        }
    } else {
        v_partition(node->get_R(), v_cut->get_cut(side));

        R = node->get_R()->get_R();

        L->set_R(node->get_R()->get_L());
        L->set_A(node->get_A());
        L->set_B(node->get_B());

        L->set_e(node->get_e());
        L->set_v_2(node->get_v_2());

        delete node->get_R();
        node->set_v_1(v_cut);
        node->clear_e();
        node->clear_v_2();
    }

    if (node->get_left() != nullptr) {
        node->get_left()->set_right(node->get_L());
        auto next = node->get_left();
        while (next != nullptr) {
            if (left_merge_side == 1) {
                next->set_A(node->get_L()->get_A());
            } else {
                next->set_B(node->get_L()->get_B());
            }

            next = next->get_left();
        }
        node->get_L()->set_left(node->get_left());
        node->set_left(nullptr);
    }

    node->set_L(L);
    node->set_R(R);
    node->clear_e();
    node->clear_v_2();
    node->set_v_1(v_cut);
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_VVE_case(Node<PointType, OrderType>* node, 
V_Cut<PointType, OrderType>* v_cut, int side) {
    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_L() != nullptr);
    assert(node->get_R() != nullptr);
    assert(node->get_e()->get_cut_type() == EDGE);
    assert(node->get_left() == nullptr);
    assert(node->get_right() == nullptr);
    // assert(node->get_v_2() != nullptr && node->get_v_2()->get_cut_type() != EDGE);
    // assert(node->get_v_1() != nullptr && node->get_v_1()->get_cut_type() != EDGE);

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(v_cut->get_cut(side));

    Node<PointType, OrderType>* L; 
    Node<PointType, OrderType>* R;

    if (v_cut->defining_point_cut_comparison(*node->get_v_1()) == -1) {
        R = new Node<PointType, OrderType>(pos_neg.first);
        v_partition(node->get_L(), v_cut, side);
        L = node->get_L()->get_L();

        R->set_desc(node->get_L()->get_R(), node->get_R(), node->get_A(), node->get_B());
        R->copy_cuts(node);

        delete node->get_L();
    } else if (v_cut->defining_point_cut_comparison(*node->get_v_2()) == 1) {
        L = new Node<PointType, OrderType>(pos_neg.second);
        v_partition(node->get_R(), v_cut, side);
        R = node->get_R()->get_R();

        L->set_desc(node->get_L(), node->get_R()->get_L(), node->get_A(), node->get_B());
        L->copy_cuts(node);
    
        delete node->get_R();
    } else {
        L = new Node<PointType, OrderType>(pos_neg.second);
        R = new Node<PointType, OrderType>(pos_neg.first);

        int def_point_orientation = Cut<PointType, OrderType>::v_cut_edge_orientation(*v_cut->get_cut(side), *node->get_e());

        switch(def_point_orientation) {
            case 1: {
                v_partition(node->get_A(), v_cut, side);
                L->set_desc(node->get_L(), nullptr, node->get_A()->get_L(), node->get_B());
                R->set_desc(nullptr, node->get_R(), node->get_A()->get_R(), node->get_B());

                L->set_right(R);
                R->set_left(L);
                
                delete node->get_A();
                break;
            }
            case -1: {
                v_partition(node->get_B(), v_cut, side);
                L->set_desc(node->get_L(), nullptr, node->get_A(), node->get_B()->get_L());
                R->set_desc(nullptr, node->get_R(), node->get_A(), node->get_B()->get_R());
                
                L->set_right(R);
                R->set_left(L);

                delete node->get_B();
                break;
            }
            case 0: {
                v_partition(node->get_A(), v_cut, side);
                v_partition(node->get_B(), v_cut, side);
                L->set_desc(node->get_L(), nullptr, node->get_A()->get_L(), node->get_B()->get_L());
                R->set_desc(nullptr, node->get_R(), node->get_A()->get_R(), node->get_B()->get_R());
                
                delete node->get_A();
                delete node->get_B();
                break;
            }
        }

        L->set_e(node->get_e());
        L->set_v_1(node->get_v_1());

        R->set_e(node->get_e());
        R->set_v_2(node->get_v_2());
    }

    node->set_desc(L, R, nullptr, nullptr);
    node->clear_cuts();
    node->set_v_1(v_cut);
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_partition(Node<PointType, OrderType>* node, 
V_Cut<PointType, OrderType>* v_cut, int side) {
    DestructionPattern pattern = node->get_dest_pattern();

    switch(pattern) {
        case E:
            v_part_handle_E_case(node, v_cut, side);
            break;
        case VE:
            v_part_handle_VE_case(node, v_cut, side);
            break;
        case V:
            v_part_handle_V_case(node, v_cut, side);
            break;
        case VVE:
            v_part_handle_VVE_case(node, v_cut, side);
            break;
        case NO_DESTRUCTION:
            v_part_handle_leaf_case(node, v_cut, side);
            break;
    }
}