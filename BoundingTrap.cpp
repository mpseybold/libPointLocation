#include "BoundingTrap.h"

template <class PointType, class OrderType>
std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>
BoundingTrap<PointType, OrderType>::vertical_destruction(Cut<PointType, OrderType> cut) {
    assert(cut.get_cut_type() != EDGE);
    assert(cut.get_cut_type() != NO_CUT);

    BoundingTrap<PointType, OrderType> new_trap_pos, new_trap_neg;

    switch(type) {
        case BRTL: {
            assert(cut.defining_point_cut_comparison(left) != -1);
            assert(cut.defining_point_cut_comparison(right) != 1);

            new_trap_pos =
            BoundingTrap<PointType, OrderType>(bottom, right, top, cut);
            
            new_trap_neg =
            BoundingTrap<PointType, OrderType>(bottom, cut, top, left);
            
            break;
        }
        case BRT: {
            assert(cut.defining_point_cut_comparison(right) != 1);
            assert(cut.defining_point_cut_comparison(
                Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
            ) != -1);

            new_trap_neg = 
            BoundingTrap<PointType, OrderType>(bottom, cut, top, Cut<PointType, OrderType>());

            new_trap_pos = 
            BoundingTrap<PointType, OrderType>(bottom, right, top, cut);

            break;
        }
        case BTL: {
            assert(cut.defining_point_cut_comparison(left) != -1);
            assert(cut.defining_point_cut_comparison(
                Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
            ) != 1);

            new_trap_neg = 
            BoundingTrap<PointType, OrderType>(bottom, cut, top, left);

            new_trap_pos =
            BoundingTrap<PointType, OrderType>(bottom, Cut<PointType, OrderType>(), top, cut);    

            break;
        }
    }
    
    return std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>(
        new_trap_pos, new_trap_neg
    );
}

template <class PointType, class OrderType>
std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>
BoundingTrap<PointType, OrderType>::edge_destruction(Cut<PointType, OrderType> cut) {
    assert(cut.get_cut_type() == EDGE);
    
    BoundingTrap<PointType, OrderType> new_trap_pos, new_trap_neg;

    switch(type) {
        case BRTL: {
            // TODO: add some assertions for safety

            new_trap_pos =
            BoundingTrap<PointType, OrderType>(cut, right, top, left);

            new_trap_neg =
            BoundingTrap<PointType, OrderType>(bottom, right, cut, left);

            break;
        }
        case BRT: {
            // TODO: add some assertions for safety

            new_trap_pos =
            BoundingTrap<PointType, OrderType>(cut, right, top, Cut<PointType, OrderType>());

            new_trap_neg = 
            BoundingTrap<PointType, OrderType>(bottom, right, cut, Cut<PointType, OrderType>());

            break;
        }
        case BTL: {
            //  TODO: add some assertions for safety
            
            new_trap_pos = 
            BoundingTrap<PointType, OrderType>(cut, Cut<PointType, OrderType>(), top, left);

            new_trap_neg = 
            BoundingTrap<PointType, OrderType>(bottom, Cut<PointType, OrderType>(), cut, left);

            break;
        }
    }

    return std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>(
        new_trap_pos, new_trap_neg
    );
}

template <class PointType, class OrderType>
std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>
BoundingTrap<PointType, OrderType>::destroy(Cut<PointType, OrderType> cut) {

    if (cut.get_cut_type() == EDGE)
        return edge_destruction(cut);

    return vertical_destruction(cut);
}

template <class PointType, class OrderType>
BoundingTrap<PointType, OrderType> BoundingTrap<PointType, OrderType>::vertical_merge(
    BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2
) { 
    assert(trap_1.get_top() == trap_2.get_top());
    assert(trap_1.get_bottom() == trap_2.get_bottom());
    return BoundingTrap<PointType, OrderType>(
        trap_1.get_bottom(), trap_1.get_right(), 
        trap_1.get_top(), trap_2.get_left()
    );
}

template <class PointType, class OrderType>
BoundingTrap<PointType, OrderType> BoundingTrap<PointType, OrderType>::edge_merge(
    BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2
) { 
    // assert(trap_1.get_left() == trap_2.get_left());
    // assert(trap_1.get_right() == trap_2.get_right());

    Cut<PointType, OrderType> left_cut = trap_1.get_left().get_cut_type() == NO_CUT ?
    trap_2.get_left() : trap_1.get_left();

    Cut<PointType, OrderType> right_cut = trap_1.get_right().get_cut_type() == NO_CUT ?
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
    if (trap_1.get_left().get_cut_type() != NO_CUT 
    && trap_1.get_left() == trap_2.get_right()) 
        return vertical_merge(trap_1, trap_2);

    if (trap_1.get_bottom() == trap_2.get_top()) 
        return edge_merge(trap_1, trap_2);

    throw std::logic_error("trapezoids do not share the correct boundary cut");
}

template <class PointType, class OrderType>
int BoundingTrap<PointType, OrderType>::trap_cut_region(Segment<PointType, OrderType>* seg, int endpoint) {

    assert(endpoint == 0 || endpoint == 1);
    PointType p = endpoint == 0 ? seg->get_source() : seg->get_target();

    Cut<PointType, OrderType> left_cut = left.get_cut_type() == NO_CUT 
    ? Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
    : left;

    Cut<PointType, OrderType> right_cut = right.get_cut_type() == NO_CUT 
    ? Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
    : right;

    if (left_cut.orientation(p) < 0) {
        if (top.orientation(seg, endpoint) > 0) {
            return 1;
        }
        if (top.orientation(seg, endpoint) < 0) {
            if (bottom.orientation(seg, endpoint) > 0) {
                return 4;
            }
            if (bottom.orientation(seg, endpoint) < 0) {
                return 7;
            }
        }
    }
    if (left_cut.orientation(p) > 0) {
        if (top.orientation(seg, endpoint) > 0) {
            if (right_cut.orientation(p) < 0) {
                return 2;
            }
            if (right_cut.orientation(p) > 0) {
                return 3;
            }
        }
        if (top.orientation(seg, endpoint) < 0) {
            if (bottom.orientation(seg, endpoint) > 0) {
                if (right_cut.orientation(p) < 0) {
                    return 5;
                }
                if (right_cut.orientation(p) > 0) {
                    return 6;
                } 
            }
            if (bottom.orientation(seg, endpoint) < 0) {
                if (right_cut.orientation(p) < 0) {
                    return 8;
                }
                if (right_cut.orientation(p) > 0) {
                    return 9;
                }
            }
        }
    }

    return 0;
}

template <class PointType, class OrderType>
std::pair<int, int> BoundingTrap<PointType, OrderType>::trap_cut_regions(Segment<PointType, OrderType>* seg) {

    if (top.has_seg_on_pos_side(seg) && !top.has_seg_on_neg_side(seg)) 
        return {1, 1};

    if (bottom.has_seg_on_neg_side(seg) && !bottom.has_seg_on_pos_side(seg))
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
bool BoundingTrap<PointType, OrderType>::intersects_segment(Segment<PointType, OrderType>* seg) {

    Cut<PointType, OrderType> left_cut = left.get_cut_type() == NO_CUT 
    ? Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
    : left;

    Cut<PointType, OrderType> right_cut = right.get_cut_type() == NO_CUT 
    ? Cut<PointType, OrderType>(INTERSECTION, top.get_segment(), bottom.get_segment())
    : right;

    Cut<PointType, OrderType> seg_cut = Cut<PointType, OrderType>(
        EDGE, seg, nullptr
    );

    Cut<PointType, OrderType> top_intersection = Cut<PointType, OrderType>(
        INTERSECTION, seg, top.get_segment()
    );
    Cut<PointType, OrderType> bottom_intersection = Cut<PointType, OrderType>(
        INTERSECTION, seg, bottom.get_segment()
    );

    // int source_region = trap_cut_region(seg->get_source());
    // int target_region = trap_cut_region(seg->get_target());
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

    if (source_region == 5 && target_region == 8)
        return true;

    if (source_region == 8 && target_region == 5)
        return true;


    bool intersects_via_top =
    top.has_seg_on_pos_side(seg) && top.has_seg_on_neg_side(seg) ? 
    (top_intersection.defining_point_cut_comparison(left_cut) == 1 &&
        top_intersection.defining_point_cut_comparison(right_cut) == -1) :
    false;

    bool intersects_via_bottom =
    (bottom.has_seg_on_pos_side(seg) && bottom.has_seg_on_neg_side(seg)) ? 
    (bottom_intersection.defining_point_cut_comparison(left_cut) == 1 &&
        bottom_intersection.defining_point_cut_comparison(right_cut) == -1) :
    false;

    // bool bottom_is_left_of_left_pred = Segment<PointType, OrderType>::slope_comparison(
    //     *seg, *bottom.get_segment()
    // ) == 0 ? bottom.has_seg_on_pos_side(seg) :
    // bottom_intersection.defining_point_cut_comparison(left_cut) == -1;
    // bool bottom_is_left_of_right_pred = Segment<PointType, OrderType>::slope_comparison(
    //     *seg, *bottom.get_segment()
    // ) == 0 ? bottom.has_seg_on_pos_side(seg) :
    // bottom_intersection.defining_point_cut_comparison(right_cut) == -1;
    // bool bottom_is_right_of_left_pred = Segment<PointType, OrderType>::slope_comparison(
    //     *seg, *bottom.get_segment()
    // ) == 0 ? bottom.has_seg_on_pos_side(seg) :
    // bottom_intersection.defining_point_cut_comparison(left_cut) == 1;
    // bool bottom_is_right_of_right_pred = Segment<PointType, OrderType>::slope_comparison(
    //     *seg, *bottom.get_segment()
    // ) == 0 ? bottom.has_seg_on_pos_side(seg) :
    // bottom_intersection.defining_point_cut_comparison(right_cut) == 1;

    // bool top_is_left_of_left_pred = Segment<PointType, OrderType>::slope_comparison(
    //     *seg, *top.get_segment()
    // ) == 0 ? top.has_seg_on_pos_side(seg) :
    // top_intersection.defining_point_cut_comparison(left_cut) == -1;
    // bool top_is_left_of_right_pred = Segment<PointType, OrderType>::slope_comparison(
    //     *seg, *top.get_segment()
    // ) == 0 ? top.has_seg_on_pos_side(seg) :
    // top_intersection.defining_point_cut_comparison(right_cut) == -1;
    // bool top_is_right_of_left_pred = Segment<PointType, OrderType>::slope_comparison(
    //     *seg, *top.get_segment()
    // ) == 0 ? top.has_seg_on_pos_side(seg) :
    // top_intersection.defining_point_cut_comparison(left_cut) == 1;
    // bool top_is_right_of_right_pred = Segment<PointType, OrderType>::slope_comparison(
    //     *seg, *top.get_segment()
    // ) == 0 ? top.has_seg_on_pos_side(seg) :
    // top_intersection.defining_point_cut_comparison(right_cut) == 1;

    // bool intersects_via_left =
    // (left_cut.has_seg_on_pos_side(seg) && left_cut.has_seg_on_neg_side(seg)) ? 
    // ((bottom_is_left_of_left_pred && top_is_right_of_left_pred) || 
    // (top_is_left_of_left_pred && bottom_is_right_of_left_pred)) :
    // false;

    // bool intersects_via_right =
    // (right_cut.has_seg_on_pos_side(seg) && right_cut.has_seg_on_neg_side(seg)) ? 
    // ((bottom_is_left_of_right_pred && top_is_right_of_right_pred) || 
    // (top_is_left_of_right_pred && bottom_is_right_of_right_pred)) :
    // false;

    bool intersects_via_left = false;
    if (left_cut.has_seg_on_pos_side(seg) && left_cut.has_seg_on_neg_side(seg)) {
        if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment()) == 0) {
            if (top_intersection.defining_point_cut_comparison(left_cut) == 1) {
                intersects_via_left = true;
            }
        }
        else if (Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment()) == 0) {
            if (bottom_intersection.defining_point_cut_comparison(left_cut) == 1) {
                intersects_via_left = true;
            }
        }
        else if (bottom_intersection.defining_point_cut_comparison(left_cut) * top_intersection.defining_point_cut_comparison(left_cut) == -1) {
            intersects_via_left = true;
        }
    }

    // bool intersects_via_right = false;
    // if (right_cut.has_seg_on_neg_side(seg) && right_cut.has_seg_on_neg_side(seg)) {
    //     if (Segment<PointType, OrderType>::slope_comparison(*seg, *bottom.get_segment()) == 0) {
    //         if (top_intersection.defining_point_cut_comparison(right_cut) == -1) {
    //             intersects_via_right = true;
    //         }
    //     } else if (Segment<PointType, OrderType>::slope_comparison(*seg, *top.get_segment()) == 0) {
    //         if (bottom_intersection.defining_point_cut_comparison(right_cut) == -1)
    //             intersects_via_right = true;
    //     }
    //     else if (bottom_intersection.defining_point_cut_comparison(right_cut) * top_intersection.defining_point_cut_comparison(right_cut) == -1) {
    //         intersects_via_left = true;
    //     }
    // }


    if (intersects_via_bottom || intersects_via_top || intersects_via_left)
        return true;

    if (source_region != 0 && target_region != 0)
        return false;

    if (top.has_on(seg)) {
        if (left_cut.has_seg_on_pos_side(seg) && right_cut.has_seg_on_neg_side(seg))
            return seg < top.get_segment();
        else
            return false;
    }

    if (bottom.has_on(seg)) {
        if (left_cut.has_seg_on_pos_side(seg) && right_cut.has_seg_on_neg_side(seg))
            return seg > bottom.get_segment();
        else
            return false;
    }

    if (top.orientation(seg->get_source()) == 0) {
        bool tgt_below_top = top.orientation(seg->get_target()) == -1;
        bool src_right_of_left = left_cut.orientation(seg->get_source()) > -1;
        bool src_left_of_right = right_cut.orientation(seg->get_source()) == -1;
        
        if (tgt_below_top && src_right_of_left && src_left_of_right)
            return true;
        else
            return false;
    }

    if (top.orientation(seg->get_target()) == 0) {
        bool src_below_top = top.orientation(seg->get_source()) == -1;
        bool tgt_right_of_left = left_cut.orientation(seg->get_source()) == 1;
        bool tgt_left_of_right = right_cut.orientation(seg->get_source()) < 1;
        
        if (src_below_top && tgt_right_of_left && tgt_left_of_right)
            return true;
        else
            return false;
    }

    if (bottom.orientation(seg->get_source()) == 0) {
        bool tgt_above_bottom = bottom.orientation(seg->get_target()) == 1;
        bool src_right_of_left = left_cut.orientation(seg->get_source()) > -1;
        bool src_left_of_right = right_cut.orientation(seg->get_source()) == -1;
        
        if (tgt_above_bottom && src_right_of_left && src_left_of_right)
            return true;
        else
            return false;
    }

    if (bottom.orientation(seg->get_target()) == 0) {
        bool src_above_bottom = bottom.orientation(seg->get_source()) == 1;
        bool tgt_right_of_left = left_cut.orientation(seg->get_target()) == 1;
        bool tgt_left_of_right = right_cut.orientation(seg->get_target()) < 1;
        
        if (src_above_bottom && tgt_right_of_left && tgt_left_of_right)
            return true;
        else
            return false;
    }

    

    std::cout << "this is not where I want to be...\n";
    return false;
}

template class BoundingTrap<PointCart, int>;