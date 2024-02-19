#pragma once

#include "FirstLayerNode.h"
#include "ElmInt.h"
#include "LeafDataAVL.h"
#include "SecondLayerNode.h"
namespace SegmentDS {
    class FirstLayerNode {

        private:

            FirstLayerNode* left;
            FirstLayerNode* right;
            FirstLayerNode* parent;
            ElmInt key;
            int height;

            ElmInt union_interval;
            LeafDataAVL<SecondLayerNode> second_layer_structure;

        public:

            void set_union_interval(ElmInt interval) {
                union_interval = interval;
            };
            ElmInt& get_union_interval() { return union_interval; }

            ~FirstLayerNode() {}

            FirstLayerNode() {
                left = nullptr;
                right = nullptr;
                parent = nullptr;
                height = 1;
                key = ElmInt(0, 0, CC);

                second_layer_structure = LeafDataAVL<SecondLayerNode>();
            }

            FirstLayerNode(ElmInt _key) {
                left = nullptr;
                right = nullptr;
                parent = nullptr;
                height = 1;
                key = _key;
            }

            void insert_segment(Segment* seg) {
                auto node = new SecondLayerNode(union_interval, *seg);
                second_layer_structure.insert(node);
            }

            ElmInt& get_key() {
                return key;
            }

            void set_key(ElmInt _key) {
                key = _key;
            }

            FirstLayerNode* get_left() {
                return left;
            }

            FirstLayerNode* get_right() {
                return right;
            }

            FirstLayerNode* get_parent() {
                return parent;
            }

            void set_parent(FirstLayerNode* other) {
                parent = other;
            }

            void set_left(FirstLayerNode* other) {
                left = other;
                if (other != nullptr)
                    other->set_parent(this);
            }

            void set_right(FirstLayerNode* other) {
                right = other;
                if (other != nullptr)
                    other->set_parent(this);
            }

            void copy_data(FirstLayerNode* other) {
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

            void vertical_query(Segment query_seg, std::vector<Segment>& output) {
                
                node_visits++;
                
                if (union_interval.contains(query_seg.x1())) {
                    if (second_layer_structure.get_root() != nullptr) {
                        second_layer_structure.get_root()
                        ->vertical_query(query_seg, output);
                    }
                }

                if (left != nullptr && left->get_union_interval()
                .contains(query_seg.x1())) {
                    left->vertical_query(query_seg, output);
                }

                if (right != nullptr && right->get_union_interval()
                .contains(query_seg.x1())) {
                    right->vertical_query(query_seg, output);
                }
            }

            void count_segs() {
                if (second_layer_structure.get_root() != nullptr) {
                    if (second_layer_structure.get_root()->get_height() > 5)
                        std::cout << "interesting...\n";
                    second_layer_structure.get_root()->count_segs();
                }

                if (left != nullptr)
                    left->count_segs();
                if (right != nullptr)
                    right->count_segs();
            }
        };
}