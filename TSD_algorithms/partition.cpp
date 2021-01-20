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
    assert(node != nullptr);
    assert(e_cut->get_cut_type() == EDGE);
    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_e()->get_cut_type() == EDGE);

    auto trap = node->get_trapezoid();
    auto pos_neg = trap.destroy(e_cut);

    bool intersects_A = node->get_A()
    ->get_trapezoid()
    .intersects_segment(e_cut->get_segment());

    bool intersects_B = node->get_B()
    ->get_trapezoid()
    .intersects_segment(e_cut->get_segment());

    assert(intersects_A || intersects_B);

    if (!(intersects_A && intersects_B)) {
        std::vector<Node<PointType, OrderType>*> nodes {node};

        auto node_to_be_split = intersects_A ? node->get_A() : node->get_B();

        // populate nodes vector
        if (node->get_right() != nullptr) {

            auto next = node->get_right();
            
            while (next->get_right() != nullptr) {
                bool add_to_list = intersects_A ? node_to_be_split == next->get_A()
                : node_to_be_split == next->get_B();
                
                if (add_to_list)
                    nodes.push_back(next);
                
                next = next->get_right();
            }
        }

        partition(node_to_be_split, e_cut);

        auto old_e_cut = node->get_e();

        Node<PointType, OrderType>* new_A;
        Node<PointType, OrderType>* new_B;
        if (intersects_A)
            new_A = node_to_be_split->get_A();
        if (intersects_B)
            new_B = node_to_be_split->get_B();

        for (int i = 0; i < nodes.size(); ++i) {
            auto n = nodes[i];
            Node<PointType, OrderType>* new_node;
            if (!intersects_A)
                new_A = new_node;
            if (!intersects_B)
                new_B = new_node;

            if (intersects_A) {
                new_B = new Node<PointType, OrderType>(pos_neg.second);
                new_B->set_A(node_to_be_split->get_B());
                new_B->set_B(node->get_B());
                new_B->set_e(old_e_cut);
            } else {
                new_A = new Node<PointType, OrderType>(pos_neg.first);
                new_A->set_A(node->get_A());
                new_A->set_B(node_to_be_split->get_A());
                new_A->set_e(old_e_cut);
            }

            auto pattern = n->get_dest_pattern();

            if (pattern != E) {
                if (pattern == EV) {
                    assert(n->get_R() != nullptr);
                    assert(n->get_v_2() != nullptr);
                } else if (pattern == VE) {
                    assert(n->get_L() != nullptr);
                    assert(n->get_v_1() != nullptr);
                } else {
                    assert(n->get_L() != nullptr);
                    assert(n->get_R() != nullptr);
                    assert(n->get_v_1() != nullptr);
                    assert(n->get_v_2() != nullptr);
                }
        
                if (pattern == EV)
                    partition(n->get_R(), e_cut);
                else if (pattern == VE)
                    partition(n->get_L(), e_cut);
                else {
                    partition(n->get_L(), e_cut);
                    partition(n->get_R(), e_cut);
                }

                if (intersects_A) {    

                    if (pattern == EV) {
                        if (e_cut->get_priority() == 8) {
                        std::vector<BoundingTrap<PointType, OrderType>> traps = {
                            n->get_A()->get_trapezoid(), n->get_B()->get_trapezoid(), 
                            /*n->get_L()->get_trapezoid(),*/ n->get_R()->get_trapezoid()};
                        io::write_trapezoids(traps, "v_merge.dat");
                        }
                        new_A = v_merge(new_A, n->get_R()->get_A());
                        new_B->set_R(n->get_R()->get_B());
                        new_B->set_v_2(n->get_v_2());
                    } else if (pattern == VE) {
                        new_A = v_merge(n->get_L()->get_A(), new_A);
                        new_B->set_L(n->get_L()->get_B());
                        new_B->set_v_1(n->get_v_1());
                    } else {
                        auto aux = v_merge(n->get_L()->get_A(), new_A);
                        new_A = v_merge(aux, n->get_R()->get_A());
                        new_B->set_R(n->get_R()->get_B());
                        new_B->set_v_2(n->get_v_2());
                        new_B->set_L(n->get_L()->get_B());
                        new_B->set_v_1(n->get_v_1());
                    }

                    
                } else {
            
                    if (pattern == EV) {
                        new_B = v_merge(new_B, n->get_R()->get_B());
                        new_A->set_R(n->get_R()->get_A());
                        new_A->set_v_2(n->get_v_2());
                    } else if (pattern == VE) {
                        new_B = v_merge(n->get_L()->get_B(), new_B);
                        new_A->set_L(n->get_L()->get_A());
                        new_A->set_v_2(n->get_v_1());
                    } else {
                        auto aux = v_merge(n->get_L()->get_B(), new_B);
                        new_B = v_merge(aux, n->get_R()->get_B());
                        new_A->set_R(n->get_R()->get_A());
                        new_A->set_v_2(n->get_v_2());
                        new_A->set_L(n->get_L()->get_A());
                        new_A->set_v_1(n->get_v_1());
                    }
                }

                if (pattern == EV)
                    delete n->get_R();
                else
                    delete n->get_L();
                    
            }

            n->set_A(new_A);
            n->set_B(new_B);
            n->set_L(nullptr);
            n->set_R(nullptr);
            n->set_e(e_cut);
            n->clear_v_1();
            n->clear_v_2();

            if (n != node) {
                auto prev = nodes[i-1];
                if (intersects_A) {
                    prev->get_B()->set_right(n->get_B());
                    n->get_B()->set_left(prev->get_B());
                } else {
                    prev->get_A()->set_right(n->get_A());
                    n->get_A()->set_left(prev->get_A());
                }
            } else {
                if (n->get_left() != nullptr) {
                    auto prev = nodes[i-1];
                    if (intersects_A) {
                        n->get_B()->set_left(n->get_left());
                        n->get_left()->set_right(n->get_B());
                        n->set_left(nullptr);
                    } else {
                        n->get_A()->set_left(n->get_left());
                        n->get_left()->set_right(n->get_A());
                        n->set_left(nullptr);
                    }
                }
            }

            if (n == nodes[nodes.size() - 1] && n->get_right() != nullptr) {
                if (intersects_A) {
                    n->get_B()->set_right(n->get_right());
                    n->get_right()->set_left(n->get_B());
                    n->set_right(nullptr);
                } else {
                    n->get_A()->set_right(n->get_right());
                    n->get_right()->set_left(n->get_A());
                    n->set_right(nullptr);
                }
            }
            
        }

        delete node_to_be_split;
    }
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_V_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut) {
    assert(e_cut != nullptr);
    assert(e_cut->get_cut_type() == EDGE);
    assert(node->get_L() != nullptr);
    assert(node->get_R() != nullptr);
    if (node->get_A() != nullptr) {
        std::cout << "hello\n";
    }
    assert(node->get_A() == nullptr);
    assert(node->get_B() == nullptr);
    assert(node->get_e() == nullptr);
    // assert(node->get_v_1() != nullptr && node->get_v_1()->get_cut_type() != EDGE);
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
        if (node->get_R() == nullptr)
            std::cout << "oops";

        delete old_L;

    } else if (node->get_R()->get_trapezoid().intersects_segment(e_cut->get_segment()) 
    && !node->get_L()->get_trapezoid().intersects_segment(e_cut->get_segment())) {
    // } else {

        partition(node->get_R(), e_cut);

        auto old_R = node->get_R();

        node->set_desc(node->get_L(), nullptr, old_R->get_A(), old_R->get_B());
        node->set_e(e_cut);

        if (node->get_L() == nullptr)
            std::cout << "oops";

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
    // assert(node->get_v_1() != nullptr && node->get_v_1()->get_cut_type() != EDGE);
    // assert(node->get_v_2() != nullptr && node->get_v_2()->get_cut_type() != EDGE);
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
    
    // assert(node->get_A() !=  nullptr);
    // assert(node->get_B() != nullptr);
    // assert(node->get_R() != nullptr);
    // assert(node->get_left() == nullptr);

    // bool intersects_L = node->get_L() != nullptr &&
    // node->get_L()->get_trapezoid().intersects_segment(e_cut->get_segment());
    // bool intersects_R = node->get_R() != nullptr &&
    // node->get_R()->get_trapezoid().intersects_segment(e_cut->get_segment());
    // bool intersects_A = node->get_A()->get_trapezoid().intersects_segment(e_cut->get_segment());
    // bool intersects_B = node->get_B()->get_trapezoid().intersects_segment(e_cut->get_segment());

    // auto trap = node->get_trapezoid();
    // auto pos_neg = trap.destroy(e_cut);

    // bool intersects_A = node->get_A();

    // bool intersects_A = node->get_A()
    // ->get_trapezoid()
    // .intersects_segment(e_cut->get_segment());

    // bool intersects_B = node->get_B()
    // ->get_trapezoid()
    // .intersects_segment(e_cut->get_segment());

    // assert(intersects_A || intersects_B);
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

    if (cut == node->get_e())
        return;


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
            partition_E_case(node, cut);
            break;
        }
        case EV: {
            partition_E_case(node, cut);
            break;
        }
        case VVE: {
            partition_E_case(node, cut);
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
