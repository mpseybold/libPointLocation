#pragma once


/*
* A generic implemntation of an AVL tree
* which stores data in leaves
* 
* The class NodeType must implement the following functions:
* 
* NodeType* get_right()
* void set_right(NodeType* _right)
* NodeType* get_left()
* void set_right(NodeType* _right)
* NodeType* get_parent()
* void set_parent(NodeType* parent)
* 
* void copy_data(NodeType* other) copies the children 
* and parent pointers from other as well as any internal
* data required for the comp function
*
* The comp function should return 1 if the key of the first
* argument is strictly bigger than that of the second arugument
* and -1 otherwise.
*
*/

namespace SegmentDS {

    template <class NodeType>
    class LeafDataAVL {

        private:

            NodeType* root=nullptr;

            void ascend(NodeType* node);

            void repair(NodeType* node);

            void rotate_Left(NodeType* X, NodeType* Z);

            void rotate_Right(NodeType* X, NodeType* Z);

            void rotate_RightLeft(NodeType* X, NodeType* Z);

            void rotate_LeftRight(NodeType* X, NodeType* Z);

            NodeType* find_leaf(NodeType* query_node, NodeType* root_of_search);

        public:

            void insert(NodeType* node);

            NodeType* get_root() { return root; }

            void post_order(NodeType* node);

    };
}

#include "LeafDataAVL.cpp"
