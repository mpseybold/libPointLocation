#pragma once
// #include <iostream>
#include <string>

#include <CGAL/Point_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Ray_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/enum.h>

#include <libratss/ProjectS2.h>
// #include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/intersections.h>
#include <CGAL/Direction_3.h>

#define VERBOSITY 100

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
typedef typename Kernel::FT         FT;
using Point_3   = CGAL::Point_3<Kernel>;
using Line_3    = CGAL::Line_3<Kernel>;
using Ray_3     = CGAL::Ray_3<Kernel>;
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

Point_3  const northPole = Point_3 (0.0, 0.0, +1.0 );
Vector_3 const northPoleVec=Vector_3(CGAL::ORIGIN, northPole );


class PointS2ratss {
    private:
        Point_3 cgal_point;
        // inline static ratss::ProjectS2   projection;
        // inline static int                projectionPrecision = 31;
        

        void initSimpler(double lon, double lat){
            lon = lon - 90.0;   // 0-Meridian is at x=0
            lon = M_PI * lon/180.0;
            lat = M_PI * lat/180.0;
            double x = cos(lon) * cos( lat );
            double y = sin(lon) * cos( lat );
            double z = sin( lat );
            cgal_point = Point_3( x, y, z );
        }

        // inline static traitsXY           projectionXY;
        // inline static OrientationXY      orientationXY;

        inline static int       orientV( Point_3& self, Point_3& other );
        inline static Point_3   intersection( Point_3& s1, Point_3& t1, Point_3& s2, Point_3& t2 );

    public:

        PointS2ratss(double x, double y, double z){
            assert( x != 0.0 || y != 0.0 ); // Neither S/N-pole nor length==0;
            cgal_point = Point_3(x,y,z);
        }

        PointS2ratss(double lon, double lat) {
            // mpfr::mpreal _lat (lat);
            // mpfr::mpreal _lon (lon - 90.0);
            // FT xs, ys, zs;

            // projection.projectFromGeo<FT>(_lat, _lon, xs,ys,zs, projectionPrecision ); 
            // cgal_point = Point_3(xs, ys, zs);
            
            initSimpler(lon,lat);
            assert( cgal_point.z() < 1.0 && cgal_point.z() > -1.0 );
            // assert( zs < 1.0 && zs > -1.0 );     // Neither North nor South pole
        }

        Point_3& get_cgal_point() {
            return cgal_point;
        }

        friend bool operator==(const PointS2ratss& lhs, const PointS2ratss& rhs ){
            // TODO: this assumes length==1; Do isCOLLINEAR instead !
            return lhs.cgal_point == rhs.cgal_point;
        }

        inline static bool isValidSegment(PointS2ratss& src, PointS2ratss& trg){
            Point_3& s = src.get_cgal_point(),
                     t = trg.get_cgal_point();
            return     -(s.x()) != t.x()
                    || -(s.y()) != t.y()
                    || -(s.z()) != t.z() ;
        }

        inline static int orientV(PointS2ratss& self, PointS2ratss& other){
            return orientV( self.cgal_point, other.cgal_point );
        }

        // Determines the oriented side of the line through
        // source and target which contains p
        // -1 negative
        // 1 positive
        // 0 on the line
        static int orientE(
            PointS2ratss source, PointS2ratss target, PointS2ratss p);

        // Detmines the oriented side of the vertical
        // line through the intersection of line(s1, t2)
        // and line line(s2, t2) which contains p
        static int orientV(
            PointS2ratss s_1, PointS2ratss t_1,
            PointS2ratss s_2, PointS2ratss t_2,
            PointS2ratss p
        );
        
        // Compares the intersection of l(s, t) and l(s1, t1)
        // with the intersection of l(s, t) and l(s2, t2)
        static int orientV(
            PointS2ratss s1,   PointS2ratss t1,
            PointS2ratss s2,   PointS2ratss t2,
            PointS2ratss s3,   PointS2ratss t3,
            PointS2ratss s4,   PointS2ratss t4
        );
};




// Point_2 shear(Point_2 p) {
//     return Point_2(p.x() + p.y(), p.y());
// }

// Predicate for vert orientation on the cylindrical surface (left=-1; on=0; right=+1)
// Returns `left' iff `other' is left of the vertical half great circle through North,point,South
int PointS2ratss::orientV( Point_3& self, Point_3& other) {

    // This ASSUMES all points having length==1 !!!
    // if( self.x() == other.x() &&
    //     self.y() == other.y() &&
    //     self.z() == other.z()   )
    //     return 0;

    // Ray_3 selfRay( CGAL::ORIGIN, self );
    if( Ray_3( CGAL::ORIGIN, self ).has_on(other) )
        return 0;

    PointXY selfXY(   self.x(), self.y()  ),
            otherXY( other.x(), other.y() ),
            myOrigin = CGAL::ORIGIN;    

    // TODO: Handle NORTH-pole as leftmost and SOUTH-pole as rightmost points
    assert(  self.x() != 0 ||  self.y() != 0 );
    assert( other.x() != 0 || other.y() != 0 );

    //         ^y
    //     +\- |+ o 
    //       s | /
    //        \|/    
    // --------+-------> x  
    //         '\
    // West    ' \    East          Both are half-closed surfaces
    //       lon=0
    bool   isSelfWest =  self.x() < 0 || (  self.x()==0 &&  self.y() > 0),
         isOtherfWest = other.x() < 0 || ( other.x()==0 && other.y() > 0);      // TODO: simplify 

    if( isSelfWest ){
        switch ( CGAL::orientation( selfXY, myOrigin, otherXY ) ){
            case CGAL::RIGHT_TURN: 
                return +1;
            case CGAL::LEFT_TURN:
                return  isOtherfWest ? -1 : +1;
            case CGAL::COLLINEAR: 
                if( !isOtherfWest ){
                    return +1;
                } else{

                    Point_3  moreNorth = CGAL::ORIGIN + cross_product(self - CGAL::ORIGIN, other - CGAL::ORIGIN);
                    PointXY  moreNorthXY = PointXY( moreNorth.x(), moreNorth.y() );
                    if( VERBOSITY >= 100 ){
                        std::cout <<"moreNorth=" << moreNorth << std::endl;
                        std::cout <<"moreNorthXY=" << moreNorthXY << std::endl;
                    }

                    // // Convention: looking on the plane-defining points from positive half gives points in CCW order
                    // Plane_3 plane (CGAL::ORIGIN, self, other);
                    // // assumes  ON_NEGATIVE_SIDE = -1, ON_ORIENTED_BOUNDARY = 0, ON_POSITIVE_SIDE = 1
                    // return plane.oriented_side( moreNorth );

                    int result = CGAL::orientation( moreNorthXY, myOrigin, selfXY ); 
                    assert(result != 0 );
                    return result;

                    // ASSUMES INTERSECTION POINT HAS LENGTH==1
                    // return (other.z() > self.z()) ? -1 : +1;    // s.z != o.z since points are distinct and length==1
                }
        }
    } else {
        switch ( CGAL::orientation( selfXY, myOrigin, otherXY ) ){
            case CGAL::LEFT_TURN: 
                return -1;
            case CGAL::RIGHT_TURN:
                return  isOtherfWest ? -1 : +1;
            case CGAL::COLLINEAR: 
                if( isOtherfWest ){
                    return -1;
                } else{
                    return (self.z() > other.z()) ? +1 : -1;    // s.z != o.z since points are distinct
                }
        }
    }
}

// Assumes:     source left-of target  AND  isValidSeg( source, target )
// Returns -1/0/+1 for below/on/above
int PointS2ratss::orientE(
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
int PointS2ratss::orientV(
            PointS2ratss s_1, PointS2ratss t_1,
            PointS2ratss s_2, PointS2ratss t_2,
            PointS2ratss p
        ) {
    Point_3 i = intersection(   s_1.cgal_point, t_1.cgal_point,
                                s_2.cgal_point, t_2.cgal_point  );
    return orientV( i, p.cgal_point );    
}

int PointS2ratss::orientV(
            PointS2ratss s1,   PointS2ratss t1,
            PointS2ratss s2,   PointS2ratss t2,
            PointS2ratss s3,   PointS2ratss t3,
            PointS2ratss s4,   PointS2ratss t4
        ) {
    Point_3 i1 = intersection(   s1.cgal_point, t1.cgal_point,
                                 s2.cgal_point, t2.cgal_point  );
    Point_3 i2 = intersection(   s3.cgal_point, t3.cgal_point,
                                 s4.cgal_point, t4.cgal_point  );
    return orientV(i1, i2);
}

Point_3 PointS2ratss::intersection( Point_3& s1, Point_3& t1, Point_3& s2, Point_3& t2 ){
    Plane_3 seg1 (CGAL::ORIGIN, s1, t1),
            seg2 (CGAL::ORIGIN, s2, t2);
    CGAL::cpp11::result_of<Intersect_3(Plane_3, Plane_3)>::type
        result = CGAL::intersection(seg1, seg2);
    if (result) {
        if (const Line_3* line = boost::get<Line_3>(&*result)) {
            Direction_3 posDir = line->direction();
            Point_3 i ( posDir.dx(),  posDir.dy(),  posDir.dz() );  // TODO: Does it matter that this has not length==1 ?!?!?

            assert( orientV(s1, i) == +1 );
            assert( orientV(t1, i) == -1 );
            assert( orientV(s2, i) == +1 );
            assert( orientV(t2, i) == -1 );

            return i;
        } else {
            assert( false );
            // TODO: exception segs are non-crossing!
        }
    }
}