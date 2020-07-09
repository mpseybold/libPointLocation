#include "gtest/gtest.h"
#include "../Cut.h"

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
    PointCart left_of_intersection = PointCart(0.499999999, 0.5);

    ASSERT_EQ(source_cut.orientation(on_source_line), 1);
    ASSERT_EQ(target_cut.orientation(on_target_line), -1);
    ASSERT_EQ(edge_cut.orientation(above_edge), 1);
    ASSERT_EQ(intersection_cut.orientation(left_of_intersection), 1);
}