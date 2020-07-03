#pragma once

#include "geom.h"

enum CutType { EDGE, SOURCE, TARGET, INTERSECTION };

template <class PointType, class LineType>
class Cut {
    
    private:

        CutType cut_type;
        Segment<PointType>* segment;
        Segment<PointType>* intersecting_seg;
        LineType defining_line;
        LineType intersecting_line;

    public:

        Cut(CutType ct, Segment<PointType>* s, Segment<PointType>* is) :
        cut_type(ct), segment(s), intersecting_seg(is) {

            defining_line = LineType(s->get_source(), s->get_target());

            if (is != nullptr) {
                intersecting_line = LineType(is->get_source(), is->get_target());
            }
        }

        CutType get_cut_type() { return cut_type; } 

        Segment<PointType>* get_segment() { return segment; }

        Segment<PointType>* get_intersecting_seg() { return intersecting_seg; }

        bool has_on_positive_side(PointType p) {
            switch(cut_type) {
                case SOURCE:
                    return segment->get_source().has_on_positive_side(p);
                case TARGET:
                    return segment->get_target().has_on_positive_side(p);
                case EDGE:
                    return defining_line.has_on_positive_side(p);
                case INTERSECTION:
                    return defining_line
                    .intersect_has_on_positive_side(intersecting_line, p);
            }
        }

};