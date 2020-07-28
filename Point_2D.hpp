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

        // Determines the oriented side of the line through
        // source and target which contains p
        // -1 negative
        // 1 positive
        // 0 on the line
        static int line_orientation(
            PointCart source, PointCart target, PointCart p);

        // Detmines the oriented side of the vertical
        // line through the intersection of line(s1, t2)
        // and line line(s2, t2) which contains p
        static int intersection_v_orientation(
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2,
            PointCart p
        );
        
        // Compares the intersection of l(s, t) and l(s1, t1)
        // with the intersection of l(s, t) and l(s2, t2)
        static int intersection_orientation(
            PointCart s, PointCart t,
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2
        );

<<<<<<< HEAD:geom.cpp
=======
        lazy x() { return cgal_point.x(); }
        lazy y() { return cgal_point.y(); }        
        
};


Point_2 shear(Point_2 p) {
    return Point_2(p.x() + p.y(), p.y());
}
>>>>>>> 5af8a9bf0016634b5828fdca7032381fc1cea160:Point_2D.hpp


int PointCart::v_orientation(PointCart p, PointCart other_p) {
    
    Point_2 p_cgal = p.get_cgal_point();
    Point_2 other_p_cgal = other_p.get_cgal_point();

    if (p_cgal.x() > other_p_cgal.x()) 
        return -1;
    if (p_cgal.x() < other_p_cgal.x())
        return 1;
    if (p_cgal.y() < other_p_cgal.y())
        return -1;
    if (p_cgal.y() > other_p_cgal.y())
        return 1;

    return 0;
}

int PointCart::line_orientation(
    PointCart source, PointCart target, PointCart p) {

    // if (source.x() == target.x())
    //     return v_orientation(source, p);

    Line_2 line = Line_2(source.get_cgal_point(), target.get_cgal_point());

    auto side = line.oriented_side(p.get_cgal_point());
    double x_1 = CGAL::to_double(source.get_cgal_point().x());
    double y_1 = CGAL::to_double(source.get_cgal_point().y());
    double x_2 = CGAL::to_double(target.get_cgal_point().x());
    double y_2 = CGAL::to_double(target.get_cgal_point().y());
    double p_x = CGAL::to_double(p.get_cgal_point().x());
    double p_y = CGAL::to_double(p.get_cgal_point().y());



    switch(side) {
        case CGAL::ON_NEGATIVE_SIDE:
            return -1;
        case CGAL::ON_POSITIVE_SIDE:
            return 1;
        case CGAL::ON_ORIENTED_BOUNDARY: {
            return 0;
        }
    }

    throw std::logic_error("error: failed to compare point with line");
}

int PointCart::intersection_v_orientation(
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2,
            PointCart p
        ) {

    Line_2 line1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    Line_2 line2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());

    auto result = CGAL::intersection(line1, line2);
    Point_2 ip;
    
    if (result) {
    
        ip = boost::get<Point_2>(*result);
    
    } else {

        throw std::logic_error("lines do not intersect");

    }

    if (p.x() < ip.x())
        return -1;
    if (p.x() > ip.x())
        return 1;
    if (p.y() < ip.y())
        return 1;
    if (p.y() > ip.y())
        return -1;
    
    return 0;
}

int PointCart::intersection_orientation(
            PointCart s, PointCart t,
            PointCart s_1, PointCart t_1,
            PointCart s_2, PointCart t_2
        ) {

    Line_2 line = Line_2(s.get_cgal_point(), t.get_cgal_point());
    Line_2 line_1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    Line_2 line_2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());

    auto intersection_1 = CGAL::intersection(line, line_1);
    Point_2 ip_1;

    if (intersection_1) {
        ip_1 = boost::get<Point_2>(*intersection_1);
    }   else {
        throw std::logic_error("lines do not intersect");
    }

    auto intersection_2 = CGAL::intersection(line, line_2);
    Point_2 ip_2;

    if (intersection_2) {
        ip_2 = boost::get<Point_2>(*intersection_2);
    } else {
        throw std::logic_error("lines do not intersect");
    }

    if (ip_2.x() < ip_1.x())
        return -1;
    if (ip_2.x() > ip_1.x())
        return 1;
    if (ip_2.y() > ip_1.y())
        return -1;
    if (ip_2.y() < ip_1.y())
        return 1;

    return 0;
}

int PointCart::intersection_v_orientation(
    PointCart s_1, PointCart t_1,
    PointCart s_2, PointCart t_2,
    PointCart s_3, PointCart t_3,
    PointCart s_4, PointCart t_4
) {
    Line_2 line_1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    Line_2 line_2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());
    Line_2 line_3 = Line_2(s_3.get_cgal_point(), t_3.get_cgal_point());
    Line_2 line_4 = Line_2(s_4.get_cgal_point(), t_4.get_cgal_point());

    auto intersection_1 = CGAL::intersection(line_1, line_2);
    auto intersection_2 = CGAL::intersection(line_3, line_4);

    Point_2 ip_1 = boost::get<Point_2>(*intersection_1);
    Point_2 ip_2 = boost::get<Point_2>(*intersection_2);

    if (ip_1.x() < ip_2.x())
        return 1;
    if (ip_1.x() > ip_2.x())
        return -1;
    if (ip_1.y() < ip_2.y())
        return -1;
    if (ip_1.y() > ip_2.y())
        return 1;

    return 0;
}

int PointCart::intersection_h_orientation(
    PointCart s_1, PointCart t_1,
    PointCart s_2, PointCart t_2,
    PointCart s_3, PointCart t_3,
    PointCart s_4, PointCart t_4
) {
    Line_2 line_1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    Line_2 line_2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());
    Line_2 line_3 = Line_2(s_3.get_cgal_point(), t_3.get_cgal_point());
    Line_2 line_4 = Line_2(s_4.get_cgal_point(), t_4.get_cgal_point());

    auto intersection_1 = CGAL::intersection(line_1, line_2);
    auto intersection_2 = CGAL::intersection(line_3, line_4);

    Point_2 ip_1 = boost::get<Point_2>(*intersection_1);
    Point_2 ip_2 = boost::get<Point_2>(*intersection_2);

    if (ip_1.y() < ip_2.y())
        return 1;
    if (ip_1.y() > ip_2.y())
        return -1;
    if (ip_1.x() < ip_2.x())
        return 1;
    if (ip_1.x() > ip_2.x())
        return -1;

    return 0;
}

int PointCart::slope_comparison(
    PointCart s_1, PointCart t_1,
    PointCart s_2, PointCart t_2
) {
    if (s_1.x() == t_1.x() && s_2.x() != t_2.x())
        return 1;

    if (s_2.x() == t_2.x() && s_1.x() != t_1.x())
        return -1;

    if (s_2.x() == t_2.x() && s_1.x() == t_1.x())
        return 0;

    lazy diff = (t_2.y() - s_2.y()) / (t_2.x() - s_2.x()) - 
    (t_1.y() - s_1.y()) / (t_1.x() - s_1.x());

    if (diff < 0)
        return -1;
    if (diff > 0)
        return 1;

    return 0;    
}

template <class PointType, class OrderType>
bool Segment<PointType, OrderType>::operator==(Segment<PointType, OrderType>& other_seg) {
    return PointType::v_orientation(source, other_seg.get_source()) == 0 &&
    PointType::v_orientation(target, other_seg.get_target()) == 0;
}


template class Segment<PointCart, int>;