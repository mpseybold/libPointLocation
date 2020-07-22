#pragma once
// #include <iostream>
#include <string>

#include <CGAL/Point_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/enum.h>

#include <libratss/ProjectS2.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
typedef typename Kernel::FT         FT;
using Point_3 = CGAL::Point_3<Kernel>;
using Line_3 = CGAL::Line_2<Kernel>;
using Vector_3 = CGAL::Vector_3<Kernel>;
using lazy = CGAL::Lazy_exact_nt<CGAL::Epeck_ft>;


// #DEFINE PRECISION_FOR_SNAPPING 31



class PointS2ratss {
    private:
        Point_3 cgal_point;
        
        static ratss::ProjectS2 proj;

    public:
        PointS2ratss(double lon, double lat) {

            mpfr::mpreal _lat (lat);
            mpfr::mpreal _lon (lon);
            FT xs, ys, zs;

            proj.projectFromGeo<FT>(_lat, _lon, xs,ys,zs, 15); 
            cgal_point = Point_3(xs, ys, zs);
            
            
        }

        Point_3 get_cgal_point() {
            return cgal_point;
        }

        // Compares the x coordinates of a and b using an implicit
        // shear transformation. 
        // Returns 
        // -1 if b is left of a
        // 0 if a and b conincide
        // 1 if b is right of a 
        static int v_orientation(PointS2ratss a, PointS2ratss b);

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

int PointS2ratss::v_orientation(PointS2ratss a, PointS2ratss b) {
    
    // Point_2 a_cgal = a.get_cgal_point();
    // Point_2 b_cgal = b.get_cgal_point();

    // if (a_cgal.x() > b_cgal.x()) 
    //     return 1;
    // if (a_cgal.x() < b_cgal.x())
    //     return -1;

    // if (a_cgal.y() < b_cgal.y())
    //     return -1;
    // if (a_cgal.y() > b_cgal.y())
    //     return 1;

    return 0;
}

int PointS2ratss::line_orientation(
    PointS2ratss source, PointS2ratss target, PointS2ratss p) {

    // if (source.x() == target.x())
    //     return v_orientation(source, p);

    // Line_2 line = Line_2(source.get_cgal_point(), target.get_cgal_point());

    // auto side = line.oriented_side(p.get_cgal_point());

    // switch(side) {
    //     case CGAL::ON_NEGATIVE_SIDE:
    //         return -1;
    //     case CGAL::ON_POSITIVE_SIDE:
    //         return 1;
    //     case CGAL::ON_ORIENTED_BOUNDARY:
    //         return 0;
    // }

    // throw std::logic_error("error: failed to compare point with line");
}

int PointS2ratss::intersection_v_orientation(
            PointS2ratss s_1, PointS2ratss t_1,
            PointS2ratss s_2, PointS2ratss t_2,
            PointS2ratss p
        ) {

    // Line_2 line1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    // Line_2 line2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());

    // auto result = CGAL::intersection(line1, line2);
    // Point_2 ip;
    
    // if (result) {
    
    //     ip = boost::get<Point_2>(*result);
    
    // } else {

    //     throw std::logic_error("lines do not intersect");
    
    // }

    // if (p.x() < ip.x())
    //     return 1;
    // if (p.x() > ip.x())
    //     return -1;
    
    // Point_2 ip_shear = shear(ip);
    // Point_2 p_shear = shear(p.get_cgal_point());

    // Line_2 v_line = Line_2(ip_shear, Vector_2(0, 1));

    // auto side = v_line.oriented_side(p_shear);

    // switch(side) {
    //     case CGAL::ON_NEGATIVE_SIDE:
    //         return -1;
    //     case CGAL::ON_POSITIVE_SIDE:
    //         return 1;
    //     case CGAL::ON_ORIENTED_BOUNDARY:
    //         return 0;
    // }

    // throw std::logic_error("failed to compare point with intersection");
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