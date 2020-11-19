#pragma once

#include "Cut.h"

template <class PointType, class OrderType>
class V_Cut {

    private:

        Cut<PointType, OrderType>* up;
        Cut<PointType, OrderType>* down;
        std::vector<Segment<PointType, OrderType>*> segs;

    public:

        V_Cut(CutType type, Segment<PointType, OrderType>* seg, Segment<PointType, OrderType>* int_seg) {
            assert(type != EDGE);
            assert(seg != nullptr);
            auto new_cut = new Cut<PointType, OrderType>(type, seg, int_seg);
            up = new_cut;
            down = new_cut;
            segs.push_back(seg);
            if (int_seg != nullptr)
                segs.push_back(int_seg);

            if (up->orientation(PointCart(60, 50)) == 0)
                std::cout << "hello\n";
        }

        int defining_point_cut_comparison(V_Cut<PointType, OrderType>& cut) {
            if (up != nullptr)
                return up->defining_point_cut_comparison(*cut.get_cut(1));
            else if (down != nullptr)
                return down->defining_point_cut_comparison(*cut.get_cut(1));

            assert(false);
        }

        bool is_empty() {
            return segs.size() == 0;
        }

     
        void set_up(Cut<PointType, OrderType>* _up) { up = _up; }
        void set_down(Cut<PointType, OrderType>* _down) { down = _down; }

        bool all_overlap() {
            bool result = true;

            result = result && !up->orientation(segs[0]->get_source()) == 0
                && !up->orientation(segs[0]->get_target()) == 0;

            for (int i = 1; i < segs.size(); ++i) {
                result = result && Segment<PointType, OrderType>::slope_comparison(*segs[i], *segs[i-1]) == 0
                && !up->orientation(segs[i]->get_source()) == 0
                && !up->orientation(segs[i]->get_target()) == 0;
            }

            return result;
        }

        void update() {

            if (segs.size() < 2) 
                return;
            
            // search for lowest priority segment

            Segment<PointType, OrderType>* min_seg = nullptr;

            auto cut = up != nullptr ? up : down;

            for (auto s: segs) {
                if ((cut->has_seg_on_pos_side(s) && cut->has_seg_on_neg_side(s)) &&
                    (min_seg == nullptr || s->get_priority() < min_seg->get_priority()))
                    min_seg = s;
            }


            // search for the lowest priority intersection cut

            Segment<PointType, OrderType>* opt_seg = nullptr;
            Segment<PointType, OrderType>* opt_int_seg = nullptr;

            if (segs.size() > 1) {
                for (int i = 0; i < segs.size()-1; ++i) {
                    for (int j = i+1; j < segs.size(); ++j) {
                        auto s_1 = segs[i];
                        auto s_2 = segs[j];

                        if (Segment<PointType, OrderType>::slope_comparison(*s_1, *s_2) == 0)
                            continue;

                        auto e_1 = Cut<PointType, OrderType>(
                            EDGE, s_1, nullptr
                        );
                        auto e_2 = Cut<PointType, OrderType>(
                            EDGE, s_2, nullptr
                        );

                        if (!(e_1.has_seg_on_pos_side(s_2) && e_1.has_seg_on_neg_side(s_2)
                        && e_2.has_seg_on_pos_side(s_1) && e_2.has_seg_on_neg_side(s_1)))
                            continue;

                        auto aux = Cut<PointType, OrderType>(
                            INTERSECTION, s_1, s_2
                        );

                        if (up->defining_point_cut_comparison(aux) != 0)
                            continue;

                        if (opt_seg == nullptr || 
                        std::max(s_1->get_priority(), s_2->get_priority()) 
                        < std::max(opt_seg->get_priority(), opt_int_seg->get_priority())) {
                            opt_seg = s_1;
                            opt_int_seg = s_2;
                        }
                    }
                }
            }

            auto temp_int = opt_seg != nullptr ? Cut<PointType, OrderType>(
                    INTERSECTION, opt_seg, opt_int_seg
                ) : Cut<PointType, OrderType>();

            // search for the lowest priority source cut
    
            Segment<PointType, OrderType>* opt_source = nullptr;

            for (int i = 0; i < segs.size() - 1; ++i) {
                auto s = segs[i];

                if (up->orientation(s->get_source()) == 0) {
                    if (opt_source == nullptr || s->get_priority() < opt_source->get_priority())
                        opt_source = s;
                }
            }

            auto temp_source = opt_source != nullptr ? Cut<PointType, OrderType>(SOURCE, opt_source, nullptr) 
            : Cut<PointType, OrderType>();

            // search for the lowest priority target cut

            Segment<PointType, OrderType>* opt_target = nullptr;

            for (int i = 0; i < segs.size() - 1; ++i) {
                auto s = segs[i];

                if (up->orientation(s->get_target()) == 0) {
                    if (opt_target == nullptr || s->get_priority() < opt_target->get_priority())
                        opt_target = s;
                }
            }

            auto temp_target = opt_target != nullptr ? Cut<PointType, OrderType>(SOURCE, opt_target, nullptr) 
            : Cut<PointType, OrderType>();

            OrderType int_priority = opt_seg == nullptr ? 100000000000000 :
            std::max(opt_seg->get_priority(), opt_int_seg->get_priority());

            OrderType source_priority = opt_source == nullptr ? 100000000000000 :
            opt_source->get_priority();

            OrderType target_priority = opt_target == nullptr ? 100000000000000 :
            opt_target->get_priority();


            if (int_priority < std::min(source_priority, target_priority)) {
                up->set_type(INTERSECTION);
                up->set_seg(opt_seg);
                up->set_intersecting_seg(opt_int_seg);

                down->set_type(INTERSECTION);
                down->set_seg(opt_seg);
                down->set_intersecting_seg(opt_int_seg);
            } else {
                if (min_seg == nullptr) {
                    if (source_priority < std::min(int_priority, target_priority)) {
                        up->set_type(SOURCE);
                        up->set_seg(opt_source);
                        up->set_intersecting_seg(nullptr);
                        down->set_type(SOURCE);
                        down->set_seg(opt_source);
                        down->set_intersecting_seg(nullptr);
                    } else if (target_priority < std::min(int_priority, source_priority)) {
                        up->set_type(TARGET);
                        up->set_seg(opt_target);
                        up->set_intersecting_seg(nullptr);
                        down->set_type(TARGET);
                        down->set_seg(opt_target);
                        down->set_intersecting_seg(nullptr);
                    }
                } else {
                    Segment<PointType, OrderType>* min_above = nullptr;
                    Segment<PointType, OrderType>* min_below = nullptr;

                    CutType above_type = SOURCE;
                    CutType below_type = TARGET;

                    auto edge_cut = Cut<PointType, OrderType>(EDGE, min_seg, nullptr);

                    for (auto s: segs) {
                        if (cut->orientation(s->get_source()) == 0) {
                            if (edge_cut.orientation(s, 0) == 1) {
                                if (min_above == nullptr || s->get_priority() < min_above->get_priority()) {
                                    min_above = s;
                                    above_type = SOURCE;
                                }
                            } else if (edge_cut.orientation(s, 0) == -1) {
                                if (min_below == nullptr || s->get_priority() < min_below->get_priority()) {
                                    min_below = s;
                                    below_type = SOURCE;
                                }
                            } else {
                                assert(false);
                            }
                        } else if (cut->orientation(s->get_target()) == 0) {
                            if (edge_cut.orientation(s, 1) == 1) {
                                if (min_above == nullptr || s->get_priority() < min_above->get_priority()) {
                                    min_above = s;
                                    above_type = TARGET;
                                }
                            } else if (edge_cut.orientation(s, 1) == -1) {
                                if (min_below == nullptr || s->get_priority() < min_below->get_priority()) {
                                    min_below = s;
                                    below_type = TARGET;
                                }
                            } else {
                                assert(false);
                            }
                        }
                    }

                    if (min_above != nullptr) {
                        up->set_type(above_type);
                        up->set_seg(min_above);
                        up->set_intersecting_seg(nullptr);
                    }

                    if (min_below != nullptr) {
                        down->set_type(below_type);
                        down->set_seg(min_below);
                        down->set_intersecting_seg(nullptr);
                    }

                    if (opt_seg != nullptr) {
                        if (min_above == nullptr) {
                            up->set_type(INTERSECTION);
                            up->set_seg(opt_seg);
                            up->set_intersecting_seg(opt_int_seg);
                        }

                        if (min_below == nullptr) {
                            down->set_type(INTERSECTION);
                            down->set_seg(opt_seg);
                            down->set_intersecting_seg(opt_int_seg);
                        }
                    }
                    
                }
            }


        }

        void insert_intersection(CutType type, 
            Segment<PointType, OrderType>* seg, 
            Segment<PointType, OrderType>* int_seg) {
                // std::cout << "v_cut size: " << segs.size() << std::endl;
                assert(seg != nullptr);
                if (segs.empty() || std::find(segs.begin(), segs.end(), seg) == segs.end()) {
                    segs.push_back(seg);
                }
                if (int_seg != nullptr) {
                    if (std::find(segs.begin(), segs.end(), int_seg) == segs.end()) {
                        segs.push_back(int_seg);
                    }
                }

                if (up->orientation(PointCart(60, 50)) == 0)
                    std::cout << "hello\n";

                update();
        }

        void delete_segment(Segment<PointType, OrderType>* seg) {
            for (int i = 0; i < segs.size(); ++i) {
                if (seg == segs[i])
                    segs.erase(segs.begin() + i);
            }

            if (all_overlap()) {
                segs = std::vector<Segment<PointType, OrderType>*>();
            }

            update();

            if (up->orientation(PointCart(60, 50)) == 0)
                std::cout << "hello\n";
        }

        bool contains(Segment<PointType, OrderType>* seg) {
            for (auto s: segs) {
                if (s == seg)
                    return true;
            }

            return false;
        }

        Cut<PointType, OrderType>* get_cut(int side) {
            if (side == 1)
                return up;
            if (side == -1)
                return down;

            return up;
        }

        OrderType get_priority() {
            return std::min(up->get_priority(), down->get_priority());
        }
};