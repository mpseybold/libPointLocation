#pragma once

#include "geom.h"

// enum CutType { EDGE, SOURCE, TARGET, INTERSECTION };

// template <class PointType, class LineType>
// class Cut {
    
//     private:

//         CutType cut_type;
//         Segment<PointType>* segment;
//         Segment<PointType>* intersecting_seg;
//         LineType defining_line;

//     public:

//         Cut(CutType ct, Segment<PointType>* s, Segment<PointType>* is) :
//         cut_type(ct), segment(s), intersecting_seg(is) {

//             LineType line_1 = LineType(s->get_source(), s->get_target());

//         }

//         CutType get_cut_type() { return cut_type; } 

//         Segment<PointType>* get_segment() { return segment; }

//         Segment<PointType>* get_intersecting_seg() { return intersecting_seg; }



// };