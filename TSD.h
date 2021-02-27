#pragma once

#include "Node.h"
#include "Segment.h"
// #include "geom.h"
#include "BoundingTrap.h"
#include <stack>
#include <list>
#include <unordered_map>
#include "io.h"

#define TESTING 1000



enum MergeSide {LEFT, RIGHT};
    
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
        Node<PointType, OrderType>* v_merge_partial(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);
        Node<PointType, OrderType>* merge_equal_priority_case(Node<PointType, OrderType>* below, Node<PointType, OrderType>* above);

        void partition_E_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut, Node<PointType, OrderType>* parent);
        void partition_leaf_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        void partition_V_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        void partition_VV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        void partition_VE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);
        void partition_EV_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);        
        void partition_VVE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* e_cut);

        void shift_merge_list(Node<PointType, OrderType>* node, int desc, int side);
        
        void search_refinement(Segment<PointType, OrderType>* seg, Node<PointType, OrderType>* node);

        std::vector<Node<PointType, OrderType>*> intersecting_descendants = { nullptr, nullptr, nullptr, nullptr };
        std::stack<Node<PointType, OrderType>*> search_stack;
        std::vector<Node<PointType, OrderType>*> subdag_roots;
        std::vector<Node<PointType, OrderType>*> visited_nodes;
        std::vector<Node<PointType, OrderType>*> partition_visited_nodes;

        std::list<Cut<PointType, OrderType>> v_cuts;

        std::set<Node<PointType, OrderType>*> retired_nodes = std::set<Node<PointType, OrderType>*>();

        void walk_to_fix_desc(Node<PointType, OrderType>* node, Node<PointType, OrderType>* new_desc, Node<PointType, OrderType>* old_desc);

        int leaf_count;

    public:

        TSD();

        std::string visMe;

        Node<PointType, OrderType>* get_root() { return root; }
        std::vector<Node<PointType, OrderType>*> get_subdag_roots() { return subdag_roots; }

        void insert_segment(Segment<PointType, OrderType>& seg);

        void delete_segment(Segment<PointType, OrderType>& seg);

        void affected_subdag_roots(Segment<PointType, OrderType>* seg, bool insert);

        Node<PointType, OrderType>* v_merge(Node<PointType, OrderType>* left, Node<PointType, OrderType>* right);

        void v_partition(Node<PointType, OrderType>* node, V_Cut<PointType, OrderType>* v_cut, int side);        

        Node<PointType, OrderType>* merge(Node<PointType, OrderType>* below, Node<PointType, OrderType>* above);

        void partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType>* cut, Node<PointType, OrderType>* parent);

        int get_leaf_count() { return leaf_count; }

        V_Cut<PointType, OrderType>* find_v_cut(Cut<PointType, OrderType>* cut, Node<PointType, OrderType>* node);
        const std::string asJsonGraph(std::vector<Node<PointType, OrderType>*> roots, Node<PointType, OrderType>* node_of_interest=nullptr);
        const std::string asJsonGraph(){
            std::vector<Node<PointType, OrderType>*> roots = {root};
            return asJsonGraph(roots, nullptr);
        }

        void reachable_nodes_valid(Node<PointType, OrderType>* node);

        void delete_node(Node<PointType, OrderType>* node);

        void cleanup();

        bool is_reachable(Node<PointType, OrderType>* from, Node<PointType, OrderType>* to);

        bool assert_has_parent(Node<PointType, OrderType>* node, 
        Node<PointType, OrderType>* parent, 
        Node<PointType, OrderType>* root_of_search);
};

#include "TSD.cpp"
#include "TSD_algorithms/partition.cpp"
#include "TSD_algorithms/v_partition.cpp"
#include "TSD_algorithms/merge.cpp"
#include "TSD_algorithms/v_merge.cpp"