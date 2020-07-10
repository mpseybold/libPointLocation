#include "geom.h"

namespace {
    Point_2 shear(Point_2 p) {
        return Point_2(p.x() + p.y(), p.y());
    }
}

int PointCart::v_orientation(PointCart a, PointCart b) {
    
    Point_2 a_cgal = a.get_cgal_point();
    Point_2 b_cgal = b.get_cgal_point();

    if (a_cgal.x() > b_cgal.x()) 
        return 1;
    if (a_cgal.x() < b_cgal.x())
        return -1;

    if (a_cgal.y() < b_cgal.y())
        return -1;
    if (a_cgal.y() > b_cgal.y())
        return 1;

    return 0;
}

int PointCart::line_orientation(
    PointCart source, PointCart target, PointCart p) {

    if (source.x() == target.x())
        return v_orientation(source, p);

    Line_2 line = Line_2(source.get_cgal_point(), target.get_cgal_point());

    auto side = line.oriented_side(p.get_cgal_point());

    switch(side) {
        case CGAL::ON_NEGATIVE_SIDE:
            return -1;
        case CGAL::ON_POSITIVE_SIDE:
            return 1;
        case CGAL::ON_ORIENTED_BOUNDARY:
            return 0;
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
        return 1;
    if (p.x() > ip.x())
        return -1;
    
    Point_2 ip_shear = shear(ip);
    Point_2 p_shear = shear(p.get_cgal_point());

    Line_2 v_line = Line_2(ip_shear, Vector_2(0, 1));

    auto side = v_line.oriented_side(p_shear);

    switch(side) {
        case CGAL::ON_NEGATIVE_SIDE:
            return -1;
        case CGAL::ON_POSITIVE_SIDE:
            return 1;
        case CGAL::ON_ORIENTED_BOUNDARY:
            return 0;
    }

    throw std::logic_error("failed to compare point with intersection");
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
        return 1;
    if (ip_2.x() > ip_1.x())
        return -1;

    Line_2 v_line = Line_2(shear(ip_1), Vector_2(0, 1));

    auto side = v_line.oriented_side(shear(ip_2));

    switch(side) {
        case CGAL::ON_NEGATIVE_SIDE:
            return -1;
        case CGAL::ON_POSITIVE_SIDE:
            return 1;
        case CGAL::ON_ORIENTED_BOUNDARY:
            return 0;
    }

    throw std::logic_error("failed to compare intersection points");
}