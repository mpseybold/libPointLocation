#pragma once

#include "Node.h"
#include "Segment.h"
// #include "geom.h"
#include "BoundingTrap.h"
#include <stack>
#include <list>
#include "io.h"

template <class PointType, class OrderType>
class TSD {

    private:
        
        Node<PointType, OrderType>* root;
        std::vector<Segment<PointType, OrderType>> segments;

        void v_part_handle_E_case(Node<PointType, OrderType>* node, V_Cut<PointType, OrderType>* v_cut, int side);
        void v_part_handle_leaf_case(Node<PointType, OrderType>* node, V_Cut<PointType, OrderType>* v_cut, int side);
        void v_part_handle_V_case(Node<PointType, OrderType>* node, V_Cut<PointType, OrderType>* v_cut, int side);
        void v_part_handle_VE_case(Node<PointType, OrderType>* node, V_Cut<PointType, OrderType>* v_cut, int side);
        void v_part_handle_EV_case(Node<PointType, OrderType>* node, V_Cut<PointType, OrderType>* v_cut, int side);
        void v_part_handle_VVE_case(Node<PointType, OrderType>* node, V_Cut<PointType, OrderType>* v_cut, int side);

        Node<PointType, OrderType>* v_merge_left_lower_priority_case(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);
        Node<PointType, OrderType>* v_merge_right_lower_priority_case(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);
        Node<PointType, OrderType>* v_merge_equal_priority_case(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);

        Node<PointType, OrderType>* merge_equal_priority_case(Node<PointType, OrderType>* below, Node<PointType, OrderType>* above);

        void partition_E_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        void partition_leaf_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        void partition_V_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        void partition_VV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        void partition_VE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        void partition_EV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);        
        void partition_VVE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        
        void search_refinement(Segment<PointType, OrderType>* seg, Node<PointType, OrderType>* node);

        std::vector<Node<PointType, OrderType>*> intersecting_descendants = { nullptr, nullptr, nullptr, nullptr };
        std::stack<Node<PointType, OrderType>*> search_stack;
        std::vector<Node<PointType, OrderType>*> subdag_roots;
        std::vector<Node<PointType, OrderType>*> visited_nodes;
        std::vector<Node<PointType, OrderType>*> partition_visited_nodes;

        std::list<Cut<PointType, OrderType>> v_cuts;

        int leaf_count;

    public:

        TSD();

        Node<PointType, OrderType>* get_root() { return root; }
        std::vector<Node<PointType, OrderType>*> get_subdag_roots() { return subdag_roots; }

        void insert_segment(Segment<PointType, OrderType>& seg);

        void delete_segment(Segment<PointType, OrderType>& seg);

        void affected_subdag_roots(Segment<PointType, OrderType>* seg, bool insert);

        Node<PointType, OrderType>* v_merge(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);

        void v_partition(Node<PointType, OrderType>* node, V_Cut<PointType, OrderType>* v_cut, int side);        

        Node<PointType, OrderType>* merge(Node<PointType, OrderType>* below, Node<PointType, OrderType>* above);

        void partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* cut);

        int get_leaf_count() { return leaf_count; }

        V_Cut<PointType, OrderType>* find_v_cut(Cut<PointType, OrderType>* cut, Node<PointType, OrderType>* node);
};

#include "TSD.cpp"
#include "TSD_algorithms/partition.cpp"
#include "TSD_algorithms/v_partition.cpp"
#include "TSD_algorithms/merge.cpp"
#include "TSD_algorithms/v_merge.cpp"