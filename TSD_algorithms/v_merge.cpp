#include "../TSD.h"

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge_left_lower_priority_case(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right) {
    
    
    assert(left != nullptr);
    assert(right != nullptr);

    if (left->get_R() == nullptr)
        assert(left->get_R() != nullptr);
    assert(left->get_trapezoid().get_right() == right->get_trapezoid().get_left());


    auto new_trap = BoundingTrap<PointType, OrderType>::merge(left->get_trapezoid(), right->get_trapezoid());
    auto new_node = new Node<PointType, OrderType>(new_trap);

    auto right_merge_node = right->get_right();

    auto right_merge_neighbour = right->get_right();
    auto left_merge_neighbour = left->get_left();

    bool update_right_merge_pointer = right->get_priority() < left->get_R()->get_priority();

    auto R = v_merge(left->get_R(), right);

    new_node->set_desc(left->get_L(), R, left->get_A(), left->get_B());
    new_node->copy_cuts(left);

    // new_node->get_R()->set_right(right_merge_node);
    // if (right_merge_node != nullptr)
    //     right_merge_node->set_left(new_node->get_R());

    // assert(retired_nodes.find(left) == retired_nodes.end());
    delete_node(left);
    // left = NULL;
    // delete right;
    // right = NULL;

    if (right_merge_neighbour != nullptr && update_right_merge_pointer
    && right_merge_neighbour->get_priority() == new_node->get_R()->get_priority()) {
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
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge_right_lower_priority_case(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right) {
    assert(left != nullptr);
    assert(right != nullptr);
    assert(left->get_trapezoid().get_right() == right->get_trapezoid().get_left());

    if (right->get_priority() == 7)
        std::cout << "hello\n";

    
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
    

    if (left_merge_neighbour != nullptr && update_left_merge_pointer
    && left_merge_neighbour->get_priority() == new_node->get_L()->get_priority()) {
        left_merge_neighbour->set_right(new_node->get_L());
        new_node->get_L()->set_left(left_merge_neighbour);
    }

    if (right_merge_neighbour != nullptr) {
        right_merge_neighbour->set_left(new_node);
        new_node->set_right(right_merge_neighbour);
    }

    assert(new_node != nullptr);
    return new_node;
}

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge_equal_priority_case(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right) {
    
    assert(left != nullptr);
    assert(right != nullptr);
    if (left == right)
        return left;
    // if (left->get_trapezoid().get_right() != right->get_trapezoid().get_left())
        // std::cout << "hello\n";

    // if (!left->is_leaf())
    //     assert(left->get_trapezoid().get_right() == right->get_trapezoid().get_left());

    // std::cout << "v_merge_equal_priority\n";

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

    bool left_A_will_vanish = left->get_B() == right->get_B() && left->get_A() != right->get_A();
    bool left_B_will_vanish = left->get_A() == right->get_A() && left->get_B() != right->get_B();
    bool right_A_will_vanish = left->get_B() == right->get_B() && left->get_A() != right->get_A();
    bool right_B_will_vanish = left->get_A() == right->get_A() && left->get_B() != right->get_B();


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
        // std::cout << "...l\n";
        if (left_A_will_vanish && next_left->get_A() == old_left_A) {
            next_left->set_A(A);
        }
        if (left_B_will_vanish && next_left->get_B() == old_left_B) {
            next_left->set_B(B);
        }

        next_left = next_left->get_left();
    }

    auto next_right = right->get_right();

    while (next_right != nullptr) {
        // std::cout << "...r\n";
        if (right_A_will_vanish && next_right->get_A() == old_right_A) {
            next_right->set_A(A);
        }
        if (right_B_will_vanish && next_right->get_B() == old_right_B) {
            next_right->set_B(B);
        }

        next_right = next_right->get_right();
    }

    new_node->set_v_1(left->get_v_1());
    new_node->set_v_2(right->get_v_2());
    new_node->set_e(e_cut);

    // if (left != new_node)
    // assert(retired_nodes.find(left) == retired_nodes.end());
    delete_node(left);
    // left = NULL;
    // if (right != new_node)
    visMe = asJsonGraph({root});
    // assert(retired_nodes.find(right) == retired_nodes.end());
    // if (right_A_will_vanish && is_reachable(root, old_right_A)) 
    //     // assert(false);
    //     std::cout << "hello\n";
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

    // assert(!is_reachable(root, left));
    // assert(!is_reachable(root, right));
    
    assert(new_node != nullptr);
    return new_node;
}

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge_partial(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right) {
    if (left->get_priority() < right->get_priority()) {
        if (left->get_R() == right->get_L()) {
            left->get_trapezoid().set_right(
                right->get_trapezoid().get_v_right(),
                right->get_trapezoid().get_right_side()
            );
            left->set_R(right);
            return left;
        }

        // THIS IS A VERY CLUMSY WAY TO DEAL WITH
        // A CORNER CASE WHERE LEFT AND RIGHT OVERLAP
        // TODO: find cleaner solution
        if (left->get_R() == right) {
            // assert(false);
            left->get_trapezoid().set_right(
                right->get_trapezoid().get_v_right(), 
                right->get_trapezoid().get_right_side());

            return left;
        }

            Node<PointType, OrderType>* right_desc = left->get_R();
            while (right_desc != right && right_desc != nullptr) {
                std::cout << "right_desc...\n";
                // if (right_desc == nullptr)
                //     assert(false);
                right_desc = right_desc->get_R();
            }
            Node<PointType, OrderType>* left_desc = right->get_L();
            while (left_desc != left && left_desc != nullptr) {
                std::cout << "left_desc...\n";
                // if (left_desc == nullptr)
                    // assert(false);
                left_desc = left_desc->get_L();
            }

            if (right->get_L() == left && left->get_R() != nullptr) {
                right->set_L(left->get_R());
                auto new_right = v_merge(left->get_R(), right);
                left->get_trapezoid().set_right(
                    new_right->get_trapezoid().get_v_right(), 
                    new_right->get_trapezoid().get_right_side());

                return left;
            }

            left->get_trapezoid().set_right(
            right->get_trapezoid().get_v_right(), 
            right->get_trapezoid().get_right_side());

            if (right_desc == right) {
                right_desc = left->get_R();

                while (right_desc != right && right_desc != nullptr) {
                    right_desc->get_trapezoid().set_right(
                    right->get_trapezoid().get_v_right(), 
                    right->get_trapezoid().get_right_side());

                    right_desc = right_desc->get_R();
                }
            }

            if (left_desc == left) {
                auto left_old_R = left->get_R();
                left->set_R(right);
                right->set_left(left_old_R);
            } else if (right_desc == right) {
                std::cout << "good\n";
            } else {
                v_merge(left->get_R(), right);
                return left;
                assert(false);
            }
                // assert(false);

            return left;

    } else if (left->get_priority() > right->get_priority()) {
        if (right->get_L() == left->get_R()) {
            right->get_trapezoid().set_left(
                left->get_trapezoid().get_v_left(),
                left->get_trapezoid().get_left_side()
            );
            right->set_L(left);
            return right;
        }

        if (right->get_L() == left) {
            // assert(false);
            right->get_trapezoid().set_left(
                left->get_trapezoid().get_v_left(), 
                left->get_trapezoid().get_left_side());

            return right;
        }
            // assert(false);

            Node<PointType, OrderType>* right_desc = left->get_R();
            while (right_desc != right && right_desc != nullptr) {
                std::cout << "right_desc...\n";
                // if (right_desc == nullptr)
                //     assert(false);
                right_desc = right_desc->get_R();
            }
            Node<PointType, OrderType>* left_desc = right->get_L();
            while (left_desc != left && left_desc != nullptr) {
                std::cout << "left_desc...\n";
                // if (left_desc == nullptr)
                    // assert(false);
                left_desc = left_desc->get_L();
            }

            if (left->get_R() == right && right->get_L() != nullptr) {
                left->set_R(right->get_L());
                auto new_left = v_merge(left, right->get_L());
                right->get_trapezoid().set_left(
                    new_left->get_trapezoid().get_v_right(), 
                    new_left->get_trapezoid().get_right_side());

                return left;
            }

            right->get_trapezoid().set_left(
                left->get_trapezoid().get_v_left(), 
                left->get_trapezoid().get_left_side());

            if (left_desc == left) {
                left_desc = right->get_L();

                while (left_desc != left && left_desc != nullptr) {
                    left_desc->get_trapezoid().set_left(
                    left->get_trapezoid().get_v_left(), 
                    left->get_trapezoid().get_left_side());

                    left_desc = left_desc->get_L();
                }
            }

            if (right_desc == right) {
                auto right_old_L = right->get_L();
                right->set_L(left);
                left->set_right(right_old_L);
            }   else if (left_desc == left) {
                std::cout << "good\n";
            } else {
                v_merge(left, right->get_L());
                return right;
                assert(false);
            }
            return right;
        
    } else {
        return v_merge_equal_priority_case(left, right);
        assert(false);
    }

    return nullptr;
}


template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::v_merge(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right) {
    // TODO: Implement this function

    assert(left != nullptr);
    assert(right != nullptr);

    OrderType left_priority = left->get_priority();
    OrderType right_priority = right->get_priority();

    auto left_parents = left->get_parents();
    auto right_parents = right->get_parents();

    std::unordered_map<Node<PointType, OrderType>*, std::vector<Node<PointType, OrderType>*>>
    left_parent_desc = std::unordered_map<Node<PointType, OrderType>*, std::vector<Node<PointType, OrderType>*>>();

    std::unordered_map<Node<PointType, OrderType>*, std::vector<Node<PointType, OrderType>*>>
    right_parent_desc = std::unordered_map<Node<PointType, OrderType>*, std::vector<Node<PointType, OrderType>*>>();

    for (auto p: left_parents) {

        std::vector<Node<PointType, OrderType>*> desc{p->get_L(), p->get_R(), p->get_A(), p->get_B()};

        left_parent_desc.insert({{p, desc}});
    }

    for (auto p: right_parents) {

        std::vector<Node<PointType, OrderType>*> desc{p->get_L(), p->get_R(), p->get_A(), p->get_B()};

        right_parent_desc.insert({{p, desc}});
    }

    Node<PointType, OrderType>* result;

    if (right->get_priority() == 7 && left->get_priority() == 6)
        std::cout << "hello\n";

    if (left->get_trapezoid().get_right() != right->get_trapezoid().get_left()) {
        result = v_merge_partial(left, right);
    } else {
        if (left_priority < right_priority) {
            auto node = v_merge_left_lower_priority_case(left, right);
            // visMe = asJsonGraph(subdag_roots);
            result = node;
        }

        if (left_priority > right_priority) {    
            auto node = v_merge_right_lower_priority_case(left, right);
            // visMe = asJsonGraph(subdag_roots);
            result = node;
        }

        if (left_priority == right_priority) {    
            auto node = v_merge_equal_priority_case(left, right);
            // visMe = asJsonGraph(subdag_roots);
            result = node;
        }
    }


    for (auto& p_desc: left_parent_desc) {
        auto parent = p_desc.first;
        auto descendants = p_desc.second;

        if (parent->get_priority() >= result->get_priority()
        && (right->get_trapezoid().overlaps(parent->get_trapezoid())
        || left->get_trapezoid().overlaps(parent->get_trapezoid()))) {
            continue;
        }
        if (parent == right || parent == left)
            continue;

        if (descendants[0] == left) {
            parent->set_L(result);
        }
        if (descendants[1] == left) {
            parent->set_R(result);
        }
        if (descendants[2] == left) {
            parent->set_A(result);
        }
        if (descendants[3] == left) {
            parent->set_B(result);
        }
    }

    for (auto& p_desc: right_parent_desc) {
        auto parent = p_desc.first;
        auto descendants = p_desc.second;

        if (parent->get_priority() >= result->get_priority()
        && (right->get_trapezoid().overlaps(parent->get_trapezoid())
        || left->get_trapezoid().overlaps(parent->get_trapezoid()))) {
            continue;
        }
        if (parent == right || parent == left)
            continue;


        if (descendants[0] == right) {
            parent->set_L(result);
        }
        if (descendants[1] == right) {
            parent->set_R(result);
        }
        if (descendants[2] == right) {
            parent->set_A(result);
        }
        if (descendants[3] == right) {
            parent->set_B(result);
        }
    }

    visMe = asJsonGraph(subdag_roots);
    return result;
}