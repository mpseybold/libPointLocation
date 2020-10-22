#include "BoundingTrap.h"

template <class PointType, class OrderType>
std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>
BoundingTrap<PointType, OrderType>::vertical_destruction(Cut<PointType, OrderType>* cut) {
    assert(cut->get_cut_type() != EDGE);
    assert(cut != nullptr);

    BoundingTrap<PointType, OrderType> new_trap_pos, new_trap_neg;

    switch(type) {
        case BRTL: {
            assert(cut->defining_point_cut_comparison(*left) != -1);
            if (cut->defining_point_cut_comparison(*right) == 1) {
                assert(false);
            }
            assert(cut->defining_point_cut_comparison(*right) != 1);

            new_trap_pos =
            BoundingTrap<PointType, OrderType>(bottom, right, top, cut);
            
            new_trap_neg =
            BoundingTrap<PointType, OrderType>(bottom, cut, top, left);
            
            break;
        }
        case BRT: {
            assert(cut->defining_point_cut_comparison(*right) != 1);
            assert(cut->defining_point_cut_comparison(
                Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
            ) != -1);

            new_trap_neg = 
            BoundingTrap<PointType, OrderType>(bottom, cut, top, nullptr);

            new_trap_pos = 
            BoundingTrap<PointType, OrderType>(bottom, right, top, cut);

            break;
        }
        case BTL: {
            assert(cut->defining_point_cut_comparison(*left) != -1);
            assert(cut->defining_point_cut_comparison(
                Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
            ) != 1);

            new_trap_neg = 
            BoundingTrap<PointType,
             OrderType>(bottom, cut, top, left);

            new_trap_pos =
            BoundingTrap<PointType, OrderType>(bottom, nullptr, top, cut);    

            break;
        }
    }
    
    return std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>(
        new_trap_pos, new_trap_neg
    );
}

template <class PointType, class OrderType>
std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>
BoundingTrap<PointType, OrderType>::edge_destruction(Cut<PointType, OrderType>* cut) {
    assert(cut != nullptr && cut->get_cut_type() == EDGE);
    
    BoundingTrap<PointType, OrderType> new_trap_pos, new_trap_neg;

    switch(type) {
        case BRTL: {
            // TODO: add some assertions for safety

            if (Segment<PointType, OrderType>::slope_comparison(*cut->get_segment(), *cut->get_segment()) != 0) {
                auto intersection = Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), cut->get_segment());

                if (left != nullptr && intersection.defining_point_cut_comparison(*left) == 0) {
                    new_trap_pos = BoundingTrap<PointType, OrderType>(cut, right, top, nullptr);
                } else if (right != nullptr && intersection.defining_point_cut_comparison(*right) == 0) {
                    new_trap_pos = BoundingTrap<PointType, OrderType>(cut, nullptr, top, left);
                } else {
                    new_trap_pos = BoundingTrap<PointType, OrderType>(cut, right, top, left);
                }
            } else {
                new_trap_pos = BoundingTrap<PointType, OrderType>(cut, right, top, left);
            }

            if (Segment<PointType, OrderType>::slope_comparison(*bottom->get_segment(), *cut->get_segment()) != 0) {
                auto intersection = Cut<PointType, OrderType>(INTERSECTION, bottom->get_segment(), cut->get_segment());

                if (left != nullptr && intersection.defining_point_cut_comparison(*left) == 0) {
                    new_trap_neg = BoundingTrap<PointType, OrderType>(bottom, right, cut, nullptr);
                } else if (right != nullptr && intersection.defining_point_cut_comparison(*right) == 0) {
                    new_trap_neg = BoundingTrap<PointType, OrderType>(bottom, nullptr, cut, left);
                } else {
                    new_trap_neg = BoundingTrap<PointType, OrderType>(bottom, right, cut, left);
                }
            } else {
                new_trap_neg = BoundingTrap<PointType, OrderType>(bottom, right, cut, left);
            }

            break;
        }
        case BRT: {
            // TODO: add some assertions for safety

            new_trap_pos =
            BoundingTrap<PointType, OrderType>(cut, right, top, nullptr);

            new_trap_neg = 
            BoundingTrap<PointType, OrderType>(bottom, right, cut, nullptr);

            break;
        }
        case BTL: {
            //  TODO: add some assertions for safety
            
            new_trap_pos = 
            BoundingTrap<PointType, OrderType>(cut, nullptr, top, left);

            new_trap_neg = 
            BoundingTrap<PointType, OrderType>(bottom, nullptr, cut, left);

            break;
        }
    }

    return std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>(
        new_trap_pos, new_trap_neg
    );
}

template <class PointType, class OrderType>
std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>
BoundingTrap<PointType, OrderType>::destroy(Cut<PointType, OrderType>* cut) {

    if (cut->get_cut_type() == EDGE)
        return edge_destruction(cut);

    return vertical_destruction(cut);
}

template <class PointType, class OrderType>
BoundingTrap<PointType, OrderType> BoundingTrap<PointType, OrderType>::vertical_merge(
    BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2
) { 

    if (!(trap_1.get_top() == trap_2.get_top())) {
        std::cout << "here is a problem\n";
    }
    assert(trap_1.get_top() == trap_2.get_top());
    assert(trap_1.get_bottom() == trap_2.get_bottom());
    assert(trap_1.get_right() == trap_2.get_left());
    return BoundingTrap<PointType, OrderType>(
        trap_1.get_bottom(), trap_2.get_right(), 
        trap_1.get_top(), trap_1.get_left()
    );
}

template <class PointType, class OrderType>
BoundingTrap<PointType, OrderType> BoundingTrap<PointType, OrderType>::edge_merge(
    BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2
) { 
    // assert(trap_1.get_left() == trap_2.get_left());
    // assert(trap_1.get_right() == trap_2.get_right());

    Cut<PointType, OrderType>* left_cut = trap_1.get_left() == nullptr ?
    trap_2.get_left() : trap_1.get_left();

    Cut<PointType, OrderType>* right_cut = trap_1.get_right() == nullptr ?
    trap_1.get_right() : trap_1.get_right();

    return BoundingTrap<PointType, OrderType>(
        trap_2.get_bottom(), right_cut, 
        trap_1.get_top(), left_cut
    ); 
}

template <class PointType, class OrderType>
BoundingTrap<PointType, OrderType> BoundingTrap<PointType, OrderType>::merge(
    BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2
) {
    if (trap_1.get_right() != nullptr 
    && trap_1.get_right() == trap_2.get_left()) 
        return vertical_merge(trap_1, trap_2);

    if (trap_1.get_bottom() == trap_2.get_top()) 
        return edge_merge(trap_1, trap_2);

    throw std::logic_error("trapezoids do not share the correct boundary cut");
}

template <class PointType, class OrderType>
int BoundingTrap<PointType, OrderType>::trap_cut_region(Segment<PointType, OrderType>* seg, int endpoint) {

    assert(endpoint == 0 || endpoint == 1);
    PointType p = endpoint == 0 ? seg->get_source() : seg->get_target();
    PointType other_p = endpoint == 0 ? seg->get_target() : seg->get_source();

    Cut<PointType, OrderType> _left_cut = left == nullptr 
    ? Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
    : *left;
    auto left_cut = &_left_cut;

    Cut<PointType, OrderType> _right_cut = right == nullptr 
    ? Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
    : *right;
    auto right_cut = &_right_cut;

    if (left_cut->orientation(p) < 0 || (left_cut->orientation(p) == 0 && left_cut->orientation(other_p) < 0)) {
        if (top->orientation(seg, endpoint) > 0) {
            return 1;
        }
        if (top->orientation(seg, endpoint) < 0) {
            if (bottom->orientation(seg, endpoint) > 0) {
                return 4;
            }
            if (bottom->orientation(seg, endpoint) < 0) {
                return 7;
            }
        }
    }
    if (left_cut->orientation(p) > 0 || (left_cut->orientation(p) == 0 && left_cut->orientation(other_p) > 0)) {
        if (top->orientation(seg, endpoint) > 0) {
            if (right_cut->orientation(p) < 0) {
                return 2;
            }
            if (right_cut->orientation(p) > 0) {
                return 3;
            }
        }
        if (top->orientation(seg, endpoint) < 0) {
            if (bottom->orientation(seg, endpoint) > 0) {
                if (right_cut->orientation(p) < 0 || (right_cut->orientation(p) == 0 && right_cut->orientation(other_p) == -1)) {
                    return 5;
                }
                if (right_cut->orientation(p) > 0 || (right_cut->orientation(p) == 0 && right_cut->orientation(other_p) == 1)) {
                    return 6;
                } 
            }
            if (bottom->orientation(seg, endpoint) < 0) {
                if (right_cut->orientation(p) < 0 || (right_cut->orientation(p) == 0 && right_cut->orientation(other_p) == -1)) {
                    return 8;
                }
                if (right_cut->orientation(p) > 0 || (right_cut->orientation(p) == 0 && right_cut->orientation(other_p) == 1)) {
                    return 9;
                }
            }
        }
    }

    return 0;
}

template <class PointType, class OrderType>
std::pair<int, int> BoundingTrap<PointType, OrderType>::trap_cut_regions(Segment<PointType, OrderType>* seg) {

    if (top->has_seg_on_pos_side(seg) && !top->has_seg_on_neg_side(seg))
        return {1, 1};

    if (bottom->has_seg_on_neg_side(seg) && !bottom->has_seg_on_pos_side(seg))
        return {9, 9};

    // PointType src = seg->get_source();
    // PointType tgt = seg->get_target();

    return {trap_cut_region(seg, 0), trap_cut_region(seg, 1)};
    // int src_region, tgt_region;

    // Cut<PointType, OrderType> left_cut = left.get_cut_type() == NO_CUT 
    // ? Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
    // : left;

    // Cut<PointType, OrderType> right_cut = right.get_cut_type() == NO_CUT 
    // ? Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
    // : right;

    // if (top.has_seg_on_neg_side(seg) && bottom.has_seg_on_pos_side(seg)) {
    //     if (left_cut.orientation(src) == -1) {
    //         src_region = 4;
    //     } else if (left_cut.orientation(src) == 1 && right_cut.orientation(src) == -1) {
    //         src_region = 5;
    //     } else {
    //         src_region = 6;
    //     }

    //     if (left_cut.orientation(tgt) == -1) {
    //         tgt_region = 4;
    //     } else if (left_cut.orientation(tgt) == 1 && right_cut.orientation(tgt) == -1) {
    //         tgt_region = 5;
    //     } else {
    //         tgt_region = 6;
    //     }

    //     return {src_region, tgt_region};
    // }

    // if (top.orientation(src) == 1 && top.orientation(tgt) == -1) {
    //     if (left_cut.orientation(src) == -1) {
    //         src_region = 1;
    //     } else if (left_cut.orientation(src) == 1 && right_cut.orientation(src) == -1) {
    //         src_region = 2;
    //     } else {
    //         src_region = 3;
    //     }
    //     if (bottom.orientation(tgt) == 1) {
    //         if (left_cut.orientation(tgt) == -1) {
    //             tgt_region = 4;
    //         } else if (left_cut.orientation(tgt) == 1 && right_cut.orientation(tgt) == -1) {
    //             tgt_region = 5;
    //         } else {
    //             tgt_region = 6;
    //         }
    //     } else if (bottom.orientation(tgt) == -1) {
    //         if (left_cut.orientation(tgt) == -1) {
    //             tgt_region = 7;
    //         } else if (left_cut.orientation(tgt) == 1 && right_cut.orientation(tgt) == -1) {
    //             tgt_region = 8;
    //         } else {
    //             tgt_region = 9;
    //         }
    //     }
    // }

}

template <class PointType, class OrderType>
bool BoundingTrap<PointType, OrderType>::intersect_seg_triangle(Segment<PointType, OrderType>* seg) {
    return false;
}

template <class PointType, class OrderType>
bool BoundingTrap<PointType, OrderType>::through_corner(Segment<PointType, OrderType>* seg) {
    if (top->has_on(seg) || bottom->has_on(seg))
        return false;

    auto left_cut = left != nullptr ? *left :
    Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment());
    auto right_cut = right != nullptr ? *right :
    Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment());
    
    auto top_int = Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), seg);
    auto bottom_int = Cut<PointType, OrderType>(INTERSECTION, bottom->get_segment(), seg);

    // test top left
    if (Segment<PointType, OrderType>::slope_comparison(*seg, *top->get_segment()) != 0 &&
        top_int.defining_point_cut_comparison(left_cut) == 0)
        return true;

    // test top right
    if (Segment<PointType, OrderType>::slope_comparison(*seg, *top->get_segment()) != 0 &&
        top_int.defining_point_cut_comparison(right_cut) == 0)
        return true;

    // test bottom left
    if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom->get_segment()) != 0 &&
        bottom_int.defining_point_cut_comparison(left_cut) == 0)
        return true;

    // test bottom right
    if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom->get_segment()) != 0 &&
        bottom_int.defining_point_cut_comparison(right_cut) == 0)
        return true;

    return false;
}

template <class PointType, class OrderType>
bool BoundingTrap<PointType, OrderType>::intersect_corner(Segment<PointType, OrderType>* seg) {
    if (top->has_on(seg) || bottom->has_on(seg))
        return false;

    auto left_cut = left != nullptr ? *left :
    Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment());
    auto right_cut = right != nullptr ? *right :
    Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment());
    
    auto top_int = Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), seg);
    auto bottom_int = Cut<PointType, OrderType>(INTERSECTION, bottom->get_segment(), seg);

    // test top left corner
    if (Segment<PointType, OrderType>::slope_comparison(*seg, *top->get_segment()) != 0 &&
        top_int.defining_point_cut_comparison(left_cut, false) == 0 &&
        top->orientation(seg->get_target()) > -1 &&
        left_cut.orientation(seg->get_source(), false) < 1)
            return true;

    // test bottom left corner
    if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom->get_segment()) != 0 &&
        bottom_int.defining_point_cut_comparison(left_cut, false) == 0 &&
        bottom->orientation(seg->get_target()) < 1 &&
        left_cut.orientation(seg->get_source(), false) < 1)
            return true;

    // test top right corner
    if (Segment<PointType, OrderType>::slope_comparison(*seg, *top->get_segment()) != 0 &&
        top_int.defining_point_cut_comparison(right_cut, false) == 0 &&
        top->orientation(seg->get_source()) > -1 &&
        right_cut.orientation(seg->get_target(), false) > -1)
            return true;

    // test bottom right corner
    if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom->get_segment()) != 0 &&
        bottom_int.defining_point_cut_comparison(right_cut, false) == 0 &&
        bottom->orientation(seg->get_source()) < 1 &&
        right_cut.orientation(seg->get_target(), false) > -1)
            return true;

    return false;
}

template <class PointType, class OrderType>
bool BoundingTrap<PointType, OrderType>::intersect_seg_trap(Segment<PointType, OrderType>* seg) {

    if (intersect_corner(seg))
        return false;

    Cut<PointType, OrderType> _left_cut = left == nullptr 
    ? Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
    : *left;
    auto left_cut = &_left_cut;

    Cut<PointType, OrderType> _right_cut = right == nullptr
    ? Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
    : *right;
    auto right_cut = &_right_cut;

    Cut<PointType, OrderType> seg_cut = Cut<PointType, OrderType>(
        EDGE, seg, nullptr
    );

    Cut<PointType, OrderType> t_intersection = Cut<PointType, OrderType>(
        INTERSECTION, seg, top->get_segment()
    );
    auto top_intersection = &t_intersection;
    Cut<PointType, OrderType> b_intersection = Cut<PointType, OrderType>(
        INTERSECTION, seg, bottom->get_segment()
    );
    auto bottom_intersection = &b_intersection;

    // if (left_cut->orientation(seg->get_source()) == 0 && right_cut->orientation(seg->get_target()) == 0)
    //     return true;

    // First handle 'flat' trapezoids.
    if (top->has_on(bottom->get_segment())) {
        if (top->has_on(seg)) {
            if (seg < top->get_segment() && bottom->get_segment() < seg
            && (left_cut->has_seg_on_pos_side(seg) && right_cut->has_seg_on_neg_side(seg))) {
                return true;
            }
        } else {
            if (top->has_seg_on_neg_side(seg) && top->has_seg_on_pos_side(seg)) {
                if (top_intersection->defining_point_cut_comparison(*left_cut) == 1
                && top_intersection->defining_point_cut_comparison(*right_cut) == -1) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    auto regions = trap_cut_regions(seg);
    int source_region = regions.first;
    int target_region = regions.second;

    if ((source_region <= 3 && source_region > 0) && (target_region <= 3 && target_region > 0))
        return false;

    if ((source_region - 1) % 3 == 0 && (target_region - 1) % 3 == 0)
        return false;

    if ((source_region % 3 == 0 && source_region != 0) && (target_region % 3 == 0 && target_region != 0))
        return false;

    if (source_region - 7 >= 0 && target_region - 7 >= 0)
        return false;

    if (source_region == 5 || target_region == 5)
        return true;

    if (source_region == 4 && target_region == 6)
        return true;

    if (source_region == 2 && target_region == 8)
        return true;

    if (target_region == 2 && source_region == 8)
        return true;

    // handle degenerate cases where the segment lies
    // on the top or bottom boundary
    if (bottom->has_on(seg) && seg > bottom->get_segment()) {
        if (left_cut->has_seg_on_pos_side(seg) && right_cut->has_seg_on_neg_side(seg))
            return true;
    }

    if (top->has_on(seg) && top->get_segment() > seg) {
        if (left_cut->has_seg_on_pos_side(seg) && right_cut->has_seg_on_neg_side(seg))
            return true;
    }

    bool intersects_via_top =
    top->has_seg_on_pos_side(seg) && top->has_seg_on_neg_side(seg) ? 
    (top_intersection->defining_point_cut_comparison(*left_cut) == 1 &&
        top_intersection->defining_point_cut_comparison(*right_cut) == -1) :
    false;

    if ((top->has_seg_on_pos_side(seg) && top->has_seg_on_neg_side(seg))
    && (top_intersection->defining_point_cut_comparison(*left_cut) == 0 && top_intersection->defining_point_cut_comparison(*right_cut) < 1)) {
        intersects_via_top = top->orientation(seg->get_target()) == -1;
    }

    if ((top->has_seg_on_pos_side(seg) && top->has_seg_on_neg_side(seg))
    && (top_intersection->defining_point_cut_comparison(*left_cut) > -1 && top_intersection->defining_point_cut_comparison(*right_cut) == 0)) {
        intersects_via_top = top->orientation(seg->get_source()) == -1;
    }

    bool intersects_via_bottom =
    (bottom->has_seg_on_pos_side(seg) && bottom->has_seg_on_neg_side(seg)) ? 
    (bottom_intersection->defining_point_cut_comparison(*left_cut) == 1 &&
        bottom_intersection->defining_point_cut_comparison(*right_cut) == -1) :
    false;

    if ((bottom->has_seg_on_pos_side(seg) && bottom->has_seg_on_neg_side(seg))
    && (bottom_intersection->defining_point_cut_comparison(*left_cut) == 0 && bottom_intersection->defining_point_cut_comparison(*right_cut) < 1)) {
        intersects_via_bottom = bottom->orientation(seg->get_target()) == 1;
    }

    if ((bottom->has_seg_on_pos_side(seg) && bottom->has_seg_on_neg_side(seg))
    && (bottom_intersection->defining_point_cut_comparison(*left_cut) > -1 && bottom_intersection->defining_point_cut_comparison(*right_cut) == 0)) {
        intersects_via_bottom = bottom->orientation(seg->get_source()) == 1;
    }

    bool intersects_via_left = false;
    if (left_cut->has_seg_on_pos_side(seg) && left_cut->has_seg_on_neg_side(seg)) {
        if (Segment<PointType, OrderType>::slope_comparison(*seg, *(bottom->get_segment())) == 0) {      
            int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *(top->get_segment()));
            assert(top_slope_comp != 0);
            bool top_test = top_slope_comp == 1
            ? top_intersection->defining_point_cut_comparison(*left_cut) == -1
            : top_intersection->defining_point_cut_comparison(*left_cut) == 1;

            int bottom_test = top_intersection->defining_point_cut_comparison(*bottom) == 1;
            intersects_via_left = top_test && bottom_test;
            // if (top_intersection.defining_point_cut_comparison(left_cut) == 1 && top_intersection.defining_point_cut_comparison(right_cut) == -1) {
            //     intersects_via_left = true;
            // }
        }
        else if (Segment<PointType, OrderType>::slope_comparison(*seg, *(top->get_segment())) == 0) {
            int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *(bottom->get_segment()));
            assert(bottom_slope_comp != 0);
            bool bottom_test = bottom_slope_comp == 1
            ? bottom_intersection->defining_point_cut_comparison(*left_cut) == 1
            : bottom_intersection->defining_point_cut_comparison(*left_cut) == -1;

            int top_test = bottom_intersection->defining_point_cut_comparison(*top) == -1;
            intersects_via_left = top_test && bottom_test;
            // if (bottom_intersection.defining_point_cut_comparison(left_cut) == 1 && bottom_intersection.defining_point_cut_comparison(right_cut) == -1) {
            //     intersects_via_left = true;
            // }
        }
        else if (bottom_intersection->defining_point_cut_comparison(*left_cut) * top_intersection->defining_point_cut_comparison(*left_cut) == -1) {
            int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *(top->get_segment()));
            int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *(bottom->get_segment()));
            bool bottom_test = bottom_slope_comp == 1
            ? bottom_intersection->defining_point_cut_comparison(*left_cut) == 1
            : bottom_intersection->defining_point_cut_comparison(*left_cut) == -1;

            bool top_test = top_slope_comp == 1
            ? top_intersection->defining_point_cut_comparison(*left_cut) == -1
            : top_intersection->defining_point_cut_comparison(*left_cut) == 1;

            intersects_via_left = top_test && bottom_test;
            // intersects_via_left = true;
        }
    }

    bool intersects_via_right = false;
    if (right_cut->has_seg_on_pos_side(seg) && right_cut->has_seg_on_neg_side(seg)) {
        if (Segment<PointType, OrderType>::slope_comparison(*seg, *(bottom->get_segment())) == 0) {
            int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *(top->get_segment()));
            assert(top_slope_comp != 0);
            switch(top_slope_comp) {
                case 1: {
                    if (top_intersection->defining_point_cut_comparison(*right_cut) == -1 
                    && top_intersection->defining_point_cut_comparison(*bottom) == 1) {
                        intersects_via_right = true;
                    }
                    break;
                }
                case -1: {
                    if (top_intersection->defining_point_cut_comparison(*right_cut) == 1
                    && top_intersection->defining_point_cut_comparison(*bottom) == 1) {
                        intersects_via_right = true;
                    }
                    break;
                }
            }
            // if (top_intersection.defining_point_cut_comparison(left_cut) == 1 && top_intersection.defining_point_cut_comparison(right_cut) == -1) {
            //     intersects_via_left = true;
            // }
        }
        else if (Segment<PointType, OrderType>::slope_comparison(*seg, *(top->get_segment())) == 0) {
            int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *(bottom->get_segment()));
            assert(bottom_slope_comp != 0);
            switch(bottom_slope_comp) {
                case 1: {
                    if (bottom_intersection->defining_point_cut_comparison(*right_cut) == 1 
                    && bottom_intersection->defining_point_cut_comparison(*top) == -1) {
                        intersects_via_right = true;
                    }
                    break;
                }
                case -1: {
                    if (bottom_intersection->defining_point_cut_comparison(*right_cut) == -1
                    && bottom_intersection->defining_point_cut_comparison(*top) == -1) {
                        intersects_via_right = true;
                    }
                    break;
                }
            }
            // if (bottom_intersection.defining_point_cut_comparison(left_cut) == 1 && bottom_intersection.defining_point_cut_comparison(right_cut) == -1) {
            //     intersects_via_left = true;
            // }
        }
        else {
            bool top_test;
            bool bottom_test;
            int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *(top->get_segment()));
            int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *(bottom->get_segment()));

            top_test = top_slope_comp == 1 
            ? top_intersection->defining_point_cut_comparison(*right_cut) == -1
            : top_intersection->defining_point_cut_comparison(*right_cut) == 1;

            bottom_test = bottom_slope_comp == 1
            ? bottom_intersection->defining_point_cut_comparison(*right_cut) == 1
            : bottom_intersection->defining_point_cut_comparison(*right_cut) == -1;
            
            intersects_via_right = top_test && bottom_test;
        }
        // else if (bottom_intersection.defining_point_cut_comparison(left_cut) * top_intersection.defining_point_cut_comparison(left_cut) == -1) {
        //     intersects_via_left = true;
        // }
    }



    // if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment()) != 0 
    // && bottom_intersection.defining_point_cut_comparison(right_cut) == 0) {
    //     if (bottom.orientation(seg->get_source()) == -1) {
    //         return false;
    //     }
    // }

    // if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment()) != 0 
    // && bottom_intersection.defining_point_cut_comparison(left_cut) == 0) {
    //     if (bottom.orientation(seg->get_target()) == -1) {
    //         return false;
    //     }
    // }

    // if (Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment()) != 0 && top_intersection.defining_point_cut_comparison(right_cut) == 0) {
    //     if (top.orientation(seg->get_source()) == 1) {
    //         return false;
    //     }
    // }

    // if (Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment()) != 0 && top_intersection.defining_point_cut_comparison(left_cut) == 0) {
    //     if (top.orientation(seg->get_target()) == 1) {
    //         return false;
    //     }
    // }

    if (intersects_via_bottom || intersects_via_top || intersects_via_left || intersects_via_right)
        return true;
    

    if (right != nullptr && right->get_cut_type() == INTERSECTION) {
        Cut<PointType, OrderType> _aux_cut = Cut<PointType, OrderType>(
            EDGE, right_cut->get_intersecting_seg(), nullptr
        );
        auto aux_cut = &_aux_cut;
        if (aux_cut->has_on(seg) 
        && right_cut->orientation(seg->get_target()) >= 0 
        && right_cut->orientation(seg->get_source()) <= 0) {
            return seg > right_cut->get_intersecting_seg();
        }
    }
    if (left != nullptr && left->get_cut_type() == INTERSECTION) {
        Cut<PointType, OrderType> _aux_cut = Cut<PointType, OrderType>(
            EDGE, left_cut->get_intersecting_seg(), nullptr
        );
        auto aux_cut = &_aux_cut;
        if (aux_cut->has_on(seg) 
        && left_cut->orientation(seg->get_target()) >= 0 
        && left_cut->orientation(seg->get_source()) <= 0) {
            return seg > right_cut->get_intersecting_seg();
        }
    }

    return false;
}


template <class PointType, class OrderType>
bool BoundingTrap<PointType, OrderType>::intersects_segment(Segment<PointType, OrderType>* seg) {
    return intersect_seg_trap(seg);
//     Cut<PointType, OrderType> left_cut = left.get_cut_type() == NO_CUT 
//     ? Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
//     : left;

//     Cut<PointType, OrderType> right_cut = right.get_cut_type() == NO_CUT 
//     ? Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
//     : right;

//     Cut<PointType, OrderType> seg_cut = Cut<PointType, OrderType>(
//         EDGE, seg, nullptr
//     );

//     Cut<PointType, OrderType> top_intersection = Cut<PointType, OrderType>(
//         INTERSECTION, seg, top.get_segment()
//     );
//     Cut<PointType, OrderType> bottom_intersection = Cut<PointType, OrderType>(
//         INTERSECTION, seg, bottom.get_segment()
//     );

//     if (left_cut.orientation(seg->get_source()) == 0 && right_cut.orientation(seg->get_target()) == 0)
//         return true;

//     // int source_region = trap_cut_region(seg->get_source());
//     // int target_region = trap_cut_region(seg->get_target());
//     auto regions = trap_cut_regions(seg);
//     int source_region = regions.first;
//     int target_region = regions.second;

//     if ((source_region <= 3 && source_region > 0) && (target_region <= 3 && target_region > 0))
//         return false;

//     if ((source_region - 1) % 3 == 0 && (target_region - 1) % 3 == 0)
//         return false;

//     if ((source_region % 3 == 0 && source_region != 0) && (target_region % 3 == 0 && target_region != 0))
//         return false;

//     if (source_region - 7 >= 0 && target_region - 7 >= 0)
//         return false;

//     if (source_region == 5 || target_region == 5)
//         return true;

//     if (source_region == 4 && target_region == 6)
//         return true;

//     if (source_region == 2 && target_region == 8)
//         return true;

//     if (target_region == 2 && source_region == 8)
//         return true;

//     // handle degenerate cases where the segment lies
//     // on the top or bottom boundary
//     if (bottom.has_on(seg) || top.has_on(seg)) {
//         if (source_region == 4 && target_region != 4 && target_region != 0)
//             return true;
//     }



//     bool intersects_via_top =
//     top.has_seg_on_pos_side(seg) && top.has_seg_on_neg_side(seg) ? 
//     (top_intersection.defining_point_cut_comparison(left_cut) == 1 &&
//         top_intersection.defining_point_cut_comparison(right_cut) == -1) :
//     false;

//     if ((top.has_seg_on_pos_side(seg) && top.has_seg_on_neg_side(seg))
//     && (top_intersection.defining_point_cut_comparison(left_cut) == 0 && top_intersection.defining_point_cut_comparison(right_cut) < 1)) {
//         intersects_via_top = top.orientation(seg->get_target()) == -1;
//     }

//     if ((top.has_seg_on_pos_side(seg) && top.has_seg_on_neg_side(seg))
//     && (top_intersection.defining_point_cut_comparison(left_cut) > -1 && top_intersection.defining_point_cut_comparison(right_cut) == 0)) {
//         intersects_via_top = top.orientation(seg->get_source()) == -1;
//     }

//     bool intersects_via_bottom =
//     (bottom.has_seg_on_pos_side(seg) && bottom.has_seg_on_neg_side(seg)) ? 
//     (bottom_intersection.defining_point_cut_comparison(left_cut) == 1 &&
//         bottom_intersection.defining_point_cut_comparison(right_cut) == -1) :
//     false;

//     if ((bottom.has_seg_on_pos_side(seg) && bottom.has_seg_on_neg_side(seg))
//     && (bottom_intersection.defining_point_cut_comparison(left_cut) == 0 && bottom_intersection.defining_point_cut_comparison(right_cut) < 1)) {
//         intersects_via_bottom = bottom.orientation(seg->get_target()) == 1;
//     }

//     if ((bottom.has_seg_on_pos_side(seg) && bottom.has_seg_on_neg_side(seg))
//     && (bottom_intersection.defining_point_cut_comparison(left_cut) > -1 && bottom_intersection.defining_point_cut_comparison(right_cut) == 0)) {
//         intersects_via_bottom = bottom.orientation(seg->get_source()) == 1;
//     }

//     bool intersects_via_left = false;
//     if (left_cut.has_seg_on_pos_side(seg) && left_cut.has_seg_on_neg_side(seg)) {
//         if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment()) == 0) {      
//             int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment());
//             assert(top_slope_comp != 0);
//             bool top_test = top_slope_comp == 1
//             ? top_intersection.defining_point_cut_comparison(left_cut) == -1
//             : top_intersection.defining_point_cut_comparison(left_cut) == 1;

//             int bottom_test = top_intersection.defining_point_cut_comparison(bottom) == 1;
//             intersects_via_left = top_test && bottom_test;
//             // if (top_intersection.defining_point_cut_comparison(left_cut) == 1 && top_intersection.defining_point_cut_comparison(right_cut) == -1) {
//             //     intersects_via_left = true;
//             // }
//         }
//         else if (Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment()) == 0) {
//             int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment());
//             assert(bottom_slope_comp != 0);
//             bool bottom_test = bottom_slope_comp == 1
//             ? bottom_intersection.defining_point_cut_comparison(left_cut) == 1
//             : bottom_intersection.defining_point_cut_comparison(left_cut) == -1;

//             int top_test = bottom_intersection.defining_point_cut_comparison(top) == -1;
//             intersects_via_left = top_test && bottom_test;
//             // if (bottom_intersection.defining_point_cut_comparison(left_cut) == 1 && bottom_intersection.defining_point_cut_comparison(right_cut) == -1) {
//             //     intersects_via_left = true;
//             // }
//         }
//         else if (bottom_intersection.defining_point_cut_comparison(left_cut) * top_intersection.defining_point_cut_comparison(left_cut) == -1) {
//             int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment());
//             int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment());
//             bool bottom_test = bottom_slope_comp == 1
//             ? bottom_intersection.defining_point_cut_comparison(left_cut) == 1
//             : bottom_intersection.defining_point_cut_comparison(left_cut) == -1;

//             bool top_test = top_slope_comp == 1
//             ? top_intersection.defining_point_cut_comparison(left_cut) == -1
//             : top_intersection.defining_point_cut_comparison(left_cut) == 1;

//             intersects_via_left = top_test && bottom_test;
//             // intersects_via_left = true;
//         }
//     }

//     bool intersects_via_right = false;
//     if (right_cut.has_seg_on_pos_side(seg) && right_cut.has_seg_on_neg_side(seg)) {
//         if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment()) == 0) {
//             int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment());
//             assert(top_slope_comp != 0);
//             switch(top_slope_comp) {
//                 case 1: {
//                     if (top_intersection.defining_point_cut_comparison(right_cut) == -1 
//                     && top_intersection.defining_point_cut_comparison(bottom) == 1) {
//                         intersects_via_right = true;
//                     }
//                     break;
//                 }
//                 case -1: {
//                     if (top_intersection.defining_point_cut_comparison(right_cut) == 1
//                     && top_intersection.defining_point_cut_comparison(bottom) == 1) {
//                         intersects_via_right = true;
//                     }
//                     break;
//                 }
//             }
//             // if (top_intersection.defining_point_cut_comparison(left_cut) == 1 && top_intersection.defining_point_cut_comparison(right_cut) == -1) {
//             //     intersects_via_left = true;
//             // }
//         }
//         else if (Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment()) == 0) {
//             int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment());
//             assert(bottom_slope_comp != 0);
//             switch(bottom_slope_comp) {
//                 case 1: {
//                     if (bottom_intersection.defining_point_cut_comparison(right_cut) == 1 
//                     && bottom_intersection.defining_point_cut_comparison(top) == -1) {
//                         intersects_via_right = true;
//                     }
//                     break;
//                 }
//                 case -1: {
//                     if (bottom_intersection.defining_point_cut_comparison(right_cut) == -1
//                     && bottom_intersection.defining_point_cut_comparison(top) == -1) {
//                         intersects_via_right = true;
//                     }
//                     break;
//                 }
//             }
//             // if (bottom_intersection.defining_point_cut_comparison(left_cut) == 1 && bottom_intersection.defining_point_cut_comparison(right_cut) == -1) {
//             //     intersects_via_left = true;
//             // }
//         }
//         else {
//             bool top_test;
//             bool bottom_test;
//             int top_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment());
//             int bottom_slope_comp = Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment());

//             top_test = top_slope_comp == 1 
//             ? top_intersection.defining_point_cut_comparison(right_cut) == -1
//             : top_intersection.defining_point_cut_comparison(right_cut) == 1;

//             bottom_test = bottom_slope_comp == 1
//             ? bottom_intersection.defining_point_cut_comparison(right_cut) == 1
//             : bottom_intersection.defining_point_cut_comparison(right_cut) == -1;
            
//             intersects_via_right = top_test && bottom_test;
//         }
//         // else if (bottom_intersection.defining_point_cut_comparison(left_cut) * top_intersection.defining_point_cut_comparison(left_cut) == -1) {
//         //     intersects_via_left = true;
//         // }
//     }



//     if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment()) != 0 
//     && bottom_intersection.defining_point_cut_comparison(right_cut) == 0) {
//         if (bottom.orientation(seg->get_source()) == -1) {
//             return false;
//         }
//     }

//     if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment()) != 0 
//     && bottom_intersection.defining_point_cut_comparison(left_cut) == 0) {
//         if (bottom.orientation(seg->get_target()) == -1) {
//             return false;
//         }
//     }

//     if (Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment()) != 0 && top_intersection.defining_point_cut_comparison(right_cut) == 0) {
//         if (top.orientation(seg->get_source()) == 1) {
//             return false;
//         }
//     }

//     if (Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment()) != 0 && top_intersection.defining_point_cut_comparison(left_cut) == 0) {
//         if (top.orientation(seg->get_target()) == 1) {
//             return false;
//         }
//     }


//     if (intersects_via_bottom || intersects_via_top || intersects_via_left || intersects_via_right)
//         return true;
//     else
//         return false;

//     // if (source_region != 0 && target_region != 0)
//     //     return false;

//     if (top.has_on(seg)) {
//         if (left_cut.has_seg_on_pos_side(seg) && right_cut.has_seg_on_neg_side(seg))
//             return seg < top.get_segment();
//         else
//             return false;
//     }

//     if (bottom.has_on(seg)) {
//         if (left_cut.has_seg_on_pos_side(seg) && right_cut.has_seg_on_neg_side(seg))
//             return !(seg < bottom.get_segment());
//         else
//             return false;
//     }

//     if (top.orientation(seg->get_source()) == 0) {
//         bool tgt_below_top = top.orientation(seg->get_target()) == -1;
//         bool src_right_of_left = left_cut.orientation(seg->get_source()) > -1;
//         bool src_left_of_right = right_cut.orientation(seg->get_source()) == -1;
        
//         if (tgt_below_top && src_right_of_left && src_left_of_right)
//             return true;
//         else
//             return false;
//     }

//     if (top.orientation(seg->get_target()) == 0) {
//         bool src_below_top = top.orientation(seg->get_source()) == -1;
//         bool tgt_right_of_left = left_cut.orientation(seg->get_source()) == 1;
//         bool tgt_left_of_right = right_cut.orientation(seg->get_source()) < 1;
        
//         if (src_below_top && tgt_right_of_left && tgt_left_of_right)
//             return true;
//         else
//             return false;
//     }

//     if (bottom.orientation(seg->get_source()) == 0) {
//         bool tgt_above_bottom = bottom.orientation(seg->get_target()) == 1;
//         bool src_right_of_left = left_cut.orientation(seg->get_source()) > -1;
//         bool src_left_of_right = right_cut.orientation(seg->get_source()) == -1;
        
//         if (tgt_above_bottom && src_right_of_left && src_left_of_right)
//             return true;
//         else
//             return false;
//     }

//     if (bottom.orientation(seg->get_target()) == 0) {
//         bool src_above_bottom = bottom.orientation(seg->get_source()) == 1;
//         bool tgt_right_of_left = left_cut.orientation(seg->get_target()) == 1;
//         bool tgt_left_of_right = right_cut.orientation(seg->get_target()) < 1;
        
//         if (src_above_bottom && tgt_right_of_left && tgt_left_of_right)
//             return true;
//         else
//             return false;
//     }

//     return false;
}

template <class PointType, class OrderType>
bool BoundingTrap<PointType, OrderType>::contains_endpoint(Segment<PointType, OrderType>* seg, int endpoint) {
    assert(endpoint == 0 || endpoint == 1);
    assert(seg != nullptr);
    PointType p = endpoint == 0 ? seg->get_source()
    : seg->get_target();

    PointType other_p = endpoint == 0 ? seg->get_target()
    : seg->get_source();

    bool bottom_test, right_test, top_test, left_test;

    bottom_test = bottom->orientation(p) == 1
    || (bottom->orientation(p) == 0 && bottom->orientation(other_p) == 1)
    || (bottom->orientation(p) == 0 && bottom->orientation(other_p) == 0 && *(bottom->get_segment()) < *seg);

    if (right == nullptr)
        right_test = true;
    else {
        right_test = right->orientation(p) == -1
        || (right->orientation(p) == 0 && right->orientation(other_p) == -1);
    }

    top_test = top->orientation(p) == -1
    || (top->orientation(p) == 0 && top->orientation(other_p) == -1)
    || (top->orientation(p) == 0 && top->orientation(other_p) == 0 && *seg < *(top->get_segment()));

    if (left == nullptr) {
        left_test = true;
    } else {
        left_test = left->orientation(p) == 1
        || (left->orientation(p) == 0 && left->orientation(other_p) == 1);
    }

    return bottom_test && right_test && top_test && left_test;
}

template <class PointType, class OrderType>
bool BoundingTrap<PointType, OrderType>::seg_intersects_top(Segment<PointType, OrderType>* seg) {
    
    if (!(top->has_seg_on_pos_side(seg) && top->has_seg_on_neg_side(seg)))
        return false;

    auto _top_intersection = Cut<PointType, OrderType>(
        INTERSECTION, seg, top->get_segment()
    );
    auto top_intersection = &_top_intersection;

    auto _left_cut = left == nullptr
    ? Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
    : *left;
    auto left_cut = &_left_cut;

    auto _right_cut = right == nullptr
    ? Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
    : *right;
    auto right_cut = &_right_cut;
    
    bool intersects_via_top =
    (top_intersection->defining_point_cut_comparison(*left_cut) == 1 &&
        top_intersection->defining_point_cut_comparison(*right_cut) == -1);

    return intersects_via_top;
}

template <class PointType, class OrderType>
bool BoundingTrap<PointType, OrderType>::seg_intersects_bottom(Segment<PointType, OrderType>* seg) {
    
    if (!(bottom->has_seg_on_pos_side(seg) && bottom->has_seg_on_neg_side(seg)))
        return false;

    auto _bottom_intersection = Cut<PointType, OrderType>(
        INTERSECTION, seg, bottom->get_segment()
    );
    auto bottom_intersection = &_bottom_intersection;

    auto _left_cut = left->get_cut_type() == NO_CUT
    ? Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
    : *left;
    auto left_cut = &_left_cut;

    auto _right_cut = right->get_cut_type() == NO_CUT
    ? Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment())
    : *right;
    auto right_cut = &_right_cut;
    
    bool intersects_via_bottom =
    (bottom_intersection->defining_point_cut_comparison(*left_cut) == 1 &&
        bottom_intersection->defining_point_cut_comparison(*right_cut) == -1);

    return intersects_via_bottom;
}

template <class PointType, class OrderType>
bool BoundingTrap<PointType, OrderType>::is_degen() {
    return top->has_on(bottom->get_segment());
}

template class BoundingTrap<PointCart, int>;
// template class BoundingTrap<PointS2ratss, int>;