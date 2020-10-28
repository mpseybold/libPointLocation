#pragma once

#include "Cut.h"

template <class PointType, class OrderType>
class V_Cut {

    private:

        Cut<PointType, OrderType>* up;
        Cut<PointType, OrderType>* down;
        std::list<Segment<PointType, OrderType>> segs;

    public:

        V_Cut(CutType type, Segment<PointType, OrderType>* seg, Segment<PointType, OrderType>* int_seg)) {
            assert(type != EDGE);
            auto new_cut = new Cut<PointType, OrderType>(type, seg, int_seg);
            up = new_cut;
            down = new_cut;
        }

        int defining_point_cut_comparison(Cut<PointType, OrderType>& cut) {
            if (up != nullptr)
                return up->defning_point_cut_comparison(cut);
            else if (down != nullptr)
                return down->defining_point_cut_comparison(cut);

            assert(false);
        }
    
        void set_up(Cut<PointType, OrderType>* _up) { up = _up; }
        void set_down(Cut<PointType, OrderType>* _down) { down = _down; }
}