#include "gtest/gtest.h"
#include "../Segment.hpp"
#include "../Point_S2ratss.hpp"

using SegS2 = Segment<PointS2ratss,int>;
using PoiS2 = PointS2ratss;

// TEST(GeomTests, vOrientation) {
//     PointCart p = PointCart(1324.553267241, 0.00432543);
//     PointCart directly_above = PointCart(1324.553267241, 0.00432544);
//     PointCart directly_below = PointCart(1324.553267241, 0.00432542);

//     ASSERT_EQ(PointCart::v_orientation(p, directly_above), -1);
//     ASSERT_EQ(PointCart::v_orientation(p, directly_below), 1);
//     ASSERT_EQ(PointCart::v_orientation(p, p), 0);
// }

// TEST(GeomTests, lineOrientation) {
//     PointCart s = PointCart(0, 0);
//     PointCart t = PointCart(1, 1);

//     PointCart just_below = PointCart(100, 99.999999);
//     PointCart just_above = PointCart(100, 100.000001);
//     PointCart on_line = PointCart(12345.1234512345, 12345.1234512345);

//     ASSERT_EQ(PointCart::line_orientation(s, t, just_below), -1);
//     ASSERT_EQ(PointCart::line_orientation(s, t, just_above), 1);
//     ASSERT_EQ(PointCart::line_orientation(s, t, on_line), 0);
// }

// TEST(GeomTests, intersectionVOrientation) {
//     PointCart s_1 = PointCart(0, 0);
//     PointCart t_1 = PointCart(1, 1);

//     PointCart s_2 = PointCart(1, 1);
//     PointCart t_2 = PointCart(2, 0);

//     PointCart just_right = PointCart(1.00000001, 9);
//     PointCart just_left = PointCart(0.999999999, 7);
//     PointCart just_above = PointCart(1, 1.0000001);
//     PointCart just_below = PointCart(1, 0.0000009);
//     PointCart p = PointCart(1, 1);
    

//     ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, just_right), -1);
//     ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, just_left), 1);
//     ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, just_above), -1);
//     ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, just_below), 1);
//     ASSERT_EQ(PointCart::intersection_v_orientation(s_1, t_1, s_2, t_2, p), 0);
// }

// TEST(GeomTests, intersectionOrientation) {
//     PointCart s_1 = PointCart(0, 1);
//     PointCart t_1 = PointCart(1, 1);
//     PointCart s_2 = PointCart(0, 0);
//     PointCart t_2 = PointCart(1, 0);

//     PointCart top_is_left_s = PointCart(0.49999999999, 0.50000000001);
//     PointCart top_is_left_t = PointCart(0.50000000001, 0.49999999999);

//     PointCart top_is_right_s = PointCart(0.499999999, 0.4999999999);
//     PointCart top_is_right_t = PointCart(0.500000001, 0.5000000001);

//     PointCart vertical_s = PointCart(0.5, 0.5);
//     PointCart vertical_t = PointCart(0.5, 0.50000000001);

//     ASSERT_EQ(
//         PointCart::intersection_orientation(
//             top_is_left_s, top_is_left_t,
//             s_1, t_1, s_2, t_2
//         ), -1
//     );

//     ASSERT_EQ(
//         PointCart::intersection_orientation(
//             top_is_right_s, top_is_right_t,
//             s_1, t_1, s_2, t_2
//         ), 1
//     );

//     ASSERT_EQ(
//         PointCart::intersection_orientation(
//             vertical_s, vertical_t,
//             s_1, t_1, s_2, t_2
//         ), 1
//     );
// }


TEST(GeomTest, poiS2OrientV) {
    PoiS2 p = PoiS2(108.0, 45.0); 
    PoiS2 q = PoiS2(108.0, 45.0); 
    PoiS2 r = PoiS2( 108.00000001,
                     45.000000001 );
    ASSERT_EQ(  PoiS2::orientV(p,q), 0 );
    ASSERT_EQ(  PoiS2::orientV(q,r), +1 );
    ASSERT_EQ(  PoiS2::orientV(r,q), -1 );
}
TEST(GeomTest, poiS2OrientV_degen) {
    double eps = 0.0001;

    PoiS2 p = PoiS2(0.0     , -1.0      , 1.0-   eps),
          q = PoiS2(0.0     , -1.0      , 1.0- 2*eps);

    ASSERT_EQ(  PoiS2::orientV(p,q), +1 );
    ASSERT_EQ(  PoiS2::orientV(q,p), -1 );

    PoiS2 r = PoiS2(0.0     , -1.0      , 0.7       ),
          s = PoiS2(0.0     , -2.0      , 1.4       ),
          t = PoiS2(0.0+ eps, -1.0      , 0.7       );

    ASSERT_EQ(  PoiS2::orientV(r,t), +1 );
    ASSERT_EQ(  PoiS2::orientV(t,r), -1 );
    ASSERT_EQ(  PoiS2::orientV(r,s),  0 );

    // WEST
    PoiS2 u = PoiS2(0.0     , 1.0      , 0.0+eps       ),
          v = PoiS2(0.0     , 1.0      , 0.0-eps       );
    ASSERT_EQ( PoiS2::orientV(u,v), +1);
    ASSERT_EQ( PoiS2::orientV(v,u), -1);

    // EAST
    PoiS2 uu = PoiS2(0.0     , -1.0      , 0.0+eps       ),
          vv = PoiS2(0.0     , -1.0      , 0.0-eps       );
    ASSERT_EQ( PoiS2::orientV(uu,vv), +1);
    ASSERT_EQ( PoiS2::orientV(vv,uu), -1);

}

TEST(GeomTest, poiS2OrientE) {
    PoiS2 p = PoiS2( 108.0, 45.0); 
    PoiS2 q = PoiS2(-108.0,-45.0); 
    PoiS2 r = PoiS2( 108.00000000,
                     45.000000001 );

    ASSERT_EQ(  PoiS2::orientE(p,q,r), +1 );
    ASSERT_EQ(  PoiS2::orientE(p,q,p), 0 );
    ASSERT_EQ(  PoiS2::orientE(p,q,q), 0 );
    ASSERT_EQ(  PoiS2::orientE(p,r,q), -1 );
}

TEST(GeomTest, poiS2OrientV_II) {
    double d = 0.00001;
    ASSERT_EQ( +1, PoiS2::orientV(
        PoiS2(-108.0+d  ,-45.0),
        PoiS2(+108.0+d  ,+45.0),
        PoiS2(-108.0+d  ,+45.0),
        PoiS2(+108.0+d  ,-45.0),
        PoiS2(-108.0+2*d,-45.0),
        PoiS2(+108.0+2*d,+45.0),
        PoiS2(-108.0+2*d,+45.0),
        PoiS2(+108.0+2*d,-45.0)       )  );

    ASSERT_EQ( -1, PoiS2::orientV(
        PoiS2(-108.0+d  ,-45.0),
        PoiS2(+108.0+d  ,+45.0),
        PoiS2(-108.0+d  ,+45.0),
        PoiS2(+108.0+d  ,-45.0),
        PoiS2(-108.0-d  ,-45.0),
        PoiS2(+108.0-d  ,+45.0),
        PoiS2(-108.0-d  ,+45.0),
        PoiS2(+108.0-d  ,-45.0)       )  );

}


TEST(GeomTest, poiS2OrientV_IE) {
    PoiS2 p = PoiS2( 108.1, 45.0); 
    PoiS2 q = PoiS2(-107.9,-45.0); 
    PoiS2 s = PoiS2( 108.1,-45.0); 
    PoiS2 t = PoiS2(-107.9, 45.0); 
    PoiS2 r = PoiS2(  0.10000001,
                     45.000000001 );
    PoiS2 u = PoiS2(  0.09999000,
                     45.000000001 );
    PoiS2 v = PoiS2(  0.10000000,
                      0.000000000 );

    ASSERT_EQ(  PoiS2::orientV(q,p,t,s,r), +1 );
    ASSERT_EQ(  PoiS2::orientV(q,p,t,s,u), -1 );
    // ASSERT_EQ(  PoiS2::orientV(q,p,t,s,v),  0 ); // Imprecision (lat,lon)->(x,y,z) ?
}


TEST(GeomTest, poiS2Constructor) {
    PoiS2 p = PoiS2(108.0, 45.0); 
    PoiS2 q = PoiS2(108.0, 45.0); 
    PoiS2 r = PoiS2( 108.00000001,
                     45.000000001 );
    ASSERT_TRUE(  p == q );
    ASSERT_FALSE( q == r );
}

// TEST(GeomTest, segS2Constructor) {
//     PoiS2 p = PoiS2( 108.0,  45.0); 
//     PoiS2 q = PoiS2(-108.0, -45.0); 

//     SegS2 seg_1 = SegS2(p, q, 1);
//     SegS2 seg_2 = SegS2(q, p, 1);

//     ASSERT_EQ(seg_1.get_source(), q);
//     ASSERT_EQ(seg_2.get_source(), q);
// }