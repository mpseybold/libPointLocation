#include "gtest/gtest.h"
#include "../BoundingTrap.h"



TEST(BoundingTrapTests, destructionTest) {

/*
             |      |
        2    |      |
             |      |
    .___1____|______|____.
             |      |
   -1   0.___.1    2.___.3    4   
             |      |
._______-1___|______|__________.
             |      |
        -2   |      |

*/    
    Segment<PointCart, int> left = Segment<PointCart, int>(
        PointCart(0, 0), PointCart(1, 0), 1
    );

    Segment<PointCart, int> right = Segment<PointCart, int>(
        PointCart(2, 0), PointCart(3, 0), 1
    );

    Segment<PointCart, int> top = Segment<PointCart, int>(
        PointCart(-1, 1), PointCart(3, 1), 1
    );

    Segment<PointCart, int> bottom = Segment<PointCart, int>(
        PointCart(-2, -1), PointCart(4, -1), 1
    );

    Segment<PointCart, int> bottom_2 = Segment<PointCart, int>(
        PointCart(-2, -1), PointCart(3, 1), 1
    );

    Segment<PointCart, int> bottom_3 = Segment<PointCart, int>(
        PointCart(-1, 1), PointCart(4, -1), 1
    );

    Segment<PointCart, int> destroying_seg = Segment<PointCart, int>(
        PointCart(1.5, -0.5), PointCart(10, -0.5), 1
    );

    Cut<PointCart, int> no_cut = Cut<PointCart, int>();
    Cut<PointCart, int> left_cut = Cut<PointCart, int>(TARGET, &left, nullptr);
    Cut<PointCart, int> right_cut = Cut<PointCart, int>(SOURCE, &right, nullptr);
    Cut<PointCart, int> top_cut = Cut<PointCart, int>(EDGE, &top, nullptr);
    Cut<PointCart, int> bottom_cut = Cut<PointCart, int>(EDGE, &bottom, nullptr);
    Cut<PointCart, int> bottom_cut_2 = Cut<PointCart, int>(EDGE, &bottom_2, nullptr);
    Cut<PointCart, int> bottom_cut_3 = Cut<PointCart, int>(EDGE, &bottom_3, nullptr);
    Cut<PointCart, int> v_destroying_cut = Cut<PointCart, int>(SOURCE, &destroying_seg, nullptr);
    Cut<PointCart, int> e_destroying_cut = Cut<PointCart, int>(EDGE, &destroying_seg, nullptr);
    BoundingTrap<PointCart, int> trap_BRTL = BoundingTrap<PointCart, int>(
        bottom_cut, right_cut, top_cut, left_cut
    );

    BoundingTrap<PointCart, int> trap_BTL = BoundingTrap<PointCart, int>(
        bottom_cut_2, Cut<PointCart, int>(), top_cut, left_cut
    );

    BoundingTrap<PointCart, int> trap_BRT = BoundingTrap<PointCart, int>(
        bottom_cut_3, right_cut, top_cut, Cut<PointCart, int>()
    );


    // destroy the BRTL trapezoid by the vertical cut
    std::pair<BoundingTrap<PointCart, int>, BoundingTrap<PointCart, int>> pos_neg_BRTL_vertical = 
    trap_BRTL.destroy(v_destroying_cut);

    ASSERT_TRUE(pos_neg_BRTL_vertical.second.get_right() == v_destroying_cut);
    ASSERT_TRUE(pos_neg_BRTL_vertical.first.get_left() == v_destroying_cut);
    ASSERT_TRUE(pos_neg_BRTL_vertical.second.get_top() == top_cut);
    ASSERT_TRUE(pos_neg_BRTL_vertical.first.get_top() == top_cut);

    // destroythe BRTL trapezoid by the edge cut
    std::pair<BoundingTrap<PointCart, int>, BoundingTrap<PointCart, int>> pos_neg_BRTL_edge = 
    trap_BRTL.destroy(e_destroying_cut);

    ASSERT_TRUE(pos_neg_BRTL_edge.first.get_bottom() == e_destroying_cut);
    ASSERT_TRUE(pos_neg_BRTL_edge.first.get_right() == right_cut);
    ASSERT_TRUE(pos_neg_BRTL_edge.first.get_left() == left_cut);
    ASSERT_TRUE(pos_neg_BRTL_edge.first.get_top() == top_cut);
    ASSERT_TRUE(pos_neg_BRTL_edge.second.get_top() == e_destroying_cut);
    ASSERT_TRUE(pos_neg_BRTL_edge.second.get_right() == right_cut);
    ASSERT_TRUE(pos_neg_BRTL_edge.second.get_bottom() == bottom_cut);
    ASSERT_TRUE(pos_neg_BRTL_edge.second.get_left() == left_cut);

    // destroy the BRT trapezoid by the vertical cut
    std::pair<BoundingTrap<PointCart, int>, BoundingTrap<PointCart, int>> pos_neg_BRT_vertical = 
    trap_BRT.destroy(v_destroying_cut);

    ASSERT_TRUE(pos_neg_BRT_vertical.second.get_right() == v_destroying_cut);
    ASSERT_TRUE(pos_neg_BRT_vertical.second.get_top() == top_cut);
    ASSERT_TRUE(pos_neg_BRT_vertical.second.get_left() == no_cut);
    ASSERT_TRUE(pos_neg_BRT_vertical.second.get_bottom() == bottom_cut_3);
    ASSERT_TRUE(pos_neg_BRT_vertical.first.get_left() == v_destroying_cut);
    ASSERT_TRUE(pos_neg_BRT_vertical.first.get_top() == top_cut);

    // destroy the BTL trapezoid by the vertical cut
    std::pair<BoundingTrap<PointCart, int>, BoundingTrap<PointCart, int>> pos_neg_BTL_vertical = 
    trap_BTL.destroy(v_destroying_cut);

    ASSERT_TRUE(pos_neg_BTL_vertical.second.get_right() == v_destroying_cut);
    ASSERT_TRUE(pos_neg_BTL_vertical.second.get_top() == top_cut);
    ASSERT_TRUE(pos_neg_BTL_vertical.second.get_left() == left_cut);
    ASSERT_TRUE(pos_neg_BTL_vertical.second.get_bottom() == bottom_cut_2);
    ASSERT_TRUE(pos_neg_BTL_vertical.first.get_left() == v_destroying_cut);
    ASSERT_TRUE(pos_neg_BTL_vertical.first.get_right() == no_cut);
    ASSERT_TRUE(pos_neg_BTL_vertical.first.get_bottom() == bottom_cut_2);
    ASSERT_TRUE(pos_neg_BTL_vertical.first.get_top() == top_cut);

    // destroy the BRT trapezoid by the edge cut
    std::pair<BoundingTrap<PointCart, int>, BoundingTrap<PointCart, int>> pos_neg_BRT_edge = 
    trap_BRT.destroy(e_destroying_cut);

    ASSERT_TRUE(pos_neg_BRT_edge.first.get_right() == right_cut);
    ASSERT_TRUE(pos_neg_BRT_edge.first.get_top() == top_cut);
    ASSERT_TRUE(pos_neg_BRT_edge.first.get_left() == no_cut);
    ASSERT_TRUE(pos_neg_BRT_edge.first.get_bottom() == e_destroying_cut);
    ASSERT_TRUE(pos_neg_BRT_edge.second.get_left() == no_cut);
    ASSERT_TRUE(pos_neg_BRT_edge.second.get_top() == e_destroying_cut);
    ASSERT_TRUE(pos_neg_BRT_edge.second.get_bottom() == bottom_cut_3);

    // destroy the BTL trapezoid by the edge cut
    std::pair<BoundingTrap<PointCart, int>, BoundingTrap<PointCart, int>> pos_neg_BTL_edge = 
    trap_BTL.destroy(e_destroying_cut);

    ASSERT_TRUE(pos_neg_BTL_edge.first.get_right() == no_cut);
    ASSERT_TRUE(pos_neg_BTL_edge.first.get_top() == top_cut);
    ASSERT_TRUE(pos_neg_BTL_edge.first.get_left() == left_cut);
    ASSERT_TRUE(pos_neg_BTL_edge.first.get_bottom() == e_destroying_cut);
    ASSERT_TRUE(pos_neg_BTL_edge.second.get_left() == left_cut);
    ASSERT_TRUE(pos_neg_BTL_edge.second.get_top() == e_destroying_cut);
    ASSERT_TRUE(pos_neg_BTL_edge.second.get_bottom() == bottom_cut_2);
}

TEST(BoundingTrapTests, mergeTests) {

    Segment<PointCart, int> top_seg = Segment<PointCart, int>(
        PointCart(-1, 1), PointCart(100, 1), 1
    );
    Segment<PointCart, int> bottom_seg = Segment<PointCart, int>(
        PointCart(-1, 0), PointCart(101, 0), 1
    );
    Segment<PointCart, int> middle_seg = Segment<PointCart, int>(
        PointCart(1, 0.5), PointCart(2, 0.5), 1
    );
    Segment<PointCart, int> left_seg = Segment<PointCart, int>(
        PointCart(-5, 0.5), PointCart(0, 0.5), 1
    );
    Segment<PointCart, int> diagonal_seg = Segment<PointCart, int>(
        PointCart(0, 1), PointCart(1, 0), 1
    );

    Cut<PointCart, int> top_cut = Cut<PointCart, int>(
        EDGE, &top_seg, nullptr
    );
    Cut<PointCart, int> bottom_cut = Cut<PointCart, int> (
        EDGE, &bottom_seg, nullptr
    );
    Cut<PointCart, int> v_neg_left_cut = Cut<PointCart, int>(
        TARGET, &left_seg, nullptr
    );
    Cut<PointCart, int> v_neg_right_cut = Cut<PointCart, int>(
        SOURCE, &middle_seg, nullptr
    );

    Cut<PointCart, int> v_pos_left_cut = Cut<PointCart, int>(
        SOURCE, &middle_seg, nullptr
    );
    Cut<PointCart, int> v_pos_right_cut = Cut<PointCart, int>(
        TARGET, &middle_seg, nullptr
    );

    Cut<PointCart, int> diagonal_cut = Cut<PointCart, int>(
        EDGE, &diagonal_seg, nullptr
    );

    BoundingTrap<PointCart, int> v_neg_trap = BoundingTrap<PointCart, int>(
        bottom_cut, v_neg_right_cut, top_cut, v_neg_left_cut
    );
    BoundingTrap<PointCart, int> v_pos_trap = BoundingTrap<PointCart, int>(
        bottom_cut, v_pos_right_cut, top_cut, v_pos_left_cut
    );

    BoundingTrap<PointCart, int> v_merge_result = BoundingTrap<PointCart, int>::merge(
        v_neg_trap, v_pos_trap
    );

    ASSERT_TRUE(v_merge_result.get_top() == top_cut);
    ASSERT_TRUE(v_merge_result.get_bottom() == bottom_cut);
    ASSERT_TRUE(v_merge_result.get_left() == v_neg_left_cut);
    ASSERT_TRUE(v_merge_result.get_right() == v_pos_right_cut);

    BoundingTrap<PointCart, int> e_pos_trap = BoundingTrap<PointCart, int>(
        diagonal_cut, v_neg_right_cut, top_cut, Cut<PointCart, int>()
    );
    BoundingTrap<PointCart, int> e_neg_trap = BoundingTrap<PointCart, int>(
        bottom_cut, Cut<PointCart, int>(), diagonal_cut, v_neg_left_cut
    );

    BoundingTrap<PointCart, int> e_merge_result = BoundingTrap<PointCart, int>::merge(
        e_pos_trap, e_neg_trap
    );

    ASSERT_TRUE(e_merge_result.get_left() == v_neg_left_cut);
    ASSERT_TRUE(e_merge_result.get_right() == v_neg_right_cut);
    ASSERT_TRUE(e_merge_result.get_bottom() == bottom_cut);
    ASSERT_TRUE(e_merge_result.get_top() == top_cut);
}

TEST(BoundingTrapTests, segmentIntersectionTests) {
    Segment<PointCart, int> left_seg = Segment<PointCart, int>(
        PointCart(-1, 0.5), PointCart(0, 0.5), 1
    );
    Segment<PointCart, int> right_seg = Segment<PointCart, int>(
        PointCart(1, 0.5), PointCart(2, 0.5), 1
    );
    Segment<PointCart, int> top_seg = Segment<PointCart, int>(
        PointCart(-2, 1), PointCart(6, 1), 1
    );
    Segment<PointCart, int> bottom_seg = Segment<PointCart, int>(
        PointCart(-3, 0), PointCart(7, 0), 1
    );

    Cut<PointCart, int> left_cut = Cut<PointCart, int>(
        TARGET, &left_seg, nullptr
    );
    Cut<PointCart, int> right_cut = Cut<PointCart, int>(
        SOURCE, &right_seg, nullptr
    );
    Cut<PointCart, int> top_cut = Cut<PointCart, int>(
        EDGE, &top_seg, nullptr
    );
    Cut<PointCart, int> bottom_cut = Cut<PointCart, int>(
        EDGE, &bottom_seg, nullptr
    );

    BoundingTrap<PointCart, int> trap = BoundingTrap<PointCart, int>(
        bottom_cut, right_cut, top_cut, left_cut
    );

    Segment<PointCart, int> non_intersecting_1 = Segment<PointCart, int>(
        PointCart(-5, 10), PointCart(-5, 5), 1
    );
    Segment<PointCart, int> intersecting_1 = Segment<PointCart, int>(
        PointCart(0.5, 10), PointCart(0.5, -10), 1
    );

    Segment<PointCart, int> non_intersecting_2 = Segment<PointCart, int>(
        PointCart(0.1, 1.0001), PointCart(0.9, 1.00001), 1
    );
    Segment<PointCart, int> intersecting_2 = Segment<PointCart, int>(
        PointCart(0.1, 0.5), PointCart(1.5, 0.5), 1
    );

    Segment<PointCart, int> non_intersecting_3 = Segment<PointCart, int>(
        PointCart(0.5, 1), PointCart(1, 2), 1
    );

    Segment<PointCart, int> intersecting_3 = Segment<PointCart, int>(
        PointCart(0, 1), PointCart(0, 0), 1
    );

    Segment<PointCart, int> intersecting_4 = Segment<PointCart, int>(
        PointCart(0, 0.4), PointCart(0, 0), 1
    );

    Segment<PointCart, int> intersecting_5 = Segment<PointCart, int>(
        PointCart(1, 1), PointCart(1, 0.5), 1
    );

    Segment<PointCart, int> intersecting_6 = Segment<PointCart,int>(
        PointCart(0, 0.4), PointCart(0, 0.2), 1
    );

    Segment<PointCart, int> non_intersecting_4 = Segment<PointCart, int>(
        PointCart(0.5, 0), PointCart(1, -1), 1
    );

    Segment<PointCart, int> non_intersecting_5 = Segment<PointCart, int>(
        PointCart(0, 1), PointCart(0, 0.8), 1
    );

    Segment<PointCart, int> non_intersecting_6 = Segment<PointCart, int>(
        PointCart(1, 0.5), PointCart(1, 0), 1
    );

    Segment<PointCart, int> not_sure = Segment<PointCart, int>(
        PointCart(0.5, 1), PointCart(1.5, 1), 1
    );

    ASSERT_FALSE(trap.intersects_segment(&non_intersecting_1));
    ASSERT_FALSE(trap.intersects_segment(&non_intersecting_2));
    ASSERT_FALSE(trap.intersects_segment(&non_intersecting_3));
    ASSERT_FALSE(trap.intersects_segment(&non_intersecting_4));
    ASSERT_FALSE(trap.intersects_segment(&non_intersecting_5));
    ASSERT_FALSE(trap.intersects_segment(&non_intersecting_6));

    ASSERT_TRUE(trap.intersects_segment(&intersecting_1));
    ASSERT_TRUE(trap.intersects_segment(&intersecting_2));
    ASSERT_TRUE(trap.intersects_segment(&intersecting_3));
    ASSERT_TRUE(trap.intersects_segment(&intersecting_4));
    ASSERT_TRUE(trap.intersects_segment(&intersecting_5));
    ASSERT_TRUE(trap.intersects_segment(&intersecting_6));

    Segment<PointCart, int> top_seg_degen = Segment<PointCart, int>(
        PointCart(0, 2), PointCart(0, -1), 1
    );

    Cut<PointCart, int> top_cut_degen = Cut<PointCart, int>(
        EDGE, &top_seg_degen, nullptr
    );

    BoundingTrap<PointCart, int> degen_trap = BoundingTrap<PointCart, int>(
        bottom_cut, Cut<PointCart, int>(), 
        top_cut_degen, left_cut
    );

    Segment<PointCart, int> intersecting_degen_1 = Segment<PointCart, int>(
        PointCart(-3, 0.4), PointCart(3, 0.4), 1
    );

    Segment<PointCart, int> intersecting_degen_2 = Segment<PointCart, int>(
        PointCart(0, 0.4), PointCart(0, 0.2), 1
    );

    Segment<PointCart, int> intersecting_degen_3 = Segment<PointCart, int>(
        PointCart(-2, 0.4), PointCart(0, 0.4), 1
    );
    Segment<PointCart, int> intersecting_degen_4 = Segment<PointCart, int>(
        PointCart(0, 4), PointCart(0, -4), 1
    );

    Segment<PointCart, int> non_intersecting_degen_1 = Segment<PointCart, int>(
        PointCart(-3, 0.6), PointCart(3, 0.6), 1
    );

    Segment<PointCart, int> non_intersecting_degen_2 = Segment<PointCart, int>(
        PointCart(-3, 0.5), PointCart(3, 0.5), 1
    );

    Segment<PointCart, int> non_intersecting_degen_3 = Segment<PointCart, int>(
        PointCart(0, 0.8), PointCart(0, 0.6), 1
    );

    Segment<PointCart, int> non_intersecting_degen_4 = Segment<PointCart, int>(
        PointCart(-2, 0.5), PointCart(0, 0.5), 1
    );


    ASSERT_FALSE(degen_trap.intersects_segment(&non_intersecting_degen_1));
    // ASSERT_FALSE(degen_trap.intersects_segment(&non_intersecting_degen_2));
    ASSERT_FALSE(degen_trap.intersects_segment(&non_intersecting_degen_3));
    ASSERT_FALSE(degen_trap.intersects_segment(&non_intersecting_degen_4));
    ASSERT_TRUE(degen_trap.intersects_segment(&intersecting_degen_1));
    // ASSERT_TRUE(degen_trap.intersects_segment(&intersecting_degen_2));
    ASSERT_TRUE(degen_trap.intersects_segment(&intersecting_degen_3));
    // ASSERT_TRUE(degen_trap.intersects_segment(&intersecting_degen_4));
}