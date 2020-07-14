#pragma once
#include "Cut.h"
#include <utility>

enum TrapType { BRTL, BTL, BRT, NONE };

template <class PointType, class OrderType>
class BoundingTrap {

    private:
    
        Cut<PointType, OrderType> top, bottom, left, right;

        TrapType type;

    public:

        BoundingTrap(
            Cut<PointType, OrderType> top,
            Cut<PointType, OrderType> bottom,
            Cut<PointType, OrderType> left,
            Cut<PointType, OrderType> right
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

        std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> destroy(Cut<PointType, OrderType> cut);

        BoundingTrap<PointType, OrderType> merge(BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2);

        bool contains_segment(Segment<PointType, OrderType>* seg);

        bool contains_defining_point(Cut<PointType, OrderType> cut) { return true; }
};