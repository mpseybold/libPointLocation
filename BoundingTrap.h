#pragma once

#include "V_Cut.h"
#include <list>

enum TrapType { BRTL, BTL, BRT, NONE };

template <class PointType, class OrderType>
class BoundingTrap {

    private:
    
        Cut<PointType, OrderType>* top = nullptr;
        Cut<PointType, OrderType>* bottom = nullptr;
        V_Cut<PointType, OrderType>* left = nullptr;
        V_Cut<PointType, OrderType>* right = nullptr;

        int left_side = 0;
        int right_side = 0;

        TrapType type;

        std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> 
        vertical_destruction(V_Cut<PointType, OrderType>* cut, int side);

        std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> 
        edge_destruction(Cut<PointType, OrderType>* cut);

        int trap_cut_region(Segment<PointType, OrderType>* seg, int endpoint);
        std::pair<int, int> trap_cut_regions(Segment<PointType, OrderType>* seg);

        // Cut<PointType, OrderType>& _top() { return *top; }
        // Cut<PointType, OrderType>& _bottom() { return *bottom; }
        // Cut<PointType, OrderType>& _left() { return *left; }
        // Cut<PointType, OrderType>& _right() { return *right; }

    public:

        BoundingTrap(
            Cut<PointType, OrderType>* _bottom,
            V_Cut<PointType, OrderType>* _right,
            Cut<PointType, OrderType>* _top,
            V_Cut<PointType, OrderType>* _left,
            int _left_side, int _right_side
        ) : top(_top), bottom(_bottom), left(_left), right(_right),
        left_side(_left_side), right_side(_right_side) { 

            assert(top != nullptr);
            assert(bottom != nullptr);
            if (left == nullptr)
                assert(left != nullptr);
            assert(right != nullptr);

            if (top->get_segment()->get_source().x() != top->get_segment()->get_target().x() 
            && bottom->get_segment()->get_source().x() != bottom->get_segment()->get_target().x()
            && Segment<PointType, OrderType>::slope_comparison(*top->get_segment(), *bottom->get_segment()) != 0) {
                auto intersection = Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment());
                if (!(intersection.defining_point_cut_comparison(*get_left()) < 1 || intersection.defining_point_cut_comparison(*get_right()) > -1))
                    assert(intersection.defining_point_cut_comparison(*get_left()) < 1 || intersection.defining_point_cut_comparison(*get_right()) > -1);
            }

            int slope_comp = Segment<PointType, OrderType>::slope_comparison(
                *top->get_segment(), *bottom->get_segment()); 

            auto aux_int = Cut<PointType, OrderType>(INTERSECTION, top->get_segment(), bottom->get_segment());

            // if (slope_comp == 0) {
            //     if (left == nullptr || right == nullptr) {
            //         std::cout << "not good...\n"
            //     }
            //     assert(left != nullptr && right != nullptr);
            // }

            if (slope_comp != 0 && aux_int.defining_point_cut_comparison(*get_left()) == 0) {
                type = BRT;
            } else if (slope_comp != 0 && aux_int.defining_point_cut_comparison(*get_right()) == 0) {
                type = BTL;
            } else {
                type = BRTL;
            } 
        }

        BoundingTrap() { 
            type = TrapType::NONE;
            left = nullptr;
            right = nullptr;
            top = nullptr;
            bottom = nullptr;
        }

        void set_type(TrapType _type) { type = _type; }
        TrapType get_type() { return type; };

        Cut<PointType, OrderType>* get_top() { return top; }
        Cut<PointType, OrderType>* get_bottom() { 
            if (bottom == nullptr)
                std::cout << "hmmm...\n";
            return bottom; }
        Cut<PointType, OrderType>* get_left() { return left->get_cut(left_side); }
        Cut<PointType, OrderType>* get_right() { return right->get_cut(right_side); }

        V_Cut<PointType, OrderType>* get_v_left() { return left; }
        V_Cut<PointType, OrderType>* get_v_right() { return right; }

        int get_left_side() { return left_side; }
        int get_right_side() { return right_side; }

        void set_top(Cut<PointType, OrderType>* cut) { top = cut; }
        void set_bottom(Cut<PointType, OrderType>* cut) { bottom = cut; }
        void set_right(V_Cut<PointType, OrderType>* cut, int side) { 
            right_side = side;
            right = cut; 
        }
        void set_left(V_Cut<PointType, OrderType>* cut, int side) { 
            left_side = side;
            left = cut; 
        }
        std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> destroy(Cut<PointType, OrderType>* cut);
        std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> destroy(V_Cut<PointType, OrderType>* v_cut, int side);

        static BoundingTrap<PointType, OrderType> vertical_merge(
            BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2
        );

        static BoundingTrap<PointType, OrderType> edge_merge(
            BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2
        );

        static BoundingTrap<PointType, OrderType> merge(BoundingTrap<PointType, OrderType> trap_1, BoundingTrap<PointType, OrderType> trap_2);

        bool contains_segment(Segment<PointType, OrderType>* seg);
        bool intersect_corner(Segment<PointType, OrderType>* seg);
        bool through_corner(Segment<PointType, OrderType>* seg);
        bool intersect_seg_trap(Segment<PointType, OrderType>* seg);
        bool intersect_seg_biangle(Segment<PointType, OrderType>* seg);
        bool intersect_seg_triangle(Segment<PointType, OrderType>* seg);

        bool intersects_segment(Segment<PointType, OrderType>* seg);

        bool contains_defining_point(Cut<PointType, OrderType> cut) { return true; }

        bool contains_endpoint(Segment<PointType, OrderType>* seg, int endpoint);

        bool contains_endpoint_strict(Segment<PointType, OrderType>* seg, int endpoint);

        bool seg_intersects_top(Segment<PointType, OrderType>* seg);

        bool seg_intersects_bottom(Segment<PointType, OrderType>* seg);

        bool is_degen();

        bool through_common_corner(BoundingTrap<PointType, OrderType>& other, Segment<PointType, OrderType>* seg);

        bool contains_defining_point(Cut<PointType, OrderType>* cut);
};