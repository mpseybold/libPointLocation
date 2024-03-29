#pragma once
#include "Segment.hpp"

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

        bool operator== (Cut<PointType, OrderType>& other_cut);

        CutType get_cut_type() { return cut_type; } 

        Segment<PointType, OrderType>* get_segment() { return segment; }

        Segment<PointType, OrderType>* get_intersecting_seg() { return intersecting_seg; }

        OrderType get_priority() { return segment->get_priority(); }

        int orientation(PointType p);

        int orientation(Segment<PointType, OrderType>* seg, int endpoint) {
            assert(cut_type == EDGE);
            PointType p = endpoint == 0 ? seg->get_source() : seg->get_target();
            PointType other_p = endpoint == 0 ? seg->get_target() : seg->get_source();
            
            if (orientation(p) == 0) {
                if (orientation(other_p) != 0) {
                    return orientation(other_p);
                }
                return has_seg_on_pos_side(seg) ? 1 : -1;
            }

            return orientation(p);
        }

        int defining_point_cut_comparison(Cut<PointType, OrderType> other_cut);

        //TODO: implement this properly
        bool intersects_segment(Segment<PointType, OrderType>* seg) { return true; }

        static int v_cut_edge_orientation(Cut<PointType, OrderType>& v_cut, Cut<PointType, OrderType>& e_cut);
        bool has_seg_on_pos_side(Segment<PointType, OrderType>* seg);
        bool has_seg_on_neg_side(Segment<PointType, OrderType>* seg);
        bool has_on(Segment<PointType, OrderType>* seg);
};