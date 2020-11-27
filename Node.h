#pragma once

#include "BoundingTrap.h"
#include "DestructionCuts.h"

template <class PointType, class OrderType>
class Node {

    private:
        Node<PointType, OrderType>* negative_child; // delete
        Node<PointType, OrderType>* positive_child; // delete
        Node<PointType, OrderType>* L = nullptr;
        Node<PointType, OrderType>* R = nullptr;
        Node<PointType, OrderType>* A = nullptr;
        Node<PointType, OrderType>* B = nullptr;

        // TODO: can save these pointers
        Node<PointType, OrderType>* left = nullptr;
        Node<PointType, OrderType>* right = nullptr;

        Node<PointType, OrderType>* leaf_above;
        Node<PointType, OrderType>* leaf_below;
        Node<PointType, OrderType>* leaf_left;
        Node<PointType, OrderType>* leaf_right;

        DestructionCuts<PointType, OrderType> destruction_cuts;
        // Cut<PointType, OrderType> destroying_cut;
        BoundingTrap<PointType, OrderType> trapezoid;

        bool visited = false;
        bool partition_visited = false;
        bool v_partition_visited = false;

    public:

        //for debugging purposes only
        std::string name = "";

        Node(BoundingTrap<PointType, OrderType> trap) {
            trapezoid = trap;
            destruction_cuts = DestructionCuts<PointType, OrderType>();
        }

        DestructionCuts<PointType, OrderType>& get_destruction_cuts() { return destruction_cuts; }

        void set_v_1(V_Cut<PointType, OrderType>* cut) { destruction_cuts.set_v_1(cut); }
        void set_v_2(V_Cut<PointType, OrderType>* cut) { destruction_cuts.set_v_2(cut); }
        void set_e(Cut<PointType, OrderType>* cut) { destruction_cuts.set_e(cut); }

        void clear_e() { destruction_cuts.clear_e(); }
        void clear_v_1() { destruction_cuts.clear_v_1(); }
        void clear_v_2() { destruction_cuts.clear_v_2(); }

        void clear_cuts() {
            clear_e();
            clear_v_1();
            clear_v_2();
        };

        // clears cuts and descendant pointers but
        // not the bounding trapezoid!
        void clear() {
            clear_cuts();
            L = nullptr;
            R = nullptr;
            A = nullptr;
            B = nullptr;
        };

        void copy_node(Node<PointType, OrderType>* other) {
            if (other == nullptr)
                assert(other != nullptr);
            
            copy_cuts(other);
            
            set_desc(
                other->get_L(),
                other->get_R(),
                other->get_A(),
                other->get_B()
            );

            trapezoid = other->get_trapezoid();
        }

        void copy_cuts(Node<PointType, OrderType>* other) {
            assert(other != nullptr);
            // if (other->get_e() != nullptr)
                set_e(other->get_e());
            // if (other->get_v_1() != nullptr)
                set_v_1(other->get_v_1());
            // if (other->get_v_2() != nullptr)
                set_v_2(other->get_v_2());
        }

        V_Cut<PointType, OrderType>* get_v_1() { return destruction_cuts.get_v_1(); }
        V_Cut<PointType, OrderType>* get_v_2() { return destruction_cuts.get_v_2(); }
        Cut<PointType, OrderType>* get_e() { return destruction_cuts.get_e(); }

        bool is_leaf() { return destruction_cuts.no_cuts(); }

        OrderType get_priority() { 
            return destruction_cuts.priority(); 
        }
        
        bool is_visited() { return visited; }

        bool is_partition_visited() { return partition_visited; }
        
        bool is_v_partition_visited() { return v_partition_visited; }

        void toggle_visited() { visited = !visited; }

        void toggle_partition_visited() { partition_visited = !partition_visited; }

        void toggle_v_partition_visited() { v_partition_visited = !v_partition_visited; }

        bool contains(Segment<PointType, OrderType>* seg) { 
            //TODO: Implement this    
            return true; 
        }

        bool contains_endpoint(Segment<PointType, OrderType>* seg, int endpoint) {
            assert(seg != nullptr);
            return trapezoid.contains_endpoint_strict(seg, endpoint);
        }

        bool intersects_segment(Segment<PointType, OrderType>* seg) {
            return trapezoid.intersects_segment(seg);
        }

        bool seg_intersects_top(Segment<PointType, OrderType>* seg) { return trapezoid.seg_intersects_top(seg); }
        bool seg_intersects_bottom(Segment<PointType, OrderType>* seg) { return trapezoid.seg_intersects_bottom(seg); }

        // Cut<PointType, OrderType>& get_cut() { return destroying_cut; }
        BoundingTrap<PointType, OrderType>& get_trapezoid() { return trapezoid; }
        Node<PointType, OrderType>* get_negative_child() { return negative_child; }
        Node<PointType, OrderType>* get_positive_child() { return positive_child; }
        Node<PointType, OrderType>*& get_L() { return L; }
        Node<PointType, OrderType>*& get_R() { return R; }
        Node<PointType, OrderType>*& get_A() { return A; }
        Node<PointType, OrderType>*& get_B() { return B; }

        Node<PointType, OrderType>* get_left() { return left; }
        Node<PointType, OrderType>* get_right() { return right; }

        void set_negative_child(Node<PointType, OrderType>* node) { negative_child = node; }
        void set_positive_child(Node<PointType, OrderType>* node) { positive_child = node; }
        void set_L(Node<PointType, OrderType>* node) { L = node; }
        void set_R(Node<PointType, OrderType>* node) { R = node; }
        void set_A(Node<PointType, OrderType>* node) { A = node; }
        void set_B(Node<PointType, OrderType>* node) {  B = node; }

        void set_left(Node<PointType, OrderType>* _left) { left = _left; }
        void set_right(Node<PointType, OrderType>* _right) { right = _right; }

        void set_leaf_above(Node<PointType, OrderType>* node) { leaf_above = node; }
        void set_leaf_below(Node<PointType, OrderType>* node) { leaf_below = node; }
        void set_leaf_left(Node<PointType, OrderType>* node) { leaf_left = node; }
        void set_leaf_right(Node<PointType, OrderType>* node) { leaf_right = node; }

        Node<PointType, OrderType>* get_leaf_above() { return leaf_above; }
        Node<PointType, OrderType>* get_leaf_below() { return leaf_below; }
        Node<PointType, OrderType>* get_leaf_left() { return leaf_left; }
        Node<PointType, OrderType>* get_leaf_right() { return leaf_right; }

        //TODO: Implement this properly 
        DestructionPattern get_dest_pattern() { 
            return destruction_cuts.get_pattern();
        }

        void set_children(
            Node<PointType, OrderType>* pos_child, 
            Node<PointType, OrderType>* neg_child) {
                set_positive_child(pos_child);
                set_negative_child(neg_child);
        }

        void set_desc(
            Node<PointType, OrderType>* L,
            Node<PointType, OrderType>* R,
            Node<PointType, OrderType>* A,
            Node<PointType, OrderType>* B
        ) {
            this->L = L;
            this->R = R;
            this->A = A;
            this->B = B;
        }

        bool is_flat() {
            return trapezoid.get_top()->has_on(
                trapezoid.get_bottom()->get_segment()
            );
        }

        bool through_common_corner(Node<PointType, OrderType>* other_node, Segment<PointType, OrderType>* seg) {
            return trapezoid.through_common_corner(other_node->get_trapezoid(), seg);
        }
};