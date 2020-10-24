#include "TSD.h"
// #include "io.h"

#define VERBOSITY_LEVEL 2


// Returns the next priority descendants of 'node'
// whose regions intersect the segment.
// The nodes are ordered in the output vector in left-to-right
// stabbing order. 
template <class PointType, class OrderType>
void TSD<PointType, OrderType>::search_refinement(Segment<PointType, OrderType>* seg, Node<PointType, OrderType>* node) {
       
    intersecting_descendants = { nullptr, nullptr, nullptr, nullptr };

    if (VERBOSITY_LEVEL >= 2) {
        auto traps = std::vector<BoundingTrap<PointType, OrderType>>();

        if (node->get_L() != nullptr) {
            traps.push_back(node->get_L()->get_trapezoid());
        }
        if (node->get_R() != nullptr) {
            traps.push_back(node->get_R()->get_trapezoid());
        }
        if (node->get_A() != nullptr) {
            traps.push_back(node->get_A()->get_trapezoid());
        }
        if (node->get_B() != nullptr) {
            traps.push_back(node->get_B()->get_trapezoid());
        }

        io::write_trapezoids(traps, "refinement_debug.dat");
        std::cout << "wrote refinement traps...\n";
    }

    auto A_right = node->get_A() != nullptr && node->get_A()->get_trapezoid().get_right() != nullptr ?
    *(node->get_A()->get_trapezoid().get_right()) : 
    Cut<PointType, OrderType>(INTERSECTION, 
    node->get_A()->get_trapezoid().get_top()->get_segment(), node->get_A()->get_trapezoid().get_bottom()->get_segment());

    auto B_right = node->get_B() != nullptr && node->get_B()->get_trapezoid().get_right() != nullptr ?
    *(node->get_B()->get_trapezoid().get_right()) : 
    Cut<PointType, OrderType>(INTERSECTION, 
    node->get_B()->get_trapezoid().get_top()->get_segment(), node->get_B()->get_trapezoid().get_bottom()->get_segment());
    
    if (node->get_L() != nullptr) 
        if (node->get_L()->get_trapezoid().intersects_segment(seg)) {
            // auto trap = node->get_L()->get_trapezoid();
            // auto val = trap.intersects_segment(seg);
            intersecting_descendants[0] = node->get_L();
        }
    
    if (node->get_B() != nullptr) {
        assert(node->get_A() != nullptr);
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
        new Cut<PointType, OrderType>(
            TARGET, new Segment<PointType, OrderType>(
                PointType(PointType::EXTREME_LEFT - PointType::EPS, PointType::EXTREME_TOP - PointType::EPS),
                PointType(PointType::EXTREME_LEFT, PointType::EXTREME_TOP - PointType::EPS),
                -1
                // PointCart(110, -20), PointCart(164, -20), -1
            ), nullptr
        )
    );
    trapezoid.set_right(
        new Cut<PointType, OrderType>(
            SOURCE, new Segment<PointType, OrderType>(
                PointType(PointType::EXTREME_RIGHT - PointType::EPS, PointType::EXTREME_TOP - PointType::EPS),
                PointType(PointType::EXTREME_RIGHT, PointType::EXTREME_TOP - PointType::EPS),
                -1
                // PointCart(168, -20), PointCart(180, -20), -1
            ), nullptr
        )
    );

    trapezoid.set_type(BRTL);

    root = new Node<PointType, OrderType>(trapezoid);
    segments = std::vector<Segment<PointType, OrderType>>();
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::affected_subdag_roots(Segment<PointType, OrderType>* seg) {

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

        if (top->is_leaf()) {
            top->toggle_visited();
            num_subdag_roots++;
            subdag_roots.push_back(top);
            visited_nodes.push_back(top);
            continue;
        }

        search_refinement(seg, top);

        bool non_null_seen = false;

        for (int i = 0; i < intersecting_descendants.size(); ++i) {
            Node<PointType, OrderType>* node = intersecting_descendants[i];
            if (node != nullptr && !node->is_visited()) {
                if (node->get_priority() > seg->get_priority()) {
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

    affected_subdag_roots(&seg);

    //debug

    auto traps = std::vector<BoundingTrap<PointType, OrderType>>();
    for (auto node: subdag_roots) {
        traps.push_back(node->get_trapezoid());
    }
    io::write_trapezoids(traps, "leaves.dat");
    //end debug


    auto e_cut = new Cut<PointType, OrderType>(
        EDGE, &seg, nullptr
    );

    // make a first pass over affected roots to make
    // v-partition calls.
    for (int i = 0; i < subdag_roots.size(); ++i) {

        auto node = subdag_roots[i];
        int v_part_count = 0;

        // TODO: remove after support for overlapping segments is added
        // if (node->get_trapezoid().get_top().has_on(&seg)
        // || node->get_trapezoid().get_bottom().has_on(&seg)) {
        //     std::cout << "aha!\n";
        //     return;
        // }

        // auto trapezoid = node->get_trapezoid();
        // if (!node->is_flat() && (trapezoid.get_top()->has_on(&seg) 
        // || trapezoid.get_bottom()->has_on(&seg)) && trapezoid.intersect_corner(&seg)) {
        //     // partition(node, e_cut);
        //     std::cout << "about to continue...\n";
        //     continue; 
        // }
    
        if (i == 0) {
            if (node->contains_endpoint(&seg, 0)) {
                auto v_cut = new Cut<PointType, OrderType>(
                    SOURCE, &seg, nullptr
                );
                v_partition(node, v_cut);
                v_part_count++;
            }
            if (i < subdag_roots.size()-1) {
                auto next = subdag_roots[i+1];
                assert(next != nullptr);

                // TODO: add support for common intersection points
                // TODO: assertions for crossing/meeting segments

                if (next->get_trapezoid().get_top() == node->get_trapezoid().get_bottom()
                && !node->get_trapezoid().through_corner(&seg)) {
                    auto intersection_cut = new Cut<PointType, OrderType>(
                        INTERSECTION, &seg, node->get_trapezoid().get_bottom()->get_segment()
                    );
                    v_partition(node, intersection_cut);
                    v_part_count++;
                }
                if (next->get_trapezoid().get_bottom() == node->get_trapezoid().get_top()
                && !node->get_trapezoid().through_corner(&seg)) {
                    auto intersection_cut = new Cut<PointType, OrderType>(
                        INTERSECTION, &seg, node->get_trapezoid().get_top()->get_segment()
                    );
                    v_partition(node, intersection_cut);
                    v_part_count++;
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

            if (prev->get_trapezoid().get_top() == node->get_trapezoid().get_bottom()
            && !node->get_trapezoid().through_corner(&seg)) {
                auto intersection_cut = new Cut<PointType, OrderType>(
                    INTERSECTION, &seg, node->get_trapezoid().get_bottom()->get_segment()
                );
                v_partition(node, intersection_cut);
                v_part_count++;
            }
            if (next->get_trapezoid().get_bottom() == node->get_trapezoid().get_top()
            && !node->get_trapezoid().through_corner(&seg)) {
                auto intersection_cut = new Cut<PointType, OrderType>(
                    INTERSECTION, &seg, node->get_trapezoid().get_top()->get_segment()
                );
                v_partition(node, intersection_cut);
                v_part_count++;
            }
            if (prev->get_trapezoid().get_bottom() == node->get_trapezoid().get_top()
            && !node->get_trapezoid().through_corner(&seg)) {
                auto intersection_cut = new Cut<PointType, OrderType>(
                    INTERSECTION, &seg, node->get_trapezoid().get_top()->get_segment()
                );
                v_partition(node, intersection_cut);
                v_part_count++;
            }
            if (next->get_trapezoid().get_top() == node->get_trapezoid().get_bottom()
            && !node->get_trapezoid().through_corner(&seg)) {
                auto intersection_cut = new Cut<PointType, OrderType>(
                    INTERSECTION, &seg, node->get_trapezoid().get_bottom()->get_segment()
                );
                v_partition(node, intersection_cut);
                v_part_count++;
            }
        }

        if (i == subdag_roots.size() - 1) {
            if (i > 0) {
                auto prev = subdag_roots[i-1];
                assert(prev != nullptr);

                if (prev->get_trapezoid().get_top() == node->get_trapezoid().get_bottom()
                && !node->get_trapezoid().through_corner(&seg)) {
                    auto intersection_cut = new Cut<PointType, OrderType>(
                        INTERSECTION, &seg, node->get_trapezoid().get_bottom()->get_segment()
                    );
                    v_partition(node, intersection_cut);
                    v_part_count++;
                }
                if (prev->get_trapezoid().get_bottom() == node->get_trapezoid().get_top()
                && !node->get_trapezoid().through_corner(&seg)) {
                    auto intersection_cut = new Cut<PointType, OrderType>(
                        INTERSECTION, &seg, node->get_trapezoid().get_top()->get_segment()
                    );
                    v_partition(node, intersection_cut);
                    v_part_count++;
                }
            }

            if (node->contains_endpoint(&seg, 1)) {
                auto v_cut = new Cut<PointType, OrderType>(
                    TARGET, &seg, nullptr
                );
                v_partition(node, v_cut);
                v_part_count++;
            }
        }
    }

    if (seg.get_priority() == 16) {
        std::cout << "hello\n";
    }

    // second pass over affected roots
    // to make edge partition calls

    auto merge_indices = std::vector<MergeIndices>();

    for (int i = 0; i < subdag_roots.size(); ++i) {
        auto node = subdag_roots[i];
        partition(node, e_cut);
        if (!node->is_flat()) {

            if (i > 0) {
                auto prev = subdag_roots[i-1];

                // if (i == 3 && VERBOSITY_LEVEL >= 2) {
                //     io::write_trapezoids({subdag_roots[i-1]->get_trapezoid(), subdag_roots[i]->get_trapezoid()}, "merge_debug.dat");
                // }

                bool right_left_match = prev->get_trapezoid().get_right() == node->get_trapezoid().get_left();
                bool top_match = prev->get_trapezoid().get_top() == node->get_trapezoid().get_top();
                bool bottom_match = prev->get_trapezoid().get_bottom() == node->get_trapezoid().get_bottom();
                bool not_pinched_trapezoids = 
                !(prev->get_trapezoid().get_top() == node->get_trapezoid().get_top() && prev->get_trapezoid().get_bottom() == node->get_trapezoid().get_bottom() 
                && prev->get_trapezoid().get_bottom()->has_on(prev->get_trapezoid().get_top()->get_segment()));

                if (prev->get_trapezoid().get_right() == node->get_trapezoid().get_left() &&
                (prev->get_trapezoid().get_top() == node->get_trapezoid().get_top() || 
                prev->get_trapezoid().get_bottom() == node->get_trapezoid().get_bottom())
                && not_pinched_trapezoids) {

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
} 
