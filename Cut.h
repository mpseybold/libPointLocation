#pragma once
#include "geom.h"

enum CutType { EDGE, SOURCE, TARGET, INTERSECTION, NO_CUT };

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

        Cut() {
            cut_type = NO_CUT;
        }

        CutType get_cut_type() { return cut_type; } 

        Segment<PointType, OrderType>* get_segment() { return segment; }

        Segment<PointType, OrderType>* get_intersecting_seg() { return intersecting_seg; }

        OrderType get_priority() { return segment->get_priority(); }

        int orientation(PointType p);

        int defining_point_cut_comparrison(Cut<PointType, OrderType> other_cut) { return 1; }

        //TODO: implement this properly
        bool intersects_segment(Segment<PointType, OrderType>* seg) { return true; }
};