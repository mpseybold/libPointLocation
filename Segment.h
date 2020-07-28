#pragma once

#include "Point_2D.h"

template <class PointType, class OrderType>
class Segment {

    private:
    
        PointType source;
        PointType target;
        OrderType priority;
    
    public:

        Segment(PointType s, PointType t, OrderType pri) : source(s), target(t), priority(pri) {
            if (PointType::v_orientation(s, t) == 1) {
                source = s;
                target = t;
            } else {
                source = t;
                target = s;
            }
        }

        PointType get_source() { return source; }
        PointType get_target() { return target; }
        OrderType get_priority() { return priority; }

        static int slope_comparison(
            Segment<PointType, OrderType> seg, Segment<PointType, OrderType> other_seg
        ) { 
            return PointType::slope_comparison(
                seg.get_source(), seg.get_target(),
                other_seg.get_source(), other_seg.get_target()
            ); 
        }

        bool operator== (Segment<PointType, OrderType>& other_seg);

        // returns -1 if other_seg is lexicographically smaller than seg
        // returns 1 otherwise
        static int lexicographic_comparison(Segment<PointType, OrderType> seg, Segment<PointType, OrderType> other_seg) {
            return -1;
        }

        bool operator< (Segment<PointType, OrderType>& other) {
            // auto this_add = reinterpret_cast<long>(this);
            // auto other_add = reinterpret_cast<long>(&other);
            // return this_add > other_add;
            return false; 
        }
};

#include "Segment.cpp"
