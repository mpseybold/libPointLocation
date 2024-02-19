#pragma once

#include <algorithm>

template <class KeyType>
class AbstractNode {

    protected:
        AbstractNode* left;
        AbstractNode* right;
        AbstractNode* parent;
        KeyType key;
        int height;

    public:

        virtual ~AbstractNode() {};

        KeyType& get_key() {
            return key;
        }

        void set_key(KeyType _key) {
            key = _key;
        }

        AbstractNode* get_left() {
            return left;
        }

        AbstractNode* get_right() {
            return right;
        }

        AbstractNode* get_parent() {
            return parent;
        }

        void set_parent(AbstractNode* other) {
            parent = other;
        }

        void set_left(AbstractNode* other) {
            left = other;
            if (other != nullptr)
                other->set_parent(this);
        }

        void set_right(AbstractNode* other) {
            right = other;
            if (other != nullptr)
                other->set_parent(this);
        }

        void copy_data(AbstractNode* other) {
            parent = other->get_parent();
            left = other->get_left();
            right = other->get_right();
            key = other->get_key();
            height = other->get_height();
        }

        int get_height() {
            return height;
        }

        void update_height() {

            if (left == nullptr && right == nullptr)
                return;

            if (left == nullptr && right != nullptr)
                height = right->get_height() + 1;
            else if (right == nullptr && left != nullptr)
                height = left->get_height() + 1;
            else
                height = std::max(right->get_height(), left->get_height()) + 1;
        }

        int get_bf() {
            if (left == nullptr && right == nullptr)
                return 0;

            if (left == nullptr && right != nullptr)
                return -right->get_height();

            if (left != nullptr && right == nullptr)
                return -left->get_height();

            return right->get_height() - left->get_height();
        }
};

