#pragma once
#include "BoundingTrap.h"

template <class PointType, class OrderType>
class Node {

    private:

        Node<PointType, OrderType>* negative_child;
        Node<PointType, OrderType>* positive_child;
        Node<PointType, OrderType>* L;
        Node<PointType, OrderType>* R;
        Node<PointType, OrderType>* A;
        Node<PointType, OrderType>* B;

        Cut<PointType, OrderType> destroying_cut;
        BoundingTrap<PointType, OrderType> trapezoid;

    public:

        Node(Cut<PointType, OrderType> cut, BoundingTrap<PointType, OrderType> trapezoid) :
        destroying_cut(cut), trapezoid(trapezoid) {}

        bool is_leaf() { return destroying_cut.type == NO_CUT; }

        Node<PointType, OrderType>* get_negative_child() { return negative_child; }
        Node<PointType, OrderType>* get_positive_child() { return positive_child; }
        Node<PointType, OrderType>* get_L() { return L; }
        Node<PointType, OrderType>* get_R() { return R; }
        Node<PointType, OrderType>* get_A() { return A; }
        Node<PointType, OrderType>* get_B() { return B; }

        void set_negative_child(Node<PointType, OrderType>* node) { negative_child = node; }
        void set_positive_child(Node<PointType, OrderType>* node) { positive_child = node; }
        void set_L(Node<PointType, OrderType>* node) { L = node; }
        void set_R(Node<PointType, OrderType>* node) { R = node; }
        void set_A(Node<PointType, OrderType>* node) { A = node; }
        void set_B(Node<PointType, OrderType>* node) {  B = node; }

};