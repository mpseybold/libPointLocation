#include "gtest/gtest.h"
#include "../Cut.h"

TEST(CutTests, definingPointCutComparisonTest) {
    Segment<PointCart, int> seg_1 = Segment<PointCart, int>(
        PointCart(0, 0), PointCart(1, 1), 1
    );

    Segment<PointCart, int> seg_2 = Segment<PointCart, int>(
        PointCart(0, 1), PointCart(1, 0), 2
    );

    Segment<PointCart, int> seg_3 = Segment<PointCart, int>(
        PointCart(0, 0.5), PointCart(1, 0.5), 3
    );

    Cut<PointCart, int> other_intersection = Cut<PointCart, int>(
        INTERSECTION, &seg_1, &seg_2
    );

    Cut<PointCart, int> source = Cut<PointCart, int>(
        SOURCE, &seg_3, nullptr
    );

    Cut<PointCart, int> target = Cut<PointCart, int>(
        TARGET, &seg_3, nullptr
    );

    ASSERT_EQ(source.defining_point_cut_comparison(other_intersection), -1);
    ASSERT_EQ(target.defining_point_cut_comparison(other_intersection), 1);
}

TEST(CutTests, equalityOperatorTest) {
    Segment<PointCart, int> seg_1 = Segment<PointCart, int>(
        PointCart(1, 1), PointCart(2, 1), 1
    );
    Segment<PointCart, int> seg_2 = Segment<PointCart, int>(
        PointCart(1, 1), PointCart(2, 1), 2
    );
    Segment<PointCart, int> seg_3 = Segment<PointCart, int>(
        PointCart(1, 0), PointCart(2, 2), 3
    );

    Cut<PointCart, int> cut_1 = Cut<PointCart, int>(
        SOURCE, &seg_1, nullptr 
    );
    Cut<PointCart, int> cut_2 = Cut<PointCart, int>(
        SOURCE, &seg_2, nullptr
    );
    Cut<PointCart, int> cut_3 = Cut<PointCart, int>(
        INTERSECTION, &seg_1, &seg_3
    );
    Cut<PointCart, int> cut_4 = Cut<PointCart, int>(
        INTERSECTION, &seg_2, &seg_3
    );

    ASSERT_FALSE(cut_1 == cut_3);
    ASSERT_TRUE(cut_3 == cut_4);
    ASSERT_TRUE(cut_1 == cut_2);
}

TEST(CutTests, orientationTest) {
    PointCart a = PointCart(0, 0);
    PointCart b = PointCart(1, 1);
    PointCart c = PointCart(0, 1);
    PointCart d = PointCart(1, 0);

    Segment<PointCart, int> seg_1 = Segment<PointCart, int>(a, b, 1);
    Segment<PointCart, int> seg_2 = Segment<PointCart, int>(c, d, 2);

    Cut<PointCart, int> source_cut = Cut<PointCart, int>(SOURCE, &seg_1, nullptr);
    Cut<PointCart, int> target_cut = Cut<PointCart, int>(TARGET, &seg_1, nullptr);
    Cut<PointCart, int> edge_cut = Cut<PointCart, int>(EDGE, &seg_1, nullptr);
    Cut<PointCart, int> intersection_cut = Cut<PointCart, int>(INTERSECTION, &seg_1, &seg_2);


    PointCart on_source_line = PointCart(0, -0.0000001);
    PointCart on_target_line = PointCart(1, 1.00001);
    PointCart above_edge = PointCart(0.5, 0.500000001);
    PointCart on_edge = PointCart(0.51234567, 0.51234567);
    PointCart left_of_intersection = PointCart(0.499999999, 0.5);

    ASSERT_EQ(source_cut.orientation(on_source_line), 1);
    ASSERT_EQ(target_cut.orientation(on_target_line), -1);
    ASSERT_EQ(edge_cut.orientation(above_edge), 1);
    ASSERT_EQ(edge_cut.orientation(on_edge), 0);
    ASSERT_EQ(intersection_cut.orientation(left_of_intersection), -1);
}