#pragma once
#include "geom.h"

enum CutType { EDGE, SOURCE, TARGET, INTERSECTION };

template <class PointType, class OrderType>
class Cut {
    
    private:

        CutType cut_type;
        Segment<PointType, OrderType>* segment;
        Segment<PointType, OrderType>* intersecting_seg;

    public:

        Cut(CutType ct, Segment<PointType, OrderType>* s, Segment<PointType, OrderType>* is) :
        cut_type(ct), segment(s), intersecting_seg(is) {
            if (ct == INTERSECTION) {
                assert(is != nullptr);
            }
        }

        CutType get_cut_type() { return cut_type; } 

        Segment<PointType, OrderType>* get_segment() { return segment; }

        Segment<PointType, OrderType>* get_intersecting_seg() { return intersecting_seg; }

        OrderType priority() { return segment->get_priority(); }

        int orientation(PointType p);
};