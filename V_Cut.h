#pragma once

#include "Cut.h"

template <class PointType, class OrderType>
class V_Cut {

    private:

        Cut<PointType, OrderType>* up;
        Cut<PointType, OrderType>* down;
        std::list<Segment<PointType, OrderType>*> segs;

    public:

        V_Cut(CutType type, Segment<PointType, OrderType>* seg, Segment<PointType, OrderType>* int_seg) {
            assert(type != EDGE);
            auto new_cut = new Cut<PointType, OrderType>(type, seg, int_seg);
            up = new_cut;
            down = new_cut;
        }

        int defining_point_cut_comparison(V_Cut<PointType, OrderType>& cut) {
            if (up != nullptr)
                return up->defining_point_cut_comparison(*cut.get_cut(1));
            else if (down != nullptr)
                return down->defining_point_cut_comparison(*cut.get_cut(1));

            assert(false);
        }

     
        void set_up(Cut<PointType, OrderType>* _up) { up = _up; }
        void set_down(Cut<PointType, OrderType>* _down) { down = _down; }

        void insert_intersection(CutType type, 
            Segment<PointType, OrderType>* seg, 
            Segment<PointType, OrderType>* int_seg) {
                
                assert(seg != nullptr);
                if (segs.empty() || std::find(segs.begin(), segs.end(), seg) != segs.end()) {
                    segs.push_back(seg);
                }
                if (type == INTERSECTION) {
                    assert(int_seg != nullptr);
                    if (std::find(segs.begin(), segs.end(), int_seg) != segs.end()) {
                        segs.push_back(int_seg);
                    }
                }
                if (seg->get_priority() < up->get_priority()) {
                    if (type == INTERSECTION) {
                        up->set_type(type);
                        up->set_seg(seg);
                        up->set_intersecting_seg(int_seg);
                    } else if ((type == SOURCE && up->orientation(seg, 0) == 1) 
                        || (type == TARGET && up->orientation(seg, 1) == 1)) {
                            up->set_type(type);
                            up->set_seg(seg);
                            up->set_intersecting_seg(nullptr);
                    }
                }
                if (seg->get_priority() < down->get_priority()) {
                    if (type == INTERSECTION) {
                        down->set_type(type);
                        down->set_seg(seg);
                        down->set_intersecting_seg(int_seg);
                    } else if ((type == SOURCE && up->orientation(seg, 0) == -1) 
                        || (type == TARGET && up->orientation(seg, 1) == -1)) {
                            down->set_type(type);
                            down->set_seg(seg);
                            down->set_intersecting_seg(nullptr);
                    } 
                }
        }

        Cut<PointType, OrderType>* get_cut(int side) {
            if (side == 1)
                return up;
            if (side == -1)
                return down;

            return up;
        }

        OrderType get_priority() {
            return std::min(up->get_priority(), down->get_priority());
        }
};