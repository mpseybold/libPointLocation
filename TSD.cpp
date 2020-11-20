#include "TSD.h"
// #include "io.h"

#define VERBOSITY_LEVEL 1


template <class PointType, class OrderType>
V_Cut<PointType, OrderType>* TSD<PointType, OrderType>::find_v_cut(Cut<PointType, OrderType>* cut, Node<PointType, OrderType>* node) {
    if (node->get_trapezoid().get_left()
    ->defining_point_cut_comparison(*cut) == 0)
        return node->get_trapezoid().get_v_left();

    if (node->get_trapezoid().get_right()
    ->defining_point_cut_comparison(*cut) == 0)
        return node->get_trapezoid().get_v_right();

    std::vector<Node<PointType, OrderType>*> nodes{
        node->get_L(), node->get_R(),
        node->get_A(), node->get_B()
    };

    for (auto n: nodes) {
        if (n != nullptr && n->get_trapezoid().contains_defining_point(cut)) {
            auto v_cut = find_v_cut(cut, n);
            if (v_cut != nullptr)
                return v_cut;
        }
    }
    
    return nullptr;
}
// Returns the next priority descendants of 'node'
// whose regions intersect the segment.
// The nodes are ordered in the output vector in left-to-right
// stabbing order. 
template <class PointType, class OrderType>
void TSD<PointType, OrderType>::search_refinement(Segment<PointType, OrderType>* seg, Node<PointType, OrderType>* node) {
       
    intersecting_descendants = { nullptr, nullptr, nullptr, nullptr };

    if (VERBOSITY_LEVEL >= 2 && seg->get_priority() == 91) {
        auto traps = std::vector<BoundingTrap<PointType, OrderType>>();

        if (node->get_L() != nullptr && node->get_L()->get_trapezoid().intersects_segment(seg)) {
            traps.push_back(node->get_L()->get_trapezoid());
        }
        if (node->get_R() != nullptr && node->get_R()->get_trapezoid().intersects_segment(seg)) {
            traps.push_back(node->get_R()->get_trapezoid());
        }
        if (node->get_A() != nullptr && node->get_A()->get_trapezoid().intersects_segment(seg)) {
            traps.push_back(node->get_A()->get_trapezoid());
        }
        if (node->get_B() != nullptr && node->get_B()->get_trapezoid().intersects_segment(seg)) {
            traps.push_back(node->get_B()->get_trapezoid());
        }

        io::write_trapezoids(traps, "refinement_debug.dat");
        // std::cout << "wrote refinement traps...\n";
    }

    if (node->get_L() != nullptr) 
        if (node->get_L()->get_trapezoid().intersects_segment(seg)) {
            // auto trap = node->get_L()->get_trapezoid();
            // auto val = trap.intersects_segment(seg);
            intersecting_descendants[0] = node->get_L();
        }
    
    if (node->get_B() != nullptr) {
        assert(node->get_A() != nullptr);

        auto A_right = *(node->get_A()->get_trapezoid().get_right());

        auto B_right = *(node->get_B()->get_trapezoid().get_right());

        Cut<PointType, OrderType>* edge_cut = node
        ->get_destruction_cuts()
        .get_e();

        assert(edge_cut->get_cut_type() == EDGE);

        auto slope_comp = Segment<PointType, OrderType>::slope_comparison(
            *seg, *edge_cut->get_segment()
        );

        if (slope_comp > 0) {
            if (node->get_A()->get_trapezoid().intersects_segment(seg))
                intersecting_descendants[1] = node->get_A();
            // If B intersects the region adjacent to the right of
            // nodes' trapezoid, we don't check for the intersection
            // here to ensure the nodes are reported in the
            // correct order.
            if (A_right.defining_point_cut_comparison(B_right) > -1) {
                if (node->get_B()->get_trapezoid().intersects_segment(seg)) {
                    intersecting_descendants[2] = node->get_B();
                }
            }
            else {
                if (node->get_e()->has_seg_on_neg_side(seg) && node->get_e()->has_seg_on_pos_side(seg)) {
                    auto seg_intersection = Cut<PointType, OrderType>(
                        INTERSECTION, seg, node->get_e()->get_segment()
                    );
                    if (seg_intersection.defining_point_cut_comparison(A_right) == -1) {
                        if (node->get_B()->get_trapezoid().intersects_segment(seg))
                            intersecting_descendants[2] = node->get_B();
                    }
                } else {
                    if ((node->get_B()->get_trapezoid().contains_endpoint(seg, 0)
                        && A_right.orientation(seg->get_source()) == -1)
                        || (node->get_B()->get_trapezoid().contains_endpoint(seg, 1)
                        && A_right.orientation(seg->get_target()) == -1)
                    ) {
                        if (node->get_B()->get_trapezoid().intersects_segment(seg)) {
                            intersecting_descendants[2] = node->get_B();
                        }
                    } else if (!(node->get_B()->get_trapezoid().contains_endpoint(seg, 0))
                        && !(node->get_B()->get_trapezoid().contains_endpoint(seg, 1))
                        && node->get_B()->get_trapezoid().intersects_segment(seg)) {
                            intersecting_descendants[2] = node->get_B();
                    }
                }
                // if (Segment<PointType, OrderType>::slope_comparison(
                //     *seg, *node->get_trapezoid().get_bottom().get_segment()
                // ) != 0) {
                //     auto bottom_intersection = Cut<PointType, OrderType>(
                //         INTERSECTION, seg, node->get_trapezoid().get_bottom().get_segment()
                //     );
                //     if (bottom_intersection.defining_point_cut_comparison(
                //         node->get_B()->get_trapezoid().get_right()
                //     ) == -1) {
                //         if (node->get_B()->get_trapezoid().intersects_segment(seg))
                //             intersecting_descendants[2] = node->get_B();
                //     }
                // }
            }
        } else if (slope_comp < 0) {
            if (node->get_B()->get_trapezoid().intersects_segment(seg))
                intersecting_descendants[1] = node->get_B();
            // If A intersects the region to the right of
            // nodes' trapezoid, we don't check for the intersection
            // here to ensure the nodes are reported in the
            // correct order.
            if (A_right.defining_point_cut_comparison(B_right) < 1) {
                if (node->get_A()->get_trapezoid().intersects_segment(seg)) {
                    intersecting_descendants[2] = node->get_A();
                }
            } else {
                if (node->get_e()->has_seg_on_neg_side(seg) && node->get_e()->has_seg_on_pos_side(seg)) {
                    auto seg_intersection = Cut<PointType, OrderType>(
                        INTERSECTION, seg, node->get_e()->get_segment()
                    );
                    if (node->get_B()->get_trapezoid().get_right() != nullptr &&
                        seg_intersection.defining_point_cut_comparison(
                        *(node->get_B()->get_trapezoid().get_right())
                    ) == -1) {
                        if (node->get_A()->get_trapezoid().intersects_segment(seg)) {
                            intersecting_descendants[2] = node->get_A();
                        }
                    }
                } else {
                    if ((node->get_A()->get_trapezoid().contains_endpoint(seg, 0)
                        && B_right.orientation(seg->get_source()) == -1)
                        || (node->get_A()->get_trapezoid().contains_endpoint(seg, 1)
                        && B_right.orientation(seg->get_target()) == -1)
                    ) {
                        if (node->get_A()->get_trapezoid().intersects_segment(seg)) {
                            intersecting_descendants[2] = node->get_A();
                        }
                    } else if (!(node->get_A()->get_trapezoid().contains_endpoint(seg, 0))
                        && !(node->get_A()->get_trapezoid().contains_endpoint(seg, 1))
                        && node->get_A()->get_trapezoid().intersects_segment(seg)) {
                            intersecting_descendants[2] = node->get_A();
                    }
                }
            }
        } else {
            if (node->get_A()->get_trapezoid().intersects_segment(seg))
                intersecting_descendants[1] = node->get_A();
            if (node->get_B()->get_trapezoid().intersects_segment(seg))
                intersecting_descendants[1] = node->get_B();
        }
    }

    if (node->get_R() != nullptr)
        if (node->get_R()->get_trapezoid().intersects_segment(seg))
            intersecting_descendants[3] = node->get_R();
}


template <class PointType, class OrderType>
TSD<PointType, OrderType>::TSD() {

    leaf_count = 1;

    BoundingTrap<PointType, OrderType> trapezoid = BoundingTrap<PointType, OrderType>();
    trapezoid.set_top(
        new Cut<PointType, OrderType>(
            EDGE, new Segment<PointType, OrderType>(
                PointType(PointType::EXTREME_LEFT + PointType::EPS, PointType::EXTREME_TOP - PointType::EPS),
                PointType(PointType::EXTREME_RIGHT - PointType::EPS, PointType::EXTREME_TOP - PointType::EPS),
                -1
                // PointCart(-1000000, -20), PointCart(11000, -20), -1
            ), nullptr
        )
    );
    trapezoid.set_bottom(
        new Cut<PointType, OrderType>(
            EDGE, new Segment<PointType, OrderType>(
                PointType(PointType::EXTREME_LEFT + PointType::EPS, PointType::EXTREME_BOTTOM + PointType::EPS),
                PointType(PointType::EXTREME_RIGHT - PointType::EPS, PointType::EXTREME_BOTTOM + PointType::EPS),
                -1
                // PointCart(-200000, -23), PointCart(12000, -23), -1
            ), nullptr
        )
    );
    trapezoid.set_left(
        new V_Cut<PointType, OrderType>(
            TARGET, new Segment<PointType, OrderType>(
                PointType(PointType::EXTREME_LEFT - PointType::EPS, PointType::EXTREME_TOP - PointType::EPS),
                PointType(PointType::EXTREME_LEFT, PointType::EXTREME_TOP - PointType::EPS),
                -1
                // PointCart(110, -20), PointCart(164, -20), -1
            ), nullptr
        ), 1
    );
    trapezoid.set_right(
        new V_Cut<PointType, OrderType>(
            SOURCE, new Segment<PointType, OrderType>(
                PointType(PointType::EXTREME_RIGHT - PointType::EPS, PointType::EXTREME_TOP - PointType::EPS),
                PointType(PointType::EXTREME_RIGHT, PointType::EXTREME_TOP - PointType::EPS),
                -1
                // PointCart(168, -20), PointCart(180, -20), -1
            ), nullptr
        ), 1
    );

    trapezoid.set_type(BRTL);

    root = new Node<PointType, OrderType>(trapezoid);
    segments = std::vector<Segment<PointType, OrderType>>();
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::affected_subdag_roots(Segment<PointType, OrderType>* seg, bool insert) {

    int num_visited = 0;
    int num_subdag_roots = 0;

    visited_nodes = std::vector<Node<PointType, OrderType>*>();
    subdag_roots = std::vector<Node<PointType, OrderType>*>();
    
    // nodes are stored in left-to-right stabbing order on the stack
    search_stack = std::stack<Node<PointType, OrderType>*>();
    search_stack.push(root);

    while (!search_stack.empty()) {
        num_visited++;

        Node<PointType, OrderType>* top = search_stack.top();
        while(top->is_visited()) {
            search_stack.pop();
            if (search_stack.empty())
                break;
            top = search_stack.top();
            assert(top != nullptr);
        }
        if (search_stack.empty())
            continue;
        search_stack.pop();

        if ((insert && top->is_leaf()) || (!insert && top->get_priority() == seg->get_priority())) {
            top->toggle_visited();
            num_subdag_roots++;
            subdag_roots.push_back(top);
            visited_nodes.push_back(top);
            continue;
        }

        if (!insert && top->get_priority() == 91 && seg->get_priority() == 91)
            std::cout << "hello\n";


        search_refinement(seg, top);

        bool non_null_seen = false;

        for (int i = 0; i < intersecting_descendants.size(); ++i) {
            Node<PointType, OrderType>* node = intersecting_descendants[i];
            if (node != nullptr && !node->is_visited()) {
                bool condition = insert ? node->get_priority() > seg->get_priority() :
                (node->get_priority() == seg->get_priority());
                if (condition) {
                    if (!insert && node->is_leaf())
                        std::cout << "hello\n";
                    subdag_roots.push_back(node);
                    node->toggle_visited();
                    visited_nodes.push_back(node);
                    num_subdag_roots++;
                    intersecting_descendants[i] = nullptr;
                } else {
                    non_null_seen = true;
                }
            } else {
                if (node != nullptr) {
                    // std::cout << "node seen twice\n";
                    intersecting_descendants[i] = nullptr;
                }
            }
            if (non_null_seen)
                break;
        }

        for (int i = intersecting_descendants.size()-1; i >= 0; --i) {
            Node<PointType, OrderType>* node = intersecting_descendants[i];
            if (node != nullptr)
                search_stack.push(node);
        }
    }

    for (auto node: visited_nodes) {
        assert(node != nullptr);
        node->toggle_visited();
    }

    // std::cout << subdag_roots.size() << "\n";

    if (VERBOSITY_LEVEL >= 100) {
        std::cout << "number of nodes visited:\t" << num_visited << "\n";
        std::cout << "number of subdag roots:\t" << num_subdag_roots << "\n";
    }
}

struct MergeIndices {
    int start_index;
    int end_index;
    int side;

    MergeIndices(int si, int ei, int s) :
    start_index(si), end_index(ei), side(s) {
        assert(side == -1 || side == 1);
    } 
};

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::insert_segment(Segment<PointType, OrderType>& seg) {
   //TODO: implement this properly/test the code

    // for (auto& s: segments) {
    //     assert(!(s == seg));
    // }

    // TODO: check if move is needed
    segments.push_back(seg);
    // std::cout << "search...\n";
    affected_subdag_roots(&seg, true);

    //debug
    // auto traps = std::vector<BoundingTrap<PointType, OrderType>>();
    // for (auto node: subdag_roots) {
    //     traps.push_back(node->get_trapezoid());
    // }
    // io::write_trapezoids(traps, "leaves.dat");
    // end debug


    auto e_cut = new Cut<PointType, OrderType>(
        EDGE, &seg, nullptr
    );

    // make a first pass over affected roots to make
    // v-partition calls.
    // std::cout << "v_partitions..\n";
    for (int i = 0; i < subdag_roots.size(); ++i) {

        auto node = subdag_roots[i];
        int v_part_count = 0;


        // if (i == 0 && seg.get_priority() == 62) {
        //     io::write_trapezoids({subdag_roots[i]->get_trapezoid(), subdag_roots[i+1]->get_trapezoid()}, "merge_debug.dat");
        // }

    
        if (i == 0) {
            auto src_cut = new Cut<PointType, OrderType>(
                SOURCE, &seg, nullptr
            );
            if (node->contains_endpoint(&seg, 0)) {
                auto v_cut = find_v_cut(src_cut, root);

                if (v_cut == nullptr) {
                    v_cut = new V_Cut<PointType, OrderType>(SOURCE, &seg, nullptr);
                } else {
                    std::cout << "found cut\n";
                    v_cut->insert_intersection(SOURCE, &seg, nullptr);
                }

                v_partition(node, v_cut, 1);
                v_part_count++;
            } else if (node->get_trapezoid().get_left()->defining_point_cut_comparison(*src_cut) == 0) {
                node->get_trapezoid().get_v_left()->insert_intersection(SOURCE, &seg, nullptr);
            }
            delete src_cut;
            if (i < subdag_roots.size()-1) {
                auto next = subdag_roots[i+1];
                assert(next != nullptr);

                // TODO: add support for common intersection points
                // TODO: assertions for crossing/meeting segments

                int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*node->get_trapezoid().get_top()->get_segment(), seg);
                int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*node->get_trapezoid().get_bottom()->get_segment(), seg);

                auto top_int = top_slope_comp != 0 ? new Cut<PointType, OrderType>(INTERSECTION, node->get_trapezoid().get_top()->get_segment(), &seg)
                : nullptr;
                auto bottom_int = bottom_slope_comp != 0 ? new Cut<PointType, OrderType>(INTERSECTION, node->get_trapezoid().get_bottom()->get_segment(), &seg)
                : nullptr;

                if (top_int != nullptr && (top_int->defining_point_cut_comparison(*node->get_trapezoid().get_left()) < 1 
                || top_int->defining_point_cut_comparison(*node->get_trapezoid().get_right()) > -1
                || node->get_trapezoid().get_top()->orientation(seg.get_source()) == 0
                || !(node->get_trapezoid().get_top()->has_seg_on_pos_side(&seg) && node->get_trapezoid().get_top()->has_seg_on_neg_side(&seg)))) {
                    delete top_int;
                    top_int = nullptr;
                }

                if (bottom_int != nullptr &&
                (bottom_int->defining_point_cut_comparison(*node->get_trapezoid().get_left()) < 1 
                || bottom_int->defining_point_cut_comparison(*node->get_trapezoid().get_right()) > -1
                || node->get_trapezoid().get_bottom()->orientation(seg.get_source()) == 0
                || !(node->get_trapezoid().get_bottom()->has_seg_on_pos_side(&seg) && node->get_trapezoid().get_bottom()->has_seg_on_neg_side(&seg)))) {
                    delete bottom_int;
                    bottom_int = nullptr;
                }

                if (top_int != nullptr) {
                    
                    auto top_v_cut = find_v_cut(top_int, root);

                    if (top_v_cut == nullptr) {
                        top_v_cut = new V_Cut<PointType, OrderType>(INTERSECTION, node->get_trapezoid().get_top()->get_segment(), &seg);
                    } else {
                        top_v_cut->insert_intersection(INTERSECTION, node->get_trapezoid().get_top()->get_segment(), &seg);
                    }
                    delete top_int;
                    v_partition(node, top_v_cut, -1);
                
                } else if (bottom_int != nullptr) {
                    
                    auto bottom_v_cut = find_v_cut(bottom_int, root);

                    if (bottom_v_cut == nullptr) {
                        bottom_v_cut = new V_Cut<PointType, OrderType>(INTERSECTION, node->get_trapezoid().get_bottom()->get_segment(), &seg);
                    } else {
                        bottom_v_cut->insert_intersection(INTERSECTION, node->get_trapezoid().get_bottom()->get_segment(), &seg);
                    }

                    delete bottom_int;
                    v_partition(node, bottom_v_cut, 1);
                }
            }
        }

        if (i > 0 && i < subdag_roots.size() - 1) {
            auto next = subdag_roots[i+1];
            auto prev = subdag_roots[i-1];

            assert(next != prev);
            assert(prev != nullptr);
            assert(next != nullptr);
            assert(next != node);
            assert(prev != node);

            // Debug code
            // std::vector<BoundingTrap<PointType, OrderType>> debug_nodes = std::vector<BoundingTrap<PointType, OrderType>>();
            // debug_nodes.push_back(prev->get_trapezoid());
            // debug_nodes.push_back(next->get_trapezoid());
            // io::write_trapezoids(debug_nodes, "debug_nodes.dat");
            // end debug code

            int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*node->get_trapezoid().get_top()->get_segment(), seg);
            int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*node->get_trapezoid().get_bottom()->get_segment(), seg);

            auto top_int = top_slope_comp != 0 ? new Cut<PointType, OrderType>(INTERSECTION, node->get_trapezoid().get_top()->get_segment(), &seg)
            : nullptr;
            auto bottom_int = bottom_slope_comp != 0 ? new Cut<PointType, OrderType>(INTERSECTION, node->get_trapezoid().get_bottom()->get_segment(), &seg)
            : nullptr;

            if (top_int != nullptr && (top_int->defining_point_cut_comparison(*node->get_trapezoid().get_left()) < 1 
            || top_int->defining_point_cut_comparison(*node->get_trapezoid().get_right()) > -1))
                top_int = nullptr;

            if (bottom_int != nullptr && (bottom_int->defining_point_cut_comparison(*node->get_trapezoid().get_left()) < 1 
            || bottom_int->defining_point_cut_comparison(*node->get_trapezoid().get_right()) > -1))
                bottom_int = nullptr;

            Cut<PointType, OrderType>* leftmost = nullptr;
            Cut<PointType, OrderType>* rightmost = nullptr;

            if (top_int != nullptr) {
                if (bottom_int == nullptr || top_int->defining_point_cut_comparison(*bottom_int) == -1) {
                    leftmost = top_int;
                    if (bottom_int != nullptr)
                        rightmost = bottom_int;
                } else if (bottom_int != nullptr) {
                    leftmost = bottom_int;
                    rightmost = top_int;
                }
            } else if (bottom_int != nullptr) {
                leftmost = bottom_int;
            }

            if (leftmost != nullptr) {
                auto leftmost_v_cut = find_v_cut(leftmost, root);

                if (leftmost_v_cut == nullptr) {
                    leftmost_v_cut = new V_Cut<PointType, OrderType>(INTERSECTION, 
                    leftmost->get_segment(), leftmost->get_intersecting_seg());
                } else {
                    leftmost_v_cut->insert_intersection(
                        INTERSECTION, leftmost->get_segment(),
                        leftmost->get_intersecting_seg()
                    );
                }

                int side = leftmost == top_int ? -1 : 1;
                delete leftmost;
                v_partition(node, leftmost_v_cut, side);
            }

            if (rightmost != nullptr) {

                auto rightmost_v_cut = find_v_cut(rightmost, root);

                if (rightmost_v_cut == nullptr) {
                    rightmost_v_cut = new V_Cut<PointType, OrderType>(
                        INTERSECTION, rightmost->get_segment(), rightmost->get_intersecting_seg()
                    );
                } else {
                    rightmost_v_cut->insert_intersection(
                        INTERSECTION, rightmost->get_segment(), rightmost->get_intersecting_seg()
                    );
                }

                int side = rightmost == top_int ? -1 : 1;
                delete rightmost;
                v_partition(node, rightmost_v_cut, side);
            }
        }

        if (i == subdag_roots.size() - 1) {
            if (i > 0) {
                int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*node->get_trapezoid().get_top()->get_segment(), seg);
                int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*node->get_trapezoid().get_bottom()->get_segment(), seg);

                auto top_int = top_slope_comp != 0 ? new Cut<PointType, OrderType>(INTERSECTION, node->get_trapezoid().get_top()->get_segment(), &seg)
                : nullptr;
                auto bottom_int = bottom_slope_comp != 0 ? new Cut<PointType, OrderType>(INTERSECTION, node->get_trapezoid().get_bottom()->get_segment(), &seg)
                : nullptr;

                if (top_int != nullptr && (top_int->defining_point_cut_comparison(*node->get_trapezoid().get_left()) < 1 
                || top_int->defining_point_cut_comparison(*node->get_trapezoid().get_right()) > -1
                || node->get_trapezoid().get_top()->orientation(seg.get_target()) == 0
                || !(node->get_trapezoid().get_top()->has_seg_on_pos_side(&seg) && node->get_trapezoid().get_top()->has_seg_on_neg_side(&seg))))
                    top_int = nullptr;

                if (bottom_int != nullptr && (bottom_int->defining_point_cut_comparison(*node->get_trapezoid().get_left()) < 1 
                || bottom_int->defining_point_cut_comparison(*node->get_trapezoid().get_right()) > -1
                || node->get_trapezoid().get_bottom()->orientation(seg.get_target()) == 0
                || !(node->get_trapezoid().get_bottom()->has_seg_on_pos_side(&seg) && node->get_trapezoid().get_bottom()->has_seg_on_neg_side(&seg))))
                    bottom_int = nullptr;

                if (top_int != nullptr) {

                    auto top_v_cut = find_v_cut(top_int, root);

                    if (top_v_cut == nullptr) {
                        top_v_cut = new V_Cut<PointType, OrderType>(
                            INTERSECTION, top_int->get_segment(), top_int->get_intersecting_seg()
                        );
                    } else {
                        top_v_cut->insert_intersection(
                            INTERSECTION, top_int->get_segment(), top_int->get_intersecting_seg()
                        );
                    }

                    delete top_int;
                    v_partition(node, top_v_cut, -1);
                } else if (bottom_int != nullptr) {

                    auto bottom_v_cut = find_v_cut(bottom_int, root);

                    if (bottom_v_cut == nullptr) {
                        bottom_v_cut = new V_Cut<PointType, OrderType>(
                            INTERSECTION, bottom_int->get_segment(), bottom_int->get_intersecting_seg()
                        );
                    } else {
                        bottom_v_cut->insert_intersection(
                            INTERSECTION, bottom_int->get_segment(), bottom_int->get_intersecting_seg()
                        );
                    }

                    v_partition(node, bottom_v_cut, 1);
                }

            }

            auto tgt_cut = new Cut<PointType, OrderType>(
                TARGET, &seg, nullptr
            );
            if (node->contains_endpoint(&seg, 1)) {
                auto v_cut = find_v_cut(tgt_cut, root);
                if (v_cut == nullptr) {
                    v_cut = new V_Cut<PointType, OrderType>(TARGET, &seg, nullptr);
                } else {
                    std::cout << "found cut\n";
                    v_cut->insert_intersection(TARGET, &seg, nullptr);
                }
                
                v_partition(node, v_cut, 1);
                v_part_count++;
            } else if (node->get_trapezoid()
            .get_right()->defining_point_cut_comparison(*tgt_cut) == 0) {
                node->get_trapezoid().get_v_right()->insert_intersection(
                    TARGET, &seg, nullptr
                );
            }
            delete tgt_cut;
        }
    }


    // second pass over affected roots
    // to make edge partition calls

    auto merge_indices = std::vector<MergeIndices>();
    // std::cout << "e_partitions..\n";
    for (int i = 0; i < subdag_roots.size(); ++i) {
        auto node = subdag_roots[i];
        partition(node, e_cut);
        if (!node->is_flat()) {

            if (i > 0) {
                auto prev = subdag_roots[i-1];


                bool right_left_match = prev->get_trapezoid().get_right() == node->get_trapezoid().get_left();
                bool top_match = prev->get_trapezoid().get_top() == node->get_trapezoid().get_top();
                bool bottom_match = prev->get_trapezoid().get_bottom() == node->get_trapezoid().get_bottom();
                bool not_pinched_trapezoids = 
                !(prev->get_trapezoid().get_top() == node->get_trapezoid().get_top() && prev->get_trapezoid().get_bottom() == node->get_trapezoid().get_bottom() 
                && prev->get_trapezoid().get_bottom()->has_on(prev->get_trapezoid().get_top()->get_segment()));

                bool thread_through = (Segment<PointType, OrderType>::slope_comparison(
                    *(prev->get_trapezoid().get_top()->get_segment()), *(prev->get_trapezoid().get_bottom()->get_segment())) != 0
                    && (Cut<PointType, OrderType>(INTERSECTION, prev->get_trapezoid().get_top()->get_segment(), prev->get_trapezoid().get_bottom()->get_segment()))
                    .defining_point_cut_comparison(*(prev->get_trapezoid().get_right())) == 0)
                    ||  (Segment<PointType, OrderType>::slope_comparison(
                    *(node->get_trapezoid().get_top()->get_segment()), *(node->get_trapezoid().get_bottom()->get_segment())) != 0
                    && (Cut<PointType, OrderType>(INTERSECTION, node->get_trapezoid().get_top()->get_segment(), node->get_trapezoid().get_bottom()->get_segment()))
                    .defining_point_cut_comparison(*(node->get_trapezoid().get_left())) == 0);


                if (prev->get_trapezoid().get_right() == node->get_trapezoid().get_left() &&
                (prev->get_trapezoid().get_top() == node->get_trapezoid().get_top() || 
                prev->get_trapezoid().get_bottom() == node->get_trapezoid().get_bottom())
                && (not_pinched_trapezoids && !thread_through)) {

                    // set the pointers for v_merge
                    prev->set_right(node);
                    node->set_left(prev);

                    auto v_cut = prev->get_trapezoid().get_right();

                    //if (v_cut.defining_point_cut_comparison(e_cut) == 1) {
                    if (Cut<PointType, OrderType>::v_cut_edge_orientation(*v_cut, *e_cut) == 1) {
                        // merge below the segment
                        if (merge_indices.size() > 0 && merge_indices.back().end_index == i-1 && merge_indices.back().side == -1) {
                            merge_indices.back().end_index = i;
                        } else {
                            merge_indices.push_back(MergeIndices(i-1, i, -1));
                        }
                    } else {
                        // merge above segment
                        if (merge_indices.size() > 0 && merge_indices.back().end_index == i-1 && merge_indices.back().side == 1) {
                            merge_indices.back().end_index = i;
                        } else {
                            merge_indices.push_back(MergeIndices(i-1, i, 1));
                        }
                    }
                }
            }
        }
    }

    // final pass to merge nodes
    for (auto& indices: merge_indices) {
        Node<PointType, OrderType>* merged_node = indices.side == -1 
        ? subdag_roots[indices.start_index]->get_B() : subdag_roots[indices.start_index]->get_A();

        for (int i = indices.start_index + 1; i <= indices.end_index; ++i) {
            auto node = subdag_roots[i];
            if (indices.side == -1) {
                merged_node = v_merge(merged_node, node->get_B());
            } else if (indices.side == 1) {
                merged_node = v_merge(merged_node, node->get_A());
            } else {
                assert(false);
            }
        }

        for (int i = indices.start_index; i <= indices.end_index; ++i) {
            auto node = subdag_roots[i];
            if (indices.side == -1) {
                node->set_B(merged_node);
            } else {
                node->set_A(merged_node);
            }
        }
    }
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::delete_segment(Segment<PointType, OrderType>& seg) {
    //TODO: implement this properly/test the code
    if (seg.get_priority() == 91)
        std::cout << "hello\n";
    affected_subdag_roots(&seg, false);

    //DEBUG
        if (seg.get_priority() == 25)
            io::write_trapezoids(subdag_roots, "plotting/before_merges.dat");
    //DEBUG

    //TODO: Undo merges subdag_roots above and below segment/make v-partition calls
    int i = subdag_roots.size() - 1;

    while (i > 0) {
        auto node = subdag_roots[i];
        if (node->get_e() == nullptr) {
            --i;
            continue;
        }
        auto A = node->get_A();
        auto B = node->get_B();
        
        auto pattern = node->get_dest_pattern();

        assert(A != nullptr);
        assert(B != nullptr);

        auto trap_A = A->get_trapezoid();
        auto trap_B = B->get_trapezoid();

        int side = trap_B.get_left()->defining_point_cut_comparison(*trap_A.get_left());

        if (side != 0) {
            auto cut = side == 1 ? trap_B.get_v_left() : trap_A.get_v_left();
            auto descendant = side == 1 ? A : B;

            v_partition(descendant, cut, side);

            if (side == 1)
                node->set_A(descendant->get_R());
            else
                node->set_B(descendant->get_R());
            

            int j = i-1;

            auto prev_node = subdag_roots[j];
            auto prev_A_left = prev_node->get_A()
                ->get_trapezoid().get_left();
            auto prev_B_left = prev_node->get_B()
                ->get_trapezoid().get_left();

            auto prev_desc = descendant->get_L();

            bool condition = 
            side == prev_B_left->defining_point_cut_comparison(*prev_A_left);

            if (!condition) {
                if (side == 1)
                    prev_node->set_A(prev_desc);
                else
                    prev_node->set_B(prev_desc);
            } 
            

            while (condition) {
                
                cut = side == 1 ? 
                prev_node->get_B()->get_trapezoid().get_v_left() : 
                prev_node->get_A()->get_trapezoid().get_v_left();
                
                v_partition(prev_desc, cut, side);
                
                if (side == 1)
                    prev_node->set_A(prev_desc->get_R());
                else
                    prev_node->set_B(prev_desc->get_R());

                auto aux = prev_desc;
                prev_desc = prev_desc->get_L();
                delete aux;

                assert(j > 0);
                j--;
                prev_node = subdag_roots[j];
                prev_A_left = prev_node->get_A()
                    ->get_trapezoid().get_left();
                prev_B_left = prev_node->get_B()
                    ->get_trapezoid().get_left();

                condition = 
                side == prev_B_left->defining_point_cut_comparison(*prev_A_left);

                if (!condition) {
                    if (side == 1)
                        prev_node->set_A(prev_desc);
                    else
                        prev_node->set_B(prev_desc);
                } 
            }

            delete descendant;
            i = j;
        } else 
            --i;
    }

    //TODO: make merge calls

    for (int i = subdag_roots.size()-1; i >= 0; --i) {
        auto node = subdag_roots[i];

        auto pattern = node->get_dest_pattern();

        if (pattern == VE) {
            node->set_R(merge(node->get_B(), node->get_A()));
            node->set_A(nullptr);
            node->set_B(nullptr);
        } else if (pattern == EV) {
            node->set_L(merge(node->get_B(), node->get_A()));
            node->set_A(nullptr);
            node->set_B(nullptr);
            node->set_v_1(node->get_v_2());
            node->clear_v_2();
        } else if (pattern == VVE) {
            node->set_A(merge(node->get_B(), node->get_A()));
            node->set_B(nullptr);
        } else if (pattern == E) {
            node->copy_node(merge(node->get_B(), node->get_A()));
        }

        if (pattern != E)
            node->clear_e();

    }

    //TODO: make v_merge calls

    for (int i = subdag_roots.size()-1; i >= 0; --i) {
        if (seg.get_priority() == 93)
            std::cout << "hello\n";
        auto node = subdag_roots[i];

        if (i > 0 && node->get_trapezoid().get_v_left()->contains(&seg) 
        && (subdag_roots[i-1]->get_v_1() == nullptr || subdag_roots[i-1]->get_v_1() != node->get_trapezoid().get_v_left())
        && (subdag_roots[i-1]->get_v_2() == nullptr || subdag_roots[i-1]->get_v_2() != node->get_trapezoid().get_v_left())) {
            node->get_trapezoid().get_v_left()->delete_segment(&seg);
        }

        if (i == subdag_roots.size()-1 && node->get_trapezoid().get_v_right()->contains(&seg))
            node->get_trapezoid().get_v_right()->delete_segment(&seg);

        if (i == 0 && node->get_trapezoid().get_v_left()->contains(&seg))
            node->get_trapezoid().get_v_left()->delete_segment(&seg);

        auto pattern = node->get_dest_pattern();
        if (pattern == VV && node->get_v_1()->contains(&seg) && node->get_v_2()->contains(&seg)) {
            auto v_1 = node->get_v_1();
            auto v_2 = node->get_v_2();

            node->set_R(v_merge(node->get_A(), node->get_R()));
            node->copy_node(v_merge(node->get_L(), node->get_R()));

            if (i > 0) {
                auto prev = subdag_roots[i-1];

                if (v_2 != prev->get_v_1() && v_2 != prev->get_v_2()) {
                    v_2->delete_segment(&seg);
                    if (v_2->is_empty())
                        delete v_2;
                }

                if (v_1 != prev->get_v_1() && v_1 != prev->get_v_2()) {
                    v_1->delete_segment(&seg);
                    if (v_1->is_empty())
                        delete v_1;
                }
            } else if (i == 0) {
                v_2->delete_segment(&seg);
                    if (v_2->is_empty())
                        delete v_2;
                v_1->delete_segment(&seg);
                    if (v_1->is_empty())
                        delete v_1;
            }
        } else if (pattern == V && node->get_v_1()->contains(&seg)) {
            auto v_1 = node->get_v_1();
            if (node->get_v_2() != nullptr)
                std::cout << "oops...\n";
            node->copy_node(v_merge(node->get_L(), node->get_R()));

            if (i > 0) {
                auto prev = subdag_roots[i-1];
                if (v_1 != prev->get_v_1() && v_1 != prev->get_v_2()) {
                    v_1->delete_segment(&seg);
                    if (v_1->is_empty())
                        delete v_1;
                }
            } else if (i == 0) {
                v_1->delete_segment(&seg);
                    if (v_1->is_empty())
                        delete v_1;
            }
        } else {
            if (pattern != NO_DESTRUCTION)
                std::cout << "hello\n";
            if (node->get_v_1() != nullptr)
                assert(!node->get_v_1()->contains(&seg));
            if (node->get_v_2() != nullptr)
                assert(!node->get_v_2()->contains(&seg));
            if (node->get_e() != nullptr)
                assert(node->get_e()->get_segment() != &seg);
        }

        if (node->get_L() != nullptr)
            std::cout << "problem...\n";
    }


} 

// template <class PointType, class OrderType>
// V_Cut<PointType, OrderType>* find_v_cut(Cut<PointType, OrderType>* cut, Node<PointType, OrderType>* node) {
//     assert(cut->get_cut_type() != EDGE);

//     auto trap = node->get_trapezoid();

// }
