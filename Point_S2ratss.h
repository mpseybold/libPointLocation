#pragma once
// #include <iostream>
#include <string>

#include <CGAL/Point_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Ray_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/enum.h>

// #include <libratss/ProjectS2.h>
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

        const static int EXTREME_LEFT = -180;
        const static int EXTREME_RIGHT = 180;
        const static int EXTREME_TOP = 90;
        const static int EXTREME_BOTTOM = -90;
        const static int EPS = 1;

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

        // Detmines the side of the line through the intersection 
        // of l(s, t) and l(s_1, t_1) contains the intersection of
        // l(s, t) and l(s_2, t_2)
        static int orientV(
            PointS2ratss s, PointS2ratss t,
            PointS2ratss s_1, PointS2ratss t_1,
            PointS2ratss s_2, PointS2ratss t_2
        );

        // detmines the side of l(s_1, t_1)
        // which contains the intersection
        // of l(s_2, t_2) and l(s_3, t_3)
        static int orientE(
            PointS2ratss s1, PointS2ratss t1,
            PointS2ratss s2, PointS2ratss t2,
            PointS2ratss s3, PointS2ratss t3
        );

        static int slope_comparison(
            PointS2ratss s_1, PointS2ratss t_1,
            PointS2ratss s_2, PointS2ratss t_2
        );
};