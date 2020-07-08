#include "gtest/gtest.h"
#include "../geom.h"

TEST(GeomTests, vOrientation) {
    PointCart p = PointCart(1324.553267241, 0.00432543);
    PointCart directly_above = PointCart(1324.553267241, 0.00432544);
    PointCart directly_below = PointCart(1324.553267241, 0.00432542);

    ASSERT_EQ(PointCart::v_orientation(p, directly_above), -1);
    ASSERT_EQ(PointCart::v_orientation(p, directly_below), 1);
    ASSERT_EQ(PointCart::v_orientation(p, p), 0);
}

TEST(GeomTests, lineOrientation) {
    PointCart s = PointCart(0, 0);
    PointCart t = PointCart(1, 1);

    PointCart just_below = PointCart(100, 99.999999);
    PointCart just_above = PointCart(100, 100.000001);
    PointCart on_line = PointCart(12345.1234512345, 12345.1234512345);

    ASSERT_EQ(PointCart::line_orientation(s, t, just_below), -1);
    ASSERT_EQ(PointCart::line_orientation(s, t, just_above), 1);
    ASSERT_EQ(PointCart::line_orientation(s, t, on_line), 0);
}

TEST(GeomTests, intersectionVOrientation) {
    PointCart s_1 = PointCart(0, 0);
    PointCart t_1 = PointCart(1, 1);

    PointCart s_2 = PointCart(1, 1);
    PointCart t_2 = PointCart(2, 0);

    PointCart just_right = PointCart(1.00000001, 9);
    PointCart just_left = PointCart(0.999999999, 7);
    PointCart just_above = PointCart(1, 1.0000001);
    PointCart just_below = PointCart(1, 0.0000009);
    PointCart p = PointCart(1, 1);
    

    ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, just_right), -1);
    ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, just_left), 1);
    ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, just_above), -1);
    ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, just_below), 1);
    ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, p), 0);
}

TEST(GeomTests, intersectionOrientation) {
    PointCart s_1 = PointCart(0, 1);
    PointCart t_1 = PointCart(1, 1);
    PointCart s_2 = PointCart(0, 0);
    PointCart t_2 = PointCart(1, 0);

    PointCart top_is_left_s = PointCart(0.49999999999, 0.50000000001);
    PointCart top_is_left_t = PointCart(0.50000000001, 0.49999999999);

    PointCart top_is_right_s = PointCart(0.499999999, 0.4999999999);
    PointCart top_is_right_t = PointCart(0.500000001, 0.5000000001);

    PointCart vertical_s = PointCart(0.5, 0.5);
    PointCart vertical_t = PointCart(0.5, 0.50000000001);

    ASSERT_EQ(
        PointCart::intersection_orientation(
            top_is_left_s, top_is_left_t,
            s_1, t_1, s_2, t_2
        ), -1
    );

    ASSERT_EQ(
        PointCart::intersection_orientation(
            top_is_right_s, top_is_right_t,
            s_1, t_1, s_2, t_2
        ), 1
    );

    ASSERT_EQ(
        PointCart::intersection_orientation(
            vertical_s, vertical_t,
            s_1, t_1, s_2, t_2
        ), 1
    );
}