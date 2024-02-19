#pragma once

#include "Segment.h"
#include "LeafDataAVL.h"
#include "FirstLayerNode.h"
#include <vector>

namespace SegmentDS {

    class SegmentDS {
        private:

            LeafDataAVL<FirstLayerNode> first_layer_structure;

            void insert_segment(Segment* seg, FirstLayerNode* node) {
                if (node->get_union_interval().is_covered(*seg))
                    node->insert_segment(seg);
                else {
                    if (node->get_left() != nullptr 
                    && (node->get_left())
                    ->get_union_interval().intersects(*seg))
                        insert_segment(seg, (node->get_left()));
                    if (node->get_right() != nullptr 
                    && (node->get_right())
                    ->get_union_interval().intersects(*seg))
                        insert_segment(seg, (node->get_right()));
                }
            }

            void combine_intervals(FirstLayerNode* node) {
                if (node->get_height() == 1) {
                    node->set_union_interval(node->get_key());
                }

                bool left_null = node->get_left() == nullptr;
                bool right_null = node->get_right() == nullptr;

                if (!left_null)
                    combine_intervals(node->get_left());
                if (!right_null)
                    combine_intervals((node->get_right()));


                if (!(left_null || right_null)) {
                    auto& left_int = (node->get_left())->get_union_interval();
                    auto& right_int = (node->get_right())->get_union_interval();
                    node->set_union_interval(
                        left_int + right_int
                    );
                } else if (!left_null)
                    node->set_union_interval(
                        (node->get_left())->get_union_interval()
                    );
                else if (!right_null)
                    node->set_union_interval(
                        (node->get_right())->get_union_interval()
                    );
                else
                    return; 
            }

        public:
            SegmentDS(std::vector<Segment*> segments) {
                auto x_coords = std::vector<double>();

                for (auto& seg: segments) {
                    x_coords.push_back(seg->x1());
                    x_coords.push_back(seg->x2());
                }
                std::sort(x_coords.begin(), x_coords.end());

                auto intervals = std::vector<ElmInt>();

                for (int i = 0; i < x_coords.size()-1; ++i) {
                    if (i == 0) {
                        intervals.push_back(ElmInt(std::numeric_limits<double>::lowest(), x_coords[i], OO));
                        intervals.push_back(ElmInt(x_coords[i], x_coords[i], CC));
                    }
                    if (i + 1 != x_coords.size()) {
                        intervals.push_back(ElmInt(x_coords[i], x_coords[i+1], OO));
                        intervals.push_back(ElmInt(x_coords[i+1], x_coords[i+1], CC));
                    }
                    if (i == x_coords.size()-2) {
                        intervals.push_back(ElmInt(x_coords[i+1], std::numeric_limits<double>::max(), OO));
                    }
                }

                first_layer_structure = LeafDataAVL<FirstLayerNode>();

                for (auto interval: intervals) {
                    auto node = new FirstLayerNode(interval);
                    first_layer_structure.insert(node);
                }

                combine_intervals((first_layer_structure.get_root()));

                auto root = (first_layer_structure.get_root());

                for (int i = 0; i < segments.size(); ++i) {
                    insert_segment(segments[i], root);
                }
            }

            void vertical_query(Segment query_seg, std::vector<Segment>& output) {

                node_visits = 0;

                if (query_seg.y1() > query_seg.y2())
                    query_seg.swap();
                

                if (first_layer_structure.get_root() != nullptr)
                    first_layer_structure.get_root()
                    ->vertical_query(query_seg, output);
            }

            void count_segs() {
                if (first_layer_structure.get_root() != nullptr)
                    first_layer_structure.get_root()->count_segs();
            }

    };

}