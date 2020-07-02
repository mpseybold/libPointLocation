#pragma once

#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Point_2 = CGAL::Point_2<Kernel>;
using Line_2 = CGAL::Line_2<Kernel>;
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

        // Check if 'other' is on the positive side of
        // vertical line through this point using an
        // implicit shear transformation
        bool has_on_positive_side(PointCart other) {

            Point_2 other_cgal = other.get_cgal_point();
            lazy shear = cgal_point.x() + cgal_point.y();
            lazy other_shear = other_cgal.x() + other_cgal.y();

            return other_shear > shear;
        }

        lazy x() { return cgal_point.x(); }
        lazy y() { return cgal_point.y(); }
        
};


// template <class PointType>
// class Segment {

//     private:
    
//         PointType source;
//         PointType target;
    
//     public:

//         Segment(PointType s, PointType t) : source(s), target(t) {}

//         PointType get_source() { return source; }
//         PointType get_target() { return target; }


// };
