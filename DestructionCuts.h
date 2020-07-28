#pragma once

#include "geom.h"
#include "Cut.h"

enum DestructionPattern { VVE, VV, V, VE, EV, E, NO_DESTRUCTION };

template <class PointType, class OrderType>
class DestructionCuts {
    private:
        Cut<PointType, OrderType> v_1;
        Cut<PointType, OrderType> v_2;
        Cut<PointType, OrderType> e;

    public:
        DestructionCuts() {
            v_1 = Cut<PointType, OrderType>();
            v_2 = Cut<PointType, OrderType>();
            e = Cut<PointType, OrderType>();
        }

        void set_v_1(Cut<PointType, OrderType> cut) {
            assert(cut.get_cut_type() != EDGE && cut.get_cut_type() != NO_CUT);
            v_1 = cut;
        }
        void set_v_2(Cut<PointType, OrderType> cut) {
            assert(cut.get_cut_type() != EDGE && cut.get_cut_type() != NO_CUT);
            v_2 = cut;
        }
        void set_e(Cut<PointType, OrderType> cut) {
            assert(cut.get_cut_type() == EDGE);
            e = cut;
        }

        DestructionPattern get_pattern() {
            if (no_cuts())
                return NO_DESTRUCTION;

            if (v_1.get_cut_type() != NO_CUT) {
                if (v_2.get_cut_type() != NO_CUT) {
                    if (e.get_cut_type() != NO_CUT) {
                        return VVE;
                    } else {
                        return VV;
                    }
                } else {
                    return V;
                }
            } else {
                if (v_2.get_cut_type() != NO_CUT) {
                    if (e.get_cut_type() != NO_CUT) {
                        return EV;
                    } else {
                        throw std::logic_error("v_2 cannot be the only destruction cut");
                    }   
                } else if (e.get_cut_type() != NO_CUT) {
                    return E;
                }
            }
        }

        Cut<PointType, OrderType>& get_v_1() { return v_1; }
        Cut<PointType, OrderType>& get_v_2() { return v_2; }
        Cut<PointType, OrderType>& get_e() { return e; }

        void clear_e() { e = Cut<PointType, OrderType>(); }
        void clear_v_1() { v_1 = Cut<PointType, OrderType>(); }
        void clear_v_2() { v_2 = Cut<PointType, OrderType>(); }

        bool no_cuts() {
            return v_1.get_cut_type() == NO_CUT &&
            v_2.get_cut_type() == NO_CUT &&
            e.get_cut_type() == NO_CUT;
        }

        OrderType priority() {
            if (no_cuts()) 
                return 100000;
            
            if (e.get_cut_type() != NO_CUT)
                return e.get_priority();
            if (v_1.get_cut_type() != NO_CUT)
                return e.get_priority();
            if (v_2.get_cut_type() != NO_CUT)
                return e.get_priority();
        }
};