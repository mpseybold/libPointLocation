#include "../TSD.h"
#include "../io.h"


template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_leaf_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut) {
    assert(node != nullptr);
    assert(e_cut != nullptr);
    assert(e_cut->get_cut_type() == EDGE);

    if (node->is_flat())
        std::cout << "edge part flat\n";

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(e_cut);
    auto A = new Node<PointType, OrderType>(pos_neg.first);
    auto B = new Node<PointType, OrderType>(pos_neg.second);

    A->set_leaf_below(B);
    B->set_leaf_above(A);

    node->set_A(A);
    node->set_B(B);
    node->set_e(e_cut);
    node->toggle_partition_visited();
    partition_visited_nodes.push_back(node);

    leaf_count++;
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_E_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut) {
    // assert(node != nullptr);
    // assert(e_cut.get_cut_type() == EDGE);
    // assert(node->get_A() != nullptr);
    // assert(node->get_B() != nullptr);
    // assert(node->get_e().get_cut_type() == EDGE);

    // auto trap = node->get_trapezoid();
    // auto pos_neg = trap.destroy(e_cut);
    // auto A = new Node<PointType, OrderType>(pos_neg.first);
    // auto B = new Node<PointType, OrderType>(pos_neg.second);

    // bool intersects_A = node->get_A()
    // ->get_trapezoid()
    // .intersects_segment(e_cut.get_segment());

    // bool intersects_B = node->get_B()
    // ->get_trapezoid()
    // .intersects_segment(e_cut.get_segment());

    // assert(intersects_A || intersects_B);

    // auto left_cut = trap.get_left().get_cut_type() == NO_CUT
    // ? Cut<PointType, OrderType>(INTERSECTION, trap.get_top(), trap.get_bottom())
    // : trap.get_left();

    // auto right_cut = trap.get_right().get_cut_type() == NO_CUT
    // ? Cut<PointType, OrderType>(INTERSECTION, trap.get_top(), trap.get_bottom())
    // : trap.get_right();

    // if (intersects_B && intersects_A) {
    //     auto intersection_cut = Cut<PointType, OrderType>(
    //         INTERSECTION, node->get_e(), e_cut
    //     );

    //     if (trap.get_left().defining_point_cut_comparison(intersection_cut) == 1
    //     &&  trap.get_right().defining_point_cut_comparison(intersection_cut) == -1) {
            
    //         if (!node->get_A()->is_partition_visited()) {
    //             v_partition(node->get_A(), intersection_cut);
    //             node->get_A()->toggle_partition_visited();
    //             partition_visited_nodes.push_back(node->get_A());
    //         }

    //         if (!node->get_B()->is_partition_visited()) {
    //             v_partition(node->get_B(), intersection_cut);
    //             node->get_B()->toggle_intersection();
    //             partition_visited_nodes.push_back(node->get_B());
    //         }

    //         int slope_comparison = Segment<PointType, OrderType>::slope_comparison(e_cut, node->get_e());

    //         assert(slope_comparison != 0);

    //         switch(slope_comparison) {
    //             case -1: {
    //                 partition(node->get_B()->get_L(), e_cut);
    //                 partition(node->get_A()->get_R(), e_cut);

    //                 A->set_desc(nullptr, node->get_A()->get_R()->get_A(), 
    //                 node->get_A()->get_L()->get_A(), node->get_B()->get_L()->get_A());
    //                 A->set_v_2(intersection_cut);
    //                 A->set_e(e_cut);

    //                 B->set_desc(node->get_B()->get_L()->get_B(), nullptr, 
    //                 node->get_A()->get_R()->get_B(), node->get_B()->get_R());
    //                 B->set_v_1(intersection_cut);
    //                 B->set_e(e_cut);

    //                 break;
    //             }
    //             case 1: {
    //                 partition(node->get_B()->get_L(), e_cut);
    //                 partition(node->get_A()->get_R(), e_cut);

    //                 A->set_desc(nullptr, node->get_A()->get_R()->get_A(), 
    //                 node->get_A()->get_L()->get_A(), node->get_B()->get_L()->get_A());
    //                 A->set_v_2(intersection_cut);
    //                 A->set_e(e_cut);

    //                 B->set_desc(node->get_B()->get_L()->get_B(), nullptr, 
    //                 node->get_A()->get_R()->get_B(), node->get_B()->get_R());
    //                 B->set_v_1(intersection_cut);
    //                 B->set_e(e_cut);

    //                 break;
    //             }

                
    //         }

    //     } else if (trap.get_right().defining_point_cut_comparison(intersection_cut) == 1) {
            
    //     } else {

    //     }
        

    // } else if(intersects_B) {
        
    // } else if(intersects_A) {

    // }
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_V_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut) {
    assert(e_cut != nullptr);
    assert(e_cut->get_cut_type() == EDGE);
    assert(node->get_L() != nullptr);
    assert(node->get_R() != nullptr);
    assert(node->get_A() == nullptr);
    assert(node->get_B() == nullptr);
    assert(node->get_e() == nullptr);
    assert(node->get_v_1() != nullptr && node->get_v_1()->get_cut_type() != EDGE);
    assert(node->get_v_2() == nullptr);


    if (node->get_L()->get_trapezoid().intersects_segment(e_cut->get_segment()) 
    && !node->get_R()->get_trapezoid().intersects_segment(e_cut->get_segment())) {
    // if (node->get_v_1().orientation(e_cut.get_segment()->get_target()) < 1) {

        partition(node->get_L(), e_cut);

        auto old_L = node->get_L();

        node->set_desc(nullptr, node->get_R(), old_L->get_A(), old_L->get_B());

        node->set_v_2(node->get_v_1());
        node->clear_v_1();
        node->set_e(e_cut);

        delete old_L;

    } else if (node->get_R()->get_trapezoid().intersects_segment(e_cut->get_segment()) 
    && !node->get_L()->get_trapezoid().intersects_segment(e_cut->get_segment())) {
    // } else {

        partition(node->get_R(), e_cut);

        auto old_R = node->get_R();

        node->set_desc(node->get_L(), nullptr, old_R->get_A(), old_R->get_B());

        node->set_e(e_cut);

        delete old_R;
    } else {
        if (node->is_flat())
            return;
        if (node->get_R()->get_trapezoid().intersects_segment(e_cut->get_segment()))
            std::cout << "right\n";
        if (node->get_L()->get_trapezoid().intersects_segment(e_cut->get_segment()))
            std::cout << "left\n";
        assert(false);
    }
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_VV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut) {
    assert(e_cut != nullptr);
    assert(e_cut->get_cut_type() == EDGE);
    assert(node->get_L() != nullptr);
    assert(node->get_R() != nullptr);
    assert(node->get_A() != nullptr);
    assert(node->get_B() == nullptr);
    assert(node->get_e() == nullptr);
    assert(node->get_v_1() != nullptr && node->get_v_1()->get_cut_type() != EDGE);
    assert(node->get_v_2() != nullptr && node->get_v_2()->get_cut_type() != EDGE);
    if (!node->get_A()->get_trapezoid().intersects_segment(e_cut->get_segment()))
        assert(node->get_A()->get_trapezoid().intersects_segment(e_cut->get_segment()));

    auto old_A = node->get_A();

    partition(old_A, e_cut);
    node->set_A(old_A->get_A());
    node->set_B(old_A->get_B());

    node->set_e(e_cut);
    delete old_A;
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_EV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut) {

}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_VE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut) {

}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_VVE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut) {

}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* cut) {
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
            break;
        }
        case VV: {
            partition_VV_case(node, cut);
            break;
        }
        
    }
}
