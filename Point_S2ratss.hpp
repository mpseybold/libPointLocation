#pragma once
// #include <iostream>
#include <string>

#include <CGAL/Point_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/enum.h>

#include <libratss/ProjectS2.h>
// #include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/intersections.h>
#include <CGAL/Direction_3.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
typedef typename Kernel::FT         FT;
using Point_3   = CGAL::Point_3<Kernel>;
using Line_3    = CGAL::Line_3<Kernel>;
using Vector_3  = CGAL::Vector_3<Kernel>;
using lazy      = CGAL::Lazy_exact_nt<CGAL::Epeck_ft>;

// using TraitsXY  = CGAL::Projection_traits_xy_3<Kernel>;
using PointXY   = CGAL::Point_2<Kernel>;
// using SegmentXY = CGAL::Segment_2<Kernel>;
using Plane_3   = CGAL::Plane_3<Kernel>;
// typedef Kernel::Orientation_2 OrientationXY;
using Intersect_3 = Kernel::Intersect_3;
using Direction_3 = CGAL::Direction_3<Kernel>;

// #DEFINE PRECISION_FOR_SNAPPING 31


class PointS2ratss {
    private:
        Point_3 cgal_point;
        
        inline static ratss::ProjectS2   projection;
        inline static int                projectionPrecision = 15;
        // inline static traitsXY           projectionXY;

        // inline static OrientationXY      orientationXY;

        inline static int orientV(Point_3& self, Point_3& other );

    public:
        PointS2ratss(double lon, double lat) {
            mpfr::mpreal _lat (lat);
            mpfr::mpreal _lon (lon);
            FT xs, ys, zs;

            projection.projectFromGeo<FT>(_lat, _lon, xs,ys,zs, projectionPrecision ); 
            cgal_point = Point_3(xs, ys, zs);
            assert( zs < 1.0 && zs > 1.0 );     // Neither North nor South pole
        }

        Point_3& get_cgal_point() {
            return cgal_point;
        }

        inline static bool isValidSegment(PointS2ratss& src, PointS2ratss& trg){
            Point_3& s = src.get_cgal_point(),
                     t = trg.get_cgal_point();
            return     -(s.x()) != t.x()
                    || -(s.y()) != t.y()
                    || -(s.z()) != t.z() ;
        }

        static int v_orientation(PointS2ratss& self, PointS2ratss& other){
            return orientV( self.cgal_point, other.cgal_point );
        }

        // Determines the oriented side of the line through
        // source and target which contains p
        // -1 negative
        // 1 positive
        // 0 on the line
        static int line_orientation(
            PointS2ratss source, PointS2ratss target, PointS2ratss p);

        // Detmines the oriented side of the vertical
        // line through the intersection of line(s1, t2)
        // and line line(s2, t2) which contains p
        static int intersection_v_orientation(
            PointS2ratss s_1, PointS2ratss t_1,
            PointS2ratss s_2, PointS2ratss t_2,
            PointS2ratss p
        );
        
        // Compares the intersection of l(s, t) and l(s1, t1)
        // with the intersection of l(s, t) and l(s2, t2)
        static int intersection_orientation(
            PointS2ratss s, PointS2ratss t,
            PointS2ratss s_1, PointS2ratss t_1,
            PointS2ratss s_2, PointS2ratss t_2
        );

        // lazy x() { return cgal_point.x(); }
        // lazy y() { return cgal_point.y(); }        
        
};


// Point_2 shear(Point_2 p) {
//     return Point_2(p.x() + p.y(), p.y());
// }

// Predicate for vert orientation on the cylindrical surface (left=-1; on=0; right=+1)
// Returns `left' iff `other' is left of the vertical half great circle through North,point,South
int PointS2ratss::orientV( Point_3& self, Point_3& other) {
    //TODO: predicate signatures should be call-by-reference

    // Algo:
    // * IF point == other THEN  return 0;
    // * Drop the z coodrinate and consider the 2D orientation(pXY, ORIGIN, oXY)
    // * CASE: pXY is WEST    CASE pXY EAST is symmetric
    //      * IF  orient == RIGHT_TURN               THEN    return +1;
    //      * EIF orient == LEFT_TURN                THEN    return (oXY WEST)? +1 : -1;
    //      * EIF orient == COLLINEAR && oXY EAST    THEN    return +1
    //      * EIF orient == COLLINEAR                THEN    return (p.z)? +1 : -1
    

    PointXY selfXY( self.x(), self.y() );
    PointXY otherXY( other.x(), other.y() );    
    



    // switch ( orientationXY( pXY, CGAL::ORIGIN, oXY ) ){
    //     case CGAL::COLLINEAR: 
    //         std::cout << "are collinear\n";
    //         break;
    //     case CGAL::LEFT_TURN:
    //         std::cout << "make a left turn\n";
    //         break;
    //     case CGAL::RIGHT_TURN: 
    //         std::cout << "make a right turn\n";
    //         break;
    // }
    
    // Both, East and West, are half-closed surfaces
    // bool pEast = p.x < 0 || ( p.x==0 && p.y < 0);
    // bool oEast = o.x < 0 || ( o.x==0 && o.y < 0); 

    return 0;
}

// Assumes:     source left-of target  AND  isValidSeg( source, target )
// Returns -1/0/+1 for below/on/above
int PointS2ratss::line_orientation(
    PointS2ratss source, PointS2ratss target, PointS2ratss p) {
        Point_3& src = source.cgal_point,
                 trg = target.cgal_point,
                 _p  =      p.cgal_point;
        // Convention: looking on the plane-defining points from positive half gives points in CCW order
        Plane_3 plane (CGAL::ORIGIN, src, trg);
        // assumes  ON_NEGATIVE_SIDE = -1, ON_ORIENTED_BOUNDARY = 0, ON_POSITIVE_SIDE = 1
        return plane.oriented_side( _p );
}


// ASSUMES:  Arc(s1,t1) and Arc(s2,t2) are crossing
int PointS2ratss::intersection_v_orientation(
            PointS2ratss s_1, PointS2ratss t_1,
            PointS2ratss s_2, PointS2ratss t_2,
            PointS2ratss p
        ) {
            Point_3& _s1 = s_1.cgal_point, _t1 = t_1.cgal_point,
                     _s2 = s_2.cgal_point, _t2 = t_2.cgal_point,
                     _p  = p.cgal_point;
                     
            Plane_3 seg1 (CGAL::ORIGIN, _s1,_t1),
                    seg2 (CGAL::ORIGIN, _s2,_t2);
            CGAL::cpp11::result_of<Intersect_3(Plane_3, Plane_3)>::type
                result = intersection(seg1, seg2);
            if (result) {
                if (const Line_3* line = boost::get<Line_3>(&*result)) {
                    Direction_3 posDir = line->direction();
                    Point_3 i ( posDir.dx(),  posDir.dy(),  posDir.dz() );

                    assert( orientV(_s1, i) == +1 );
                    assert( orientV(_t1, i) == -1 );
                    assert( orientV(_s2, i) == +1 );
                    assert( orientV(_t2, i) == -1 );

                    return orientV(i, _p);
                } else {
                    // TODO: exception segs are non-crossing!
                }
            }
    
}

int PointS2ratss::intersection_orientation(
            PointS2ratss s,     PointS2ratss t,
            PointS2ratss s_1,   PointS2ratss t_1,
            PointS2ratss s_2,   PointS2ratss t_2
        ) {

    // Line_2 line = Line_2(s.get_cgal_point(), t.get_cgal_point());
    // Line_2 line_1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    // Line_2 line_2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());

    // auto intersection_1 = CGAL::intersection(line, line_1);
    // Point_2 ip_1;

    // if (intersection_1) {
    //     ip_1 = boost::get<Point_2>(*intersection_1);
    // }   else {
    //     throw std::logic_error("lines do not intersect");
    // }

    // auto intersection_2 = CGAL::intersection(line, line_2);
    // Point_2 ip_2;

    // if (intersection_2) {
    //     ip_2 = boost::get<Point_2>(*intersection_2);
    // } else {
    //     throw std::logic_error("lines do not intersect");
    // }

    // if (ip_2.x() < ip_1.x())
    //     return 1;
    // if (ip_2.x() > ip_1.x())
    //     return -1;

    // Line_2 v_line = Line_2(shear(ip_1), Vector_2(0, 1));

    // auto side = v_line.oriented_side(shear(ip_2));

    // switch(side) {
    //     case CGAL::ON_NEGATIVE_SIDE:
    //         return -1;
    //     case CGAL::ON_POSITIVE_SIDE:
    //         return 1;
    //     case CGAL::ON_ORIENTED_BOUNDARY:
    //         return 0;
    // }

    // throw std::logic_error("failed to compare intersection points");
}