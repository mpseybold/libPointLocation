#include "geom.h"

int PointCart::v_orientation(PointCart a, PointCart b) {
    
    Point_2 a_cgal = a.get_cgal_point();
    Point_2 b_cgal = b.get_cgal_point();

    Point_2 a_shear = Point_2(
        a_cgal.x() + a_cgal.y(), a_cgal.y()
    );
    
    Point_2 b_shear = Point_2(
        b_cgal.x() + b_cgal.y(), b_cgal.y()
    );
    

    Line_2 line_through_a = Line_2(a_shear, Vector_2(0, 1));


    return line_through_a.oriented_side(b_cgal);
}