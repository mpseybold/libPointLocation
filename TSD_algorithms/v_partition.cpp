#include "../TSD.h"

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_leaf_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut) {
    assert(node->get_A() == nullptr);
    assert(node->get_B() == nullptr);
    assert(node->get_L() == nullptr);
    assert(node->get_R() == nullptr);
    assert(node->is_leaf());

    if (node->is_flat()) {
        std::cout << "flat node!...\n";
    }

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(v_cut);
    auto L = new Node<PointType, OrderType>(pos_neg.second);
    auto R = new Node<PointType, OrderType>(pos_neg.first);

    L->set_leaf_right(R);
    R->set_leaf_left(L);

    node->set_v_1(v_cut);
    node->set_L(L);
    node->set_R(R);
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_V_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut) {
    assert(node != nullptr);

    if (node->is_flat()) {
        return;
    }

    assert(node->get_A() == nullptr);
    assert(node->get_B() == nullptr);
    assert(node->get_L() != nullptr);
    assert(node->get_R() != nullptr);
    assert(node->get_e().get_cut_type() == NO_CUT);
    assert(node->get_v_1().get_cut_type() != NO_CUT && node->get_v_1().get_cut_type() != EDGE);
    assert(node->get_v_2().get_cut_type() == NO_CUT);
    // std::vector<BoundingTrap<PointType, OrderType>> problems = std::vector<BoundingTrap<PointType, OrderType>>();
    // problems.push_back(node->get_trapezoid());
    // io::write_trapezoids(problems, "plotting/problematic_traps.dat");
    assert(v_cut.defining_point_cut_comparison(node->get_v_1()) == 1);

    auto r_trap = node->get_R()->get_trapezoid();
    auto pos_neg = r_trap.destroy(v_cut);

    auto R = new Node<PointType, OrderType>(pos_neg.first);
    auto A = new Node<PointType, OrderType>(pos_neg.second);

    v_partition(node->get_R(), v_cut);

    delete node->get_R();

    node->set_R(R);
    node->set_A(A);
    node->set_v_2(v_cut);
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_E_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut) {

    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_L() == nullptr);
    assert(node->get_R() == nullptr);
    assert(node->get_e().get_cut_type() == EDGE);
    assert(node->get_v_1().get_cut_type() == NO_CUT);
    assert(node->get_v_2().get_cut_type() == NO_CUT);

    BoundingTrap<PointType, OrderType> trap = node->get_trapezoid();
    int def_point_orientation = Cut<PointType, OrderType>::v_cut_edge_orientation(v_cut, node->get_e());
    auto pos_neg = trap.destroy(v_cut);
    auto pos_trap = pos_neg.first;
    auto neg_trap = pos_neg.second;
    Node<PointType, OrderType>* L = new Node<PointType, OrderType>(neg_trap);
    Node<PointType, OrderType>* R = new Node<PointType, OrderType>(pos_trap);
    
    L->set_e(node->get_e());
    R->set_e(node->get_e());

    switch(def_point_orientation) {
        case 1: {
            v_partition(node->get_A(), v_cut);

            L->set_A(node->get_A()->get_L());
            L->set_B(node->get_B());

            R->set_A(node->get_A()->get_R());
            R->set_B(node->get_B());

            delete node->get_A();
            break;
        }
        case -1: {
            v_partition(node->get_B(), v_cut);

            L->set_A(node->get_A());
            L->set_B(node->get_B()->get_L());

            R->set_A(node->get_A());
            R->set_B(node->get_B()->get_R());

            delete node->get_B();
            break;
        }
        case 0: {
            v_partition(node->get_A(), v_cut);
            v_partition(node->get_B(), v_cut);

            L->set_A(node->get_A()->get_L());
            L->set_B(node->get_B()->get_L());

            R->set_A(node->get_A()->get_R());
            R->set_B(node->get_B()->get_R());

            delete node->get_A();
            delete node->get_B();
            break;
        }
    }

    node->clear_e();
    node->set_v_1(v_cut);
    node->set_desc(L, R, nullptr, nullptr);
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_VE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut) {
    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_L() != nullptr);
    assert(node->get_R() == nullptr);
    assert(node->get_e().get_cut_type() == EDGE);
    assert(node->get_v_1().get_cut_type() != NO_CUT && node->get_v_1().get_cut_type() != EDGE);
    assert(node->get_v_2().get_cut_type() == NO_CUT);


    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(v_cut);
    Node<PointType, OrderType>* L;// = new Node<PointType, OrderType>(pos_neg.second);
    auto R = new Node<PointType, OrderType>(pos_neg.first);

    if (v_cut.defining_point_cut_comparison(node->get_v_1()) == 1) {
        L = new Node<PointType, OrderType>(pos_neg.second);
        
        L->set_e(node->get_e());
        L->set_v_1(node->get_v_1());
        R->set_e(node->get_e());

        L->set_L(node->get_L());

        int def_point_orientation = Cut<PointType, OrderType>::v_cut_edge_orientation(v_cut, node->get_e());
        switch(def_point_orientation) {
            case 1: {
                v_partition(node->get_A(), v_cut);

                R->set_A(node->get_A()->get_R());
                R->set_B(node->get_B());

                L->set_A(node->get_A()->get_L());
                L->set_B(node->get_B());
                
                delete node->get_A();
                break;
            }
            case -1: {
                v_partition(node->get_B(), v_cut);

                R->set_A(node->get_A());
                R->set_B(node->get_B()->get_R());

                L->set_A(node->get_A());
                L->set_B(node->get_B()->get_L());
                
                delete node->get_B();
                break;
            }
            case 0: {
                v_partition(node->get_A(), v_cut);
                v_partition(node->get_B(), v_cut);

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

        v_partition(node->get_L(), v_cut);

        L = node->get_L()->get_L();
        R->set_L(node->get_L()->get_R());
        R->set_A(node->get_A());
        R->set_B(node->get_B());

        R->set_e(node->get_e());
        R->set_v_1(node->get_v_1());

        delete node->get_L();
    }

    node->set_L(L);
    node->set_R(R);
    node->set_v_1(v_cut);
    node->clear_e();
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_EV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut) {
    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_L() == nullptr);
    assert(node->get_R() != nullptr);
    assert(node->get_e().get_cut_type() == EDGE);
    assert(node->get_v_2().get_cut_type() != NO_CUT && node->get_v_2().get_cut_type() != EDGE);
    assert(node->get_v_1().get_cut_type() == NO_CUT);

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(v_cut);

    auto L = new Node<PointType, OrderType>(pos_neg.second);
    Node<PointType, OrderType>* R;

    if (v_cut.defining_point_cut_comparison(node->get_v_2()) == -1) {
        R = new Node<PointType, OrderType>(pos_neg.first);

        L->set_e(node->get_e());
        R->set_e(node->get_e());
        R->set_v_2(node->get_v_2());
        R->set_R(node->get_R());

        int def_point_orientation = Cut<PointType, OrderType>::v_cut_edge_orientation(v_cut, node->get_e());
        switch(def_point_orientation) {
            case 1: {
                v_partition(node->get_A(), v_cut);

                L->set_A(node->get_A()->get_L());
                L->set_B(node->get_B());

                R->set_A(node->get_A()->get_R());
                R->set_B(node->get_B());

                delete node->get_A();
                break;
            }
            case -1: {
                v_partition(node->get_B(), v_cut);

                L->set_A(node->get_A());
                L->set_B(node->get_B()->get_L());

                R->set_A(node->get_A());
                R->set_B(node->get_B()->get_R());

                delete node->get_B();
                break;
            }
            case 0: {
                v_partition(node->get_A(), v_cut);
                v_partition(node->get_B(), v_cut);

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
        v_partition(node->get_R(), v_cut);

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

    node->set_L(L);
    node->set_R(R);
    node->clear_e();
    node->clear_v_2();
    node->set_v_1(v_cut);
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_part_handle_VVE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut) {
    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_L() != nullptr);
    assert(node->get_R() != nullptr);
    assert(node->get_e().get_cut_type() == EDGE);
    assert(node->get_v_2().get_cut_type() != NO_CUT && node->get_v_2().get_cut_type() != EDGE);
    assert(node->get_v_1().get_cut_type() != NO_CUT && node->get_v_1().get_cut_type() != EDGE);

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(v_cut);

    Node<PointType, OrderType>* L; 
    Node<PointType, OrderType>* R;

    if (v_cut.defining_point_cut_comparison(node->get_v_1()) == -1) {
        R = new Node<PointType, OrderType>(pos_neg.first);
        v_partition(node->get_L(), v_cut);
        L = node->get_L()->get_L();

        R->set_desc(node->get_L()->get_R(), node->get_R(), node->get_A(), node->get_B());
        R->copy_cuts(node);

        delete node->get_L();
    } else if (v_cut.defining_point_cut_comparison(node->get_v_2()) == 1) {
        L = new Node<PointType, OrderType>(pos_neg.second);
        v_partition(node->get_R(), v_cut);
        R = node->get_R()->get_R();

        L->set_desc(node->get_L(), node->get_R()->get_L(), node->get_A(), node->get_B());
        L->copy_cuts(node);
    
        delete node->get_R();
    } else {
        L = new Node<PointType, OrderType>(pos_neg.second);
        R = new Node<PointType, OrderType>(pos_neg.first);

        int def_point_orientation = Cut<PointType, OrderType>::v_cut_edge_orientation(v_cut, node->get_e());

        switch(def_point_orientation) {
            case 1: {
                v_partition(node->get_A(), v_cut);
                L->set_desc(node->get_L(), nullptr, node->get_A()->get_L(), node->get_B());
                R->set_desc(nullptr, node->get_R(), node->get_A()->get_R(), node->get_B());

                delete node->get_A();
                break;
            }
            case -1: {
                v_partition(node->get_B(), v_cut);
                L->set_desc(node->get_L(), nullptr, node->get_A(), node->get_B()->get_L());
                R->set_desc(nullptr, node->get_R(), node->get_A(), node->get_B()->get_R());
                
                delete node->get_B();
                break;
            }
            case 0: {
                v_partition(node->get_A(), v_cut);
                v_partition(node->get_B(), v_cut);
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
void TSD<PointType, OrderType>::v_partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut) {
    DestructionPattern pattern = node->get_dest_pattern();


    switch(pattern) {
        case E:
            v_part_handle_E_case(node, v_cut);
            break;
        case VE:
            v_part_handle_VE_case(node, v_cut);
            break;
        case V:
            v_part_handle_V_case(node, v_cut);
            break;
        case VVE:
            v_part_handle_VVE_case(node, v_cut);
            break;
        case NO_DESTRUCTION:
            v_part_handle_leaf_case(node, v_cut);
            break;
    }
}