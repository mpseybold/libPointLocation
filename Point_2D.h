#pragma once

#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/enum.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Point_2 = CGAL::Point_2<Kernel>;
using Line_2 = CGAL::Line_2<Kernel>;
using Vector_2 = CGAL::Vector_2<Kernel>;
using lazy = CGAL::Lazy_exact_nt<CGAL::Epeck_ft>;

class PointCart {

    private:
        Point_2 cgal_point;

    public:

        const static int EXTREME_LEFT = -10000;
        const static int EXTREME_RIGHT = 10000;
        const static int EXTREME_TOP = 10000;
        const static int EXTREME_BOTTOM = -10000;
        const static int EPS = 1;

        PointCart(double x, double y) {
            cgal_point = Point_2(x, y);
        }

        Point_2 get_cgal_point() {
            return cgal_point;
        }

        // Compares the x coordinates of a and b using an implicit
        // shear transformation. 
        // Returns 
        // -1 if b is left of a
        // 0 if a and b conincide
        // 1 if b is right of a 
        static int orientV(PointCart p, PointCart other_p);

        // Determines the oriented side of the line through
        // source and target which contains p
        // -1 negative
        // 1 positive
        // 0 on the line
        static int orientE(
            PointCart source, PointCart target, PointCart p);

        // Detmines the oriented side of the vertical
        // line through the intersection of line(s1, t2)
        // and line line(s2, t2) which contains p
        static int orientV(
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2,
            PointCart p
        );
        
        // Detmines the side of the line through the intersection 
        // of l(s, t) and l(s_1, t_1) contains the intersection of
        // l(s, t) and l(s_2, t_2)
        static int orientV(
            PointCart s, PointCart t,
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2
        );

        // compares vertical lines of intersection of l(s_1, t_1) and l(s_2, t_2)
        // with the intersection of l(s_3, t_3) and l(s_4, t_4)
        static int orientV(
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2,
            PointCart s_3, PointCart t_3,
            PointCart s_4, PointCart t_4
        );

        // detmines the side of l(s_1, t_1)
        // which contains the intersection
        // of l(s_2, t_2) and l(s_3, t_3)
        static int orientE(
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2,
            PointCart s_3, PointCart t_3
        );

        // compares horizontal lines of intersection of l(s_1, t_1) and l(s_2, t_2)
        // with the intersection of l(s_3, t_3) and l(s_4, t_4)
        static int intersection_h_orientation(
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2,
            PointCart s_3, PointCart t_3,
            PointCart s_4, PointCart t_4
        );

        static int slope_comparison(
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2
        );

        lazy x() { return cgal_point.x(); }
        lazy y() { return cgal_point.y(); }        
};
