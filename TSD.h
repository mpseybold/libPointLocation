#pragma once
#include "Node.h"
#include "Segment.hpp"
#include "BoundingTrap.h"
<<<<<<< HEAD
#include <stack>
=======
#include<vector>
>>>>>>> 5af8a9bf0016634b5828fdca7032381fc1cea160

template <class PointType, class OrderType>
class TSD {

    private:
        
        Node<PointType, OrderType>* root;
        std::vector<Segment<PointType, OrderType>> segments;

        void v_part_handle_E_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut);
        void v_part_handle_leaf_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut);
        void v_part_handle_V_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut);
        void v_part_handle_VE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut);
        void v_part_handle_EV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut);
        void v_part_handle_VVE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut);

        Node<PointType, OrderType>* v_merge_left_lower_priority_case(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);
        Node<PointType, OrderType>* v_merge_right_lower_priority_case(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);
        Node<PointType, OrderType>* v_merge_equal_priority_case(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);

        void partition_E_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut);
        void partition_leaf_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut);
        void partition_V_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut);
        void partition_VV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut);
        void partition_VE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut);
        void partition_EV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut);        
        void partition_VVE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& e_cut);
        
        void search_refinement(Segment<PointType, OrderType>* seg, Node<PointType, OrderType>* node);

        std::vector<Node<PointType, OrderType>*> intersecting_descendants = { nullptr, nullptr, nullptr, nullptr };
        std::stack<Node<PointType, OrderType>*> search_stack;
        std::vector<Node<PointType, OrderType>*> subdag_roots;
        std::vector<Node<PointType, OrderType>*> visited_nodes;
        std::vector<Node<PointType, OrderType>*> partition_visited_nodes;

    public:

        TSD();

        Node<PointType, OrderType>* get_root() { return root; }
        std::vector<Node<PointType, OrderType>*> get_subdag_roots() { return subdag_roots; }

        void insert_segment(Segment<PointType, OrderType> seg);

        void delete_segment(Segment<PointType, OrderType> seg);

        void affected_subdag_roots(Segment<PointType, OrderType>* seg);

        Node<PointType, OrderType>* v_merge(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);

        void v_partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& v_cut);        

        Node<PointType, OrderType>* merge(Node<PointType, OrderType>* below, Node<PointType, OrderType>* above);

        void partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType>& cut);
};