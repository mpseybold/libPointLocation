#include "LeafDataAVL.h"

namespace SegmentDS {
    template <class NodeType>
    void LeafDataAVL<NodeType>::
    insert(NodeType* node) {
        if (root == nullptr) {
            root = node;
            return;
        }

        auto leaf = find_leaf(node, root);

        auto new_node = new NodeType();
        new_node->copy_data(leaf);
        if (leaf != root &&
            leaf == leaf->get_parent()->get_right())
                leaf->get_parent()->set_right(new_node);
        else if (leaf != root)
            leaf->get_parent()->set_left(new_node);
        
        new_node->set_key(node->get_key());

        auto right_key = leaf->get_key();
        if (node->get_key() > right_key) {
            new_node->set_right(node);
            new_node->set_left(leaf);
        } else {
            new_node->set_right(leaf);
            new_node->set_left(node);
        }

        // if (comp(node, leaf) == 1) {
        //     new_node->set_right(node);
        //     new_node->set_left(leaf);
        // } else {
        //     new_node->set_right(leaf);
        //     new_node->set_left(node);
        // }

        if (root == leaf)
            root = new_node;

        ascend(new_node);
    }

    template<class NodeType>
    void LeafDataAVL<NodeType>::
    rotate_Left(NodeType* X, NodeType* Z) {
        auto old_X_parent = X->get_parent();

        auto t23 = Z->get_left();
        X->set_right(t23);
        Z->set_left(X);

        if (X == root) {
            root = Z;
            Z->set_parent(nullptr);
        }
        else {
            if (old_X_parent->get_right() == X)
                old_X_parent->set_right(Z);
            else
                old_X_parent->set_left(Z);
        }
    }

    template<class NodeType>
    void LeafDataAVL<NodeType>::
    rotate_Right(NodeType* X, NodeType* Z) {
        auto old_X_parent = X->get_parent();

        auto t23 = Z->get_right();
        X->set_left(t23);
        Z->set_right(X);

        if (X == root) {
            root = Z;
            Z->set_parent(nullptr);
        }
        else {
            if (old_X_parent->get_right() == X)
                old_X_parent->set_right(Z);
            else
                old_X_parent->set_left(Z);
        }
    }

    template<class NodeType>
    void LeafDataAVL<NodeType>::
    rotate_RightLeft(NodeType* X, NodeType* Z) {

        auto old_X_parent = X->get_parent();

        auto Y = Z->get_left();
        auto t3 = Y->get_right();
        Z->set_left(t3);
        Y->set_right(Z);
        auto t2 = Y->get_left();
        X->set_right(t2);
        Y->set_left(X);

        Z->update_height();
        X->update_height();

        if (X == root) {
            root = Y;
            Y->set_parent(nullptr);
        }
        else {
            if (old_X_parent->get_right() == X)
                old_X_parent->set_right(Y);
            else
                old_X_parent->set_left(Y);
        }
    }

    template<class NodeType>
    void LeafDataAVL<NodeType>::
    rotate_LeftRight(NodeType* X, NodeType* Z) {
        auto old_X_parent = X->get_parent();

        auto Y = Z->get_right();
        auto t3 = Y->get_left();
        Z->set_right(t3);
        Y->set_left(Z);
        auto t2 = Y->get_right();
        X->set_left(t2);
        Y->set_right(X);

        Z->update_height();
        X->update_height();

        if (X == root) {
            root = Y;
            Y->set_parent(nullptr);
        }
        else {
            if (old_X_parent->get_right() == X)
                old_X_parent->set_right(Y);
            else
                old_X_parent->set_left(Y);
        }
    }

    template <class NodeType>
    void LeafDataAVL<NodeType>::
    repair(NodeType* node) {
        auto left = node->get_left();
        auto right = node->get_right();

        if (left->get_height() > right->get_height()) {
            if (left->get_bf() > 0)
                rotate_LeftRight(node, left);
            else
                rotate_Right(node, left);
        } else {
            if (right->get_bf() < 0)
                rotate_RightLeft(node, right);
            else
                rotate_Left(node, right);
        }
    }

    template<class NodeType>
    void LeafDataAVL<NodeType>::
    ascend(NodeType* node) {

        if (abs(node->get_bf()) >= 2)
            repair(node);

        node->update_height();
        if (node->get_parent() != nullptr)
            ascend(node->get_parent());

    }

    template <class NodeType>
    NodeType* LeafDataAVL<NodeType>::
    find_leaf(NodeType* query_node, NodeType* root_of_search) {
        auto right_key = root_of_search->get_key();
        if (query_node->get_key() > right_key) {
        // if (comp(query_node, root_of_search) == 1) {
            if (root_of_search->get_right() == nullptr)
                return root_of_search;
            else
                return find_leaf(query_node, root_of_search->get_right());
        } else {
            if (root_of_search->get_left() == nullptr)
                return root_of_search;
            else
                return find_leaf(query_node, root_of_search->get_left());
        }     
    }

    template <class NodeType>
    void LeafDataAVL<NodeType>::
    post_order(NodeType* node) {
        if (node->get_left() == nullptr && node->get_right() == nullptr) {
            std::cout << node->get_key() << std::endl;
            return;
        }
        if (node->get_left() != nullptr)
            post_order(node->get_left());
        if (node->get_right() != nullptr)
            post_order(node->get_right());
    }

}