#pragma once
#include "Node.h"
#include "geom.h"

template <class PointType, class OrderType>
class TSD {

    private:
        
        Node<PointType, OrderType>* root;
        std::vector<Segment<PointType, OrderType>> segments;

    public:

        TSD();

        void insert_segment(Segment<PointType, OrderType> seg);

        void delete_segment(Segment<PointType, OrderType> seg);

        Node<PointType, OrderType>* deepest_node_containing_segment(Segment<PointType, OrderType>* seg);

        std::vector<Node<PointType, OrderType>*> affected_subdag_roots(Segment<PointType, OrderType>* seg);

        void v_merge(Node<PointType, OrderType>* node, Cut<PointType, OrderType> v_cut);

        void v_partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType> v_cut);        

        void merge(Node<PointType, OrderType>* node, Cut<PointType, OrderType> cut);

        void partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType> cut);
};