#include "Point_S2ratss.h"
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

int PointS2ratss::orientV(
            PointS2ratss s, PointS2ratss t,
            PointS2ratss s_1, PointS2ratss t_1,
            PointS2ratss s_2, PointS2ratss t_2
        ) {
    return orientV(
        s, t, s_1, t_1,
        s, t, s_2, t_2
    );
}

int PointS2ratss::orientE(
            PointS2ratss s1, PointS2ratss t1,
            PointS2ratss s2, PointS2ratss t2,
            PointS2ratss s3, PointS2ratss t3
        ) {
    Point_3 i1 = intersection(   s2.cgal_point, t2.cgal_point,
                                 s3.cgal_point, t3.cgal_point  );      

    return orientE(s1, t1, PointS2ratss(CGAL::to_double(i1.x()), CGAL::to_double(i1.y())));
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

int PointS2ratss::slope_comparison(
        PointS2ratss s_1, PointS2ratss t_1,
        PointS2ratss s_2, PointS2ratss t_2
    ) {

    Vector_3 n_1 = CGAL::normal(Point_3(0, 0, 0), s_1.get_cgal_point(), t_1.get_cgal_point());
    Vector_3 n_2 = CGAL::normal(Point_3(0, 0, 0), s_2.get_cgal_point(), t_2.get_cgal_point());
    Vector_3 cross = CGAL::cross_product(n_2, n_1);

    auto volume = CGAL::scalar_product(cross, cross);

    if (volume > 0)
        return 1;
    if (volume < 0)
        return -1;
    return 0;
}