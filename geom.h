#pragma once

#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Point_2 = CGAL::Point_2<Kernel>;
using Line_2 = CGAL::Line_2<Kernel>;
using Vector_2 = CGAL::Vector_2<Kernel>;
using lazy = CGAL::Lazy_exact_nt<CGAL::Epeck_ft>;

class PointCart {

    private:
        Point_2 cgal_point;

    public:

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
        static int v_orientation(PointCart a, PointCart b);

        lazy x() { return cgal_point.x(); }
        lazy y() { return cgal_point.y(); }        
};

class LineCart {

    private:
        Line_2 cgal_line;

    public:

        LineCart() {}

        LineCart(PointCart a, PointCart b) {
            cgal_line = Line_2(a.get_cgal_point(), b.get_cgal_point());
        }

        bool has_on_positive_side(PointCart p) {
            return cgal_line.has_on_positive_side(p.get_cgal_point());
        }

};


template <class PointType>
class Segment {

    private:
    
        PointType source;
        PointType target;
    
    public:

        Segment(PointType s, PointType t) : source(s), target(t) {}

        PointType get_source() { return source; }
        PointType get_target() { return target; }


};
