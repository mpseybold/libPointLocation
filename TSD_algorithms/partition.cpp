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


// Walk left/right from 'node'
// and reassign either the A or B descendant to 'new_desc'
// to all merge neighbours which are parents of 'old_desc'
template <class PointType , class OrderType>
void TSD<PointType, OrderType>::walk_to_fix_desc(Node<PointType, OrderType>* node, Node<PointType, OrderType>* new_desc, Node<PointType, OrderType>* old_desc) {
    
    auto next = node->get_right();

    assert(new_desc != nullptr);

    bool A_match; 
    bool B_match;

    if (next != nullptr) {
        A_match = next->get_A() == old_desc;
        B_match = next->get_B() == old_desc;
    }

    while (next != nullptr && (A_match || B_match)) {
        if (A_match)
            next->set_A(new_desc);
        if (B_match)
            next->set_B(new_desc);

        next = next->get_right();
        if (next != nullptr) {
            A_match = next->get_A() == old_desc;
            B_match = next->get_B() == old_desc;
        }
    }

    next = node->get_left();

    if (next != nullptr) {
        A_match = next->get_A() == old_desc;
        B_match = next->get_B() == old_desc;
    }

    while (next != nullptr && (A_match || B_match)) {
        if (A_match)
            next->set_A(new_desc);
        if (B_match)
            next->set_B(new_desc);

        next = next->get_left();
        if (next != nullptr) {
            A_match = next->get_A() == old_desc;
            B_match = next->get_B() == old_desc;
        }
    }
}



template <class PointType, class OrderType>
void TSD<PointType, OrderType>::partition_E_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut, Node<PointType, OrderType>* parent) {
    assert(node != nullptr);
    assert(e_cut->get_cut_type() == EDGE);
    assert(node->get_A() != nullptr);
    assert(node->get_B() != nullptr);
    assert(node->get_e()->get_cut_type() == EDGE);

    // auto trap = node->get_trapezoid();
    // auto pos_neg = trap.destroy(e_cut);

    bool intersects_A = node->get_A()
    ->get_trapezoid()
    .intersects_segment(e_cut->get_segment());

    bool intersects_B = node->get_B()
    ->get_trapezoid()
    .intersects_segment(e_cut->get_segment());

    assert(intersects_A || intersects_B);
    bool l_visited = false;

    if (!(intersects_A && intersects_B)) {
        std::vector<Node<PointType, OrderType>*> nodes {node};

        auto node_to_be_split = intersects_A ? node->get_A() : node->get_B();

        // populate nodes vector
        if (node->get_right() != nullptr) {

            auto next = node->get_right();
            
            while (next != nullptr) {
                bool add_to_list = intersects_A ? node_to_be_split == next->get_A()
                : node_to_be_split == next->get_B();
                
                if (add_to_list)
                    nodes.push_back(next);
                
                next = next->get_right();
            }
        }

        // if (nodes.size() > 1) {
        //     std::cout << "list length: " << nodes.size() << std::endl;
        //     std::cout << "hello\n";
        // }

        auto pattern = node->get_dest_pattern();

        if (pattern == VE || pattern == VVE) {
            l_visited = (node->get_L()->get_priority() == e_cut->get_priority());
            partition(node->get_L(), e_cut, node);
        }

        if (l_visited)
            std::cout << "l_visited\n";
    
        partition(node_to_be_split, e_cut, node);

        auto old_e_cut = node->get_e();

        Node<PointType, OrderType>* new_A;
        Node<PointType, OrderType>* new_B;
        if (intersects_A)
            new_A = node_to_be_split->get_A();
        if (intersects_B)
            new_B = node_to_be_split->get_B();

        for (int i = 0; i < nodes.size(); ++i) {
            auto n = nodes[i];

            auto trap = n->get_trapezoid();
            auto pos_neg = trap.destroy(e_cut);

            auto old_L = n->get_L();
            auto old_R = n->get_R();

            pattern = n->get_dest_pattern();

            Node<PointType, OrderType>* new_node;
            if (!intersects_A)
                new_A = new_node;
            if (!intersects_B)
                new_B = new_node;

            if (intersects_A) {
                new_B = new Node<PointType, OrderType>(pos_neg.second);
                new_B->set_A(node_to_be_split->get_B());
                new_B->set_B(n->get_B());
                new_B->set_e(old_e_cut);
            } else {
                new_A = new Node<PointType, OrderType>(pos_neg.first);
                new_A->set_A(n->get_A());
                new_A->set_B(node_to_be_split->get_A());
                new_A->set_e(old_e_cut);
            }

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
    

                // if (e_cut->get_priority() == 23) {
                //     if (node->get_R() != nullptr && node->get_R()->get_priority() == 99)
                //         std::cout << "hello\n";
                //     if (node->get_L() != nullptr && node->get_L()->get_priority() == 99)
                //         std::cout << "hello\n";
                // }

                if (pattern == EV || pattern == VVE) 
                    partition(n->get_R(), e_cut, n);
                if (intersects_A)
                    new_A = node_to_be_split->get_A();
                if (intersects_B)
                    new_B = node_to_be_split->get_B();
                // else if (pattern == VE) {
                //     l_visited = (n->get_L()->get_priority() == e_cut->get_priority());
                //     partition(n->get_L(), e_cut);
                // } else {
                //     l_visited = (n->get_L()->get_priority() == e_cut->get_priority()); 
                //     partition(n->get_L(), e_cut);
                //     partition(n->get_R(), e_cut);
                // }


                if (intersects_A) {    
                    auto old_desc = new_A;
                    if (pattern == EV) {
                        if (e_cut->get_priority() == 8) {
                        std::vector<BoundingTrap<PointType, OrderType>> traps = {
                            n->get_A()->get_trapezoid(), n->get_B()->get_trapezoid(), 
                            /*n->get_L()->get_trapezoid(),*/ n->get_R()->get_trapezoid()};
                        io::write_trapezoids(traps, "v_merge.dat");
                        }
                        auto old_desc_R = n->get_R()->get_A();
                        new_A = v_merge(new_A, n->get_R()->get_A());
                        new_B->set_R(n->get_R()->get_B());
                        new_B->set_v_2(n->get_v_2());

                        walk_to_fix_desc(n->get_R(), new_A, old_desc_R);
                        walk_to_fix_desc(n->get_A(), new_A, old_desc);
                        if (parent != nullptr) {
                            walk_to_fix_desc(parent, new_A, old_desc_R);
                            walk_to_fix_desc(parent, new_A, old_desc);
                        }
                        // if (n->get_B()->get_right() != nullptr) {
                        //     std::cout << "vveR\n";
                        //     n->get_B()->get_right()->set_left(new_B->get_B());
                        //     new_B->get_B()->set_right(n->get_B()->get_right());
                        // }
                    } else if (pattern == VE) {   

                        auto old_desc_L = n->get_L()->get_A();
                        new_A = v_merge(n->get_L()->get_A(), new_A);
                        new_B->set_L(n->get_L()->get_B());
                        new_B->set_v_1(n->get_v_1());

                        walk_to_fix_desc(n->get_L(), new_A, old_desc_L);
                        walk_to_fix_desc(n->get_A(), new_A, old_desc);
                        if (parent != nullptr) {
                            walk_to_fix_desc(parent, new_A, old_desc_L);
                            walk_to_fix_desc(parent, new_A, old_desc);
                        }
                        // if (n->get_B()->get_left() != nullptr) {
                        //     std::cout << "vveR\n";
                        //     n->get_B()->get_left()->set_right(new_B->get_B());
                        //     new_B->get_B()->set_left(n->get_B()->get_left());
                        // }
                    } else {
                        
                        auto old_desc_L = n->get_L()->get_A();
                        auto old_desc_R = n->get_R()->get_A();
                        auto old_desc = new_A; 

                        auto aux = v_merge(n->get_L()->get_A(), new_A);
                        new_A = v_merge(aux, n->get_R()->get_A());
                        new_B->set_R(n->get_R()->get_B());
                        new_B->set_v_2(n->get_v_2());
                        new_B->set_L(n->get_L()->get_B());
                        new_B->set_v_1(n->get_v_1());

                        walk_to_fix_desc(n->get_L(), new_A, old_desc_L);
                        walk_to_fix_desc(n->get_R(), new_A, old_desc_R);
                        walk_to_fix_desc(n->get_A(), new_A, old_desc);
                        if (parent != nullptr) {
                            walk_to_fix_desc(parent, new_A, old_desc_L);
                            walk_to_fix_desc(parent, new_A, old_desc_R);
                            walk_to_fix_desc(parent, new_A, old_desc);
                        }
                        // if (n->get_B()->get_right() != nullptr) {
                        //     std::cout << "vveR\n";
                        //     n->get_B()->get_right()->set_left(new_B->get_B());
                        //     new_B->get_B()->set_right(n->get_B()->get_right());
                        // }
                        // if (n->get_B()->get_left() != nullptr) {
                        //     std::cout << "vveR\n";
                        //     n->get_B()->get_left()->set_right(new_B->get_B());
                        //     new_B->get_B()->set_left(n->get_B()->get_left());
                        // }
                    }

                } else {
                    auto old_desc = new_B;
                    if (pattern == EV) {
                        auto old_desc_R = n->get_R()->get_B();
                        new_B = v_merge(new_B, n->get_R()->get_B());
                        new_A->set_R(n->get_R()->get_A());
                        new_A->set_v_2(n->get_v_2());

                        walk_to_fix_desc(n->get_R(), new_B, old_desc_R);
                        walk_to_fix_desc(n->get_B(), new_B, old_desc);
                        if (parent != nullptr) {
                            walk_to_fix_desc(parent, new_B, old_desc_R);
                            walk_to_fix_desc(parent, new_B, old_desc);
                        }
                        // if (n->get_A()->get_right() != nullptr) {
                        //     std::cout << "vveR\n";
                        //     n->get_A()->get_right()->set_left(new_A->get_A());
                        //     new_A->get_A()->set_right(n->get_A()->get_right());
                        // }
                    } else if (pattern == VE) {
                        auto old_desc_L = n->get_L()->get_B();
                        new_B = v_merge(n->get_L()->get_B(), new_B);
                        new_A->set_L(n->get_L()->get_A());
                        new_A->set_v_1(n->get_v_1());

                        walk_to_fix_desc(n->get_L(), new_B, old_desc_L);
                        walk_to_fix_desc(n->get_B(), new_B, old_desc);
                        if (parent != nullptr) {
                            walk_to_fix_desc(parent, new_B, old_desc);
                            walk_to_fix_desc(parent, new_B, old_desc_L);
                        }
                        // if (n->get_A()->get_left() != nullptr) {
                        //     std::cout << "vveR\n";
                        //     n->get_A()->get_left()->set_right(new_A->get_A());
                        //     new_A->get_A()->set_left(n->get_A()->get_left());
                        // }
                    } else { 

                        auto old_desc_L = n->get_L()->get_B();
                        auto old_desc_R = n->get_R()->get_B();
                        auto old_desc = new_B;

                        auto aux = v_merge(n->get_L()->get_B(), new_B);
                        new_B = v_merge(aux, n->get_R()->get_B());
                        new_A->set_R(n->get_R()->get_A());
                        new_A->set_v_2(n->get_v_2());
                        new_A->set_L(n->get_L()->get_A());
                        new_A->set_v_1(n->get_v_1());

                        walk_to_fix_desc(n->get_L(), new_B, old_desc_L);
                        walk_to_fix_desc(n->get_R(), new_B, old_desc_R);
                        walk_to_fix_desc(n->get_B(), new_B, old_desc);
                        if (parent != nullptr) {
                            walk_to_fix_desc(parent, new_B, old_desc_L);
                            walk_to_fix_desc(parent, new_B, old_desc_R);
                            walk_to_fix_desc(parent, new_B, old_desc);
                        }
                        // if (n->get_A()->get_right() != nullptr) {
                        //     std::cout << "vveR\n";
                        //     n->get_A()->get_right()->set_left(new_A->get_A());
                        //     new_A->get_A()->set_right(n->get_A()->get_right());
                        // }
                        // if (n->get_A()->get_left() != nullptr) {
                        //     std::cout << "vveR\n";
                        //     n->get_A()->get_left()->set_right(new_A->get_A());
                        //     new_A->get_A()->set_left(n->get_A()->get_left());
                        // }
                    }
                }                    
            }

            // Fixing left boundaires if l_visited

            if ((pattern == VE || pattern == VVE) && l_visited) {
                auto old_L_A_left = n->get_L()->get_A()->get_trapezoid().get_v_left();
                auto old_L_B_left = n->get_L()->get_B()->get_trapezoid().get_v_left();

                auto old_L_A_left_side = n->get_L()->get_A()->get_trapezoid().get_left_side();
                auto old_L_B_left_side = n->get_L()->get_B()->get_trapezoid().get_left_side();
            
                if (n->get_trapezoid().get_v_left() != old_L_A_left) {
                    new_A->get_trapezoid().set_left(old_L_A_left,
                    old_L_A_left_side);
                }

                if (n->get_trapezoid().get_v_left() != old_L_B_left) {
                    new_B->get_trapezoid().set_left(old_L_B_left,
                    old_L_B_left_side);
                }
            }

            n->set_A(new_A);
            n->set_B(new_B);
            n->set_L(nullptr);
            n->set_R(nullptr);
            n->set_e(e_cut);
            n->clear_v_1();
            n->clear_v_2();

            if (n != node && n != nodes[nodes.size() - 1]) {
                auto prev = nodes[i-1];
                if (intersects_A) {
                    prev->get_B()->set_right(n->get_B());
                    n->get_B()->set_left(prev->get_B());
                } else {
                    prev->get_A()->set_right(n->get_A());
                    n->get_A()->set_left(prev->get_A());
                }
                // prev->set_right(nullptr);
                // n->set_left(nullptr);
            } else if (n == node) {
                if (n->get_left() != nullptr) {
                    auto prev = nodes[i-1];
                    if (intersects_A) {
                        n->get_B()->set_left(n->get_left());
                        n->get_left()->set_right(n->get_B());
                    } else {
                        n->get_A()->set_left(n->get_left());
                        n->get_left()->set_right(n->get_A());
                    }
                    n->set_left(nullptr);
                }
            }

            // if ((pattern == VE || pattern == VVE) && l_visited) {
            //     assert(old_L->get_left() != nullptr);

            //     if (intersects_A) {
            //         old_L->get_left()->get_B()->set_right(old_L->get_B());
            //         old_L->get_B()->set_left(old_L->get_left()->get_B());
            //         old_L->get_left()->set_right(nullptr);
            //         old_L->get_left()->set_A(new_A);
            //         old_L->set_left(nullptr);
            //     } else {
            //         old_L->get_left()->get_A()->set_right(old_L->get_A());
            //         old_L->get_A()->set_left(old_L->get_left()->get_A());
            //         old_L->get_left()->set_right(nullptr);
            //         old_L->get_left()->set_B(new_B);
            //         old_L->set_left(nullptr);
            //     }
            // }

            if (n == nodes[nodes.size() - 1]) {
                
                if (n != node) {
                    auto prev = nodes[i-1];
                    if (intersects_A) {
                        prev->get_B()->set_right(n->get_B());
                        n->get_B()->set_left(prev->get_B());
                    } else {
                        prev->get_A()->set_right(n->get_A());
                        n->get_A()->set_left(prev->get_A());
                    }
                    // prev->set_right(nullptr);
                    // n->set_left(nullptr);
                }

                if (n->get_right() != nullptr) {
                // if (n->get_right() != nullptr
                // && n->get_trapezoid().get_right()->orientation(
                    // e_cut->get_segment()->get_target()
                // ) == 0) {
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

            if (pattern == EV)
                delete_node(old_R);
            if (pattern == VE)
                delete_node(old_L);
            if (pattern == VVE) {
                delete_node(old_L);
                delete_node(old_R);
            }

            // if (old_L != nullptr && is_reachable(root, old_L))
            //     assert(false);

            // if (old_R != nullptr && is_reachable(root, old_R))
            //     assert(false);

        }


        for (int i = 0; i < nodes.size(); ++i) {
            if (intersects_A) {
                nodes[i]->set_A(new_A);
            }
            if (intersects_B) {
                nodes[i]->set_B(new_B);
            }
        }

        delete_node(node_to_be_split);

        // visMe = asJsonGraph({root});
        // if (is_reachable(root, node_to_be_split))
        //     assert(false);
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

        partition(node->get_L(), e_cut, node);

        auto old_L = node->get_L();

        node->set_desc(nullptr, node->get_R(), old_L->get_A(), old_L->get_B());

        node->set_v_2(node->get_v_1());
        node->clear_v_1();
        node->set_e(e_cut);
        if (node->get_R() == nullptr)
            std::cout << "oops";

        if (e_cut->get_priority() == 16)
            std::cout << "hello\n";

        if (old_L->get_left() != nullptr) {
            old_L->get_left()->set_right(node);
            node->set_left(old_L->get_right());
            old_L->set_left(nullptr);
        }

        delete_node(old_L);

    } else if (node->get_R()->get_trapezoid().intersects_segment(e_cut->get_segment()) 
    && !node->get_L()->get_trapezoid().intersects_segment(e_cut->get_segment())) {
    // } else {

        partition(node->get_R(), e_cut, node);

        auto old_R = node->get_R();

        node->set_desc(node->get_L(), nullptr, old_R->get_A(), old_R->get_B());
        node->set_e(e_cut);

        if (node->get_L() == nullptr)
            std::cout << "oops";

        if (old_R->get_right() != nullptr) {
            old_R->get_right()->set_left(node);
            node->set_right(old_R->get_right());
            old_R->set_right(nullptr);
        }

        delete_node(old_R);
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

    partition(old_A, e_cut, node);
    node->set_A(old_A->get_A());
    node->set_B(old_A->get_B());

    node->set_e(e_cut);
    delete_node(old_A);
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
void TSD<PointType, OrderType>::partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* cut, Node<PointType, OrderType>* parent) {
    //TODO: Implement this function.

    assert(node != nullptr);
    DestructionPattern pattern = node->get_dest_pattern();

    if (cut->get_priority() == 23 && node->get_trapezoid().get_bottom()->get_priority() == 99)
        std::cout << "hello\n";

    if (cut == node->get_e())
        return;


    switch(pattern) {
        case NO_DESTRUCTION: {
            partition_leaf_case(node, cut);
            visMe = asJsonGraph(subdag_roots);
            break;
        }
        case E: {
            partition_E_case(node, cut, parent);
            visMe = asJsonGraph(subdag_roots);
            break;
        }
        case VE: {
            partition_E_case(node, cut, parent);
            visMe = asJsonGraph(subdag_roots);
            break;
        }
        case EV: {
            partition_E_case(node, cut, parent);
            visMe = asJsonGraph(subdag_roots);
            break;
        }
        case VVE: {
            partition_E_case(node, cut, parent);
            visMe = asJsonGraph(subdag_roots);
            break;
        }
        case V: {
            partition_V_case(node, cut);
            visMe = asJsonGraph(subdag_roots);
            break;
        }
        case VV: {
            partition_VV_case(node, cut);
            visMe = asJsonGraph(subdag_roots);
            break;
        }
    }
}
