#pragma once

#include "Cut.h"

enum TrapType { BRTL, BTL, BRT, NONE };

template <class PointType, class OrderType>
class BoundingTrap {

    private:
    
        Cut<PointType, OrderType> top, bottom, left, right;

        TrapType type;

        std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> 
        vertical_destruction(Cut<PointType, OrderType> cut);

        std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> 
        edge_destruction(Cut<PointType, OrderType> cut);

        int trap_cut_region(Segment<PointType, OrderType>* seg, int endpoint);
        std::pair<int, int> trap_cut_regions(Segment<PointType, OrderType>* seg);

    public:

        BoundingTrap(
            Cut<PointType, OrderType> bottom,
            Cut<PointType, OrderType> right,
            Cut<PointType, OrderType> top,
            Cut<PointType, OrderType> left
        ) : top(top), bottom(bottom), left(left), right(right){ 
            if (left.get_cut_type() == NO_CUT) {
                assert(right.get_cut_type() != NO_CUT);
                assert(top.get_cut_type() != NO_CUT);
                assert(bottom.get_cut_type() != NO_CUT);
                type = BRT;
            } else if (right.get_cut_type() == NO_CUT) {
                assert(left.get_cut_type() != NO_CUT);
                assert(top.get_cut_type() != NO_CUT);
                assert(bottom.get_cut_type() != NO_CUT);
                type = BTL;
            } else {
                assert(left.get_cut_type() != NO_CUT);
                assert(right.get_cut_type() != NO_CUT);
                assert(top.get_cut_type() != NO_CUT);
                assert(bottom.get_cut_type() != NO_CUT);
                type = BRTL;
            } 
        }

        BoundingTrap() { 
            type = TrapType::NONE;
        }

        void set_type(TrapType _type) { type = _type; }
        TrapType get_type() { return type; }

        Cut<PointType, OrderType>& get_top() { return top; }
        Cut<PointType, OrderType>& get_bottom() { return bottom; }
        Cut<PointType, OrderType>& get_left() { return left; }
        Cut<PointType, OrderType>& get_right() { return right; }

        void set_top(Cut<PointType, OrderType> cut) { top = cut; }
        void set_bottom(Cut<PointType, OrderType> cut) { bottom = cut; }
        void set_right(Cut<PointType, OrderType> cut) { right = cut; }
        void set_left(Cut<PointType, OrderType> cut) { left = cut; }

        std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> destroy(Cut<PointType, OrderType> cut);

        static BoundingTrap<PointType, OrderType> vertical_merge(
            BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2
        );

        static BoundingTrap<PointType, OrderType> edge_merge(
            BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2
        );

        static BoundingTrap<PointType, OrderType> merge(BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2);

        bool contains_segment(Segment<PointType, OrderType>* seg);
        bool intersect_corner(Segment<PointType, OrderType>* seg);
        bool intersect_seg_trap(Segment<PointType, OrderType>* seg);
        bool intersect_seg_biangle(Segment<PointType, OrderType>* seg);
        bool intersect_seg_triangle(Segment<PointType, OrderType>* seg);

        bool intersects_segment(Segment<PointType, OrderType>* seg);

        bool contains_defining_point(Cut<PointType, OrderType> cut) { return true; }

        bool contains_endpoint(Segment<PointType, OrderType>* seg, int endpoint);

        bool seg_intersects_top(Segment<PointType, OrderType>* seg);

        bool seg_intersects_bottom(Segment<PointType, OrderType>* seg);

        bool is_degen();
};