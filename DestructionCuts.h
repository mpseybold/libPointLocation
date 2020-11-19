#pragma once

// #include "geom.h"
#include "Cut.h"

enum DestructionPattern { VVE, VV, V, VE, EV, E, NO_DESTRUCTION };

template <class PointType, class OrderType>

class DestructionCuts {
    private:
        V_Cut<PointType, OrderType>* v_1 = nullptr;
        V_Cut<PointType, OrderType>* v_2 = nullptr;
        Cut<PointType, OrderType>* e = nullptr;

    public:
        DestructionCuts() {
            v_1 = nullptr;
            v_2 = nullptr;
            e = nullptr;
        }

        void set_v_1(V_Cut<PointType, OrderType>* cut) {
            if (cut != nullptr)
                assert(cut->get_cut(0)->get_cut_type() != EDGE && cut != nullptr);
            v_1 = cut;
        }
        void set_v_2(V_Cut<PointType, OrderType>* cut) {
            if (cut != nullptr)
                assert(cut->get_cut(0)->get_cut_type() != EDGE && cut != nullptr);
            v_2 = cut;
        }
        void set_e(Cut<PointType, OrderType>* cut) {
            if (cut != nullptr)
                assert(cut->get_cut_type() == EDGE);
            e = cut;
        }

        DestructionPattern get_pattern() {
            if (no_cuts())
                return NO_DESTRUCTION;

            if (v_1 != nullptr) {
                if (v_2 != nullptr) {
                    if (e != nullptr) {
                        return VVE;
                    } else {
                        return VV;
                    }
                } else if ( e == nullptr) {
                    return V;
                } else {
                    return VE;
                }
            } else {
                if (v_2 != nullptr) {
                    if (e != nullptr) {
                        return EV;
                    } else {
                        throw std::logic_error("v_2 cannot be the only destruction cut");
                    }   
                } else if (e != nullptr) {
                    return E;
                }
            }
        }

        V_Cut<PointType, OrderType>* get_v_1() { return v_1; }
        V_Cut<PointType, OrderType>* get_v_2() { return v_2; }
        Cut<PointType, OrderType>* get_e() { return e; }

        void clear_e() { e = nullptr; }
        void clear_v_1() { v_1 = nullptr; }
        void clear_v_2() { v_2 = nullptr; }

        bool no_cuts() {
            return v_1 == nullptr &&
            v_2 == nullptr &&
            e == nullptr;
        }

        OrderType priority() {
            if (no_cuts()) 
                return 100000;
            
            

            if (e != nullptr)
                return e->get_priority();
            if (v_1 != nullptr)
                return v_1->get_priority();
            if (v_2 != nullptr)
                return v_2->get_priority();
        }
};