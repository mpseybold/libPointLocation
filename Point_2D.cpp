#include "Point_2D.h"


int PointCart::orientV(PointCart& p, PointCart& other_p, bool shear) {
    predicate_count++;
    if (!EXACT) {
       if (p.X_() > other_p.X_()) 
        return -1;
        if (p.X_() < other_p.X_())
            return 1;
        if (shear) {
            if (p.Y_() < other_p.Y_())
                return -1;
            if (p.Y_() > other_p.Y_())
                return 1;
        }

        return 0;
    } 
    Point_2 p_cgal = p.get_cgal_point();
    Point_2 other_p_cgal = other_p.get_cgal_point();

    if (p_cgal.x() > other_p_cgal.x()) 
        return -1;
    if (p_cgal.x() < other_p_cgal.x())
        return 1;
    if (shear) {
        if (p_cgal.y() < other_p_cgal.y())
            return -1;
        if (p_cgal.y() > other_p_cgal.y())
            return 1;
    }

    return 0;
}

int PointCart::orientE(
    PointCart& source, PointCart& target, PointCart& p) {

    // if (source.x() == target.x())
    //     return v_orientation(source, p);
    predicate_count++;
    if (!EXACT) {
        double m = (target.Y_() - source.Y_())/(target.X_() - source.X_());
        if (p.Y_() > m*(p.X_()-source.X_()) + source.Y_())
            return 1;
        else if (p.Y_() < m*(p.X_()-source.X_()) + source.Y_())
            return -1;
        else 
            return 0;
    }

    Line_2 line = Line_2(source.get_cgal_point(), target.get_cgal_point());

    auto side = line.oriented_side(p.get_cgal_point());
    // double x_1 = CGAL::to_double(source.get_cgal_point().x());
    // double y_1 = CGAL::to_double(source.get_cgal_point().y());
    // double x_2 = CGAL::to_double(target.get_cgal_point().x());
    // double y_2 = CGAL::to_double(target.get_cgal_point().y());
    // double p_x = CGAL::to_double(p.get_cgal_point().x());
    // double p_y = CGAL::to_double(p.get_cgal_point().y());



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

int PointCart::orientV(
            PointCart& s_1, PointCart& t_1,
            PointCart& s_2, PointCart& t_2,
            PointCart& p, bool shear
        ) {
    predicate_count++;
    if (!EXACT) {
        double m_1 = (t_1.Y_() - s_1.Y_())/(t_1.X_() - s_1.X_());
        double m_2 = (t_2.Y_() - s_2.Y_())/(t_2.X_() - s_2.X_());

        double int_x;

        if (t_1.X_() == s_1.X_())
            int_x = t_1.X_();
        else if (t_2.X_() == s_2.X_())
            int_x = t_2.X_();
        else
            int_x = (s_2.Y_() - m_2*s_2.X_() - s_1.Y_() + m_1*s_1.X_())/(m_1-m_2);

        if (p.X_() < int_x)
            return -1;
        else if (p.X_() > int_x)
            return 1;
        else 
            return 0;
    }

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
    if (shear) {
        if (p.y() < ip.y())
            return 1;
        if (p.y() > ip.y())
            return -1;
    }
    return 0;
}

int PointCart::orientE(
    PointCart& s_1, PointCart& t_1,
    PointCart& s_2, PointCart& t_2,
    PointCart& s_3, PointCart& t_3
) {
    predicate_count++;
    Line_2 line_1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    Line_2 line_2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());
    Line_2 line_3 = Line_2(s_3.get_cgal_point(), t_3.get_cgal_point());

    auto intersection = CGAL::intersection(line_2, line_3);
    Point_2 ip;

    if (intersection) {
        ip = boost::get<Point_2>(*intersection);
    } else {
        assert(false);
    }

    auto side = line_1.oriented_side(ip);

    switch(side) {
        case CGAL::ON_NEGATIVE_SIDE:
            return -1;
        case CGAL::ON_POSITIVE_SIDE:
            return 1;
        case CGAL::ON_ORIENTED_BOUNDARY: {
            return 0;
        }
    }
}

int PointCart::orientV(
            PointCart& s, PointCart& t,
            PointCart& s_1, PointCart& t_1,
            PointCart& s_2, PointCart& t_2,
            bool shear
        ) {
            predicate_count++;
    Line_2 line = Line_2(s.get_cgal_point(), t.get_cgal_point());
    Line_2 line_1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    Line_2 line_2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());

    auto intersection_1 = CGAL::intersection(line, line_1);
    Point_2 ip_1;

    if (intersection_1) {
        ip_1 = boost::get<Point_2>(*intersection_1);
    }   else {
        assert(false);
        throw std::logic_error("lines do not intersect");
    }

    auto intersection_2 = CGAL::intersection(line, line_2);
    Point_2 ip_2;

    if (intersection_2) {
        ip_2 = boost::get<Point_2>(*intersection_2);
    } else {
        assert(false);
        throw std::logic_error("lines do not intersect");
    }

    if (ip_2.x() < ip_1.x())
        return -1;
    if (ip_2.x() > ip_1.x())
        return 1;
    if (shear) {
        if (ip_2.y() > ip_1.y())
            return -1;
        if (ip_2.y() < ip_1.y())
            return 1;
    }
    return 0;
}

int PointCart::orientV(
    PointCart& s_1, PointCart& t_1,
    PointCart& s_2, PointCart& t_2,
    PointCart& s_3, PointCart& t_3,
    PointCart& s_4, PointCart& t_4,
    bool shear
) {
    exit(0);
    predicate_count++;
    Line_2 line_1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    Line_2 line_2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());
    Line_2 line_3 = Line_2(s_3.get_cgal_point(), t_3.get_cgal_point());
    Line_2 line_4 = Line_2(s_4.get_cgal_point(), t_4.get_cgal_point());

    auto intersection_1 = CGAL::intersection(line_1, line_2);
    auto intersection_2 = CGAL::intersection(line_3, line_4);

    Point_2 ip_1;
    
    if (intersection_1) {
        ip_1 = boost::get<Point_2>(*intersection_1);
    } else {
        assert(false);
    }
    
    Point_2 ip_2;

    if (intersection_2) {
        ip_2 = boost::get<Point_2>(*intersection_2);
    } else {
        assert(false);
    }

    if (ip_1.x() < ip_2.x())
        return 1;
    if (ip_1.x() > ip_2.x())
        return -1;
    if (shear) {
        if (ip_1.y() < ip_2.y())
            return -1;
        if (ip_1.y() > ip_2.y())
            return 1;
    }
    return 0;
}

int PointCart::intersection_h_orientation(
    PointCart& s_1, PointCart& t_1,
    PointCart& s_2, PointCart& t_2,
    PointCart& s_3, PointCart& t_3,
    PointCart& s_4, PointCart& t_4
) {
    predicate_count++;
    Line_2 line_1 = Line_2(s_1.get_cgal_point(), t_1.get_cgal_point());
    Line_2 line_2 = Line_2(s_2.get_cgal_point(), t_2.get_cgal_point());
    Line_2 line_3 = Line_2(s_3.get_cgal_point(), t_3.get_cgal_point());
    Line_2 line_4 = Line_2(s_4.get_cgal_point(), t_4.get_cgal_point());

    auto intersection_1 = CGAL::intersection(line_1, line_2);
    auto intersection_2 = CGAL::intersection(line_3, line_4);

    Point_2 ip_1;
    
    if (intersection_1) {
        ip_1 = boost::get<Point_2>(*intersection_1);
    } else {
        assert(false);
    }
    
    Point_2 ip_2;

    if (intersection_2) {
        ip_2 = boost::get<Point_2>(*intersection_2);
    } else {
        assert(false);
    }

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
    PointCart& s_1, PointCart& t_1,
    PointCart& s_2, PointCart& t_2
) {
    predicate_count++;
    if (!EXACT) {
        if (s_1.X_() == t_1.X_() && s_2.X_() != t_2.X_())
        return 1;

        if (s_2.X_() == t_2.X_() && s_1.X_() != t_1.X_())
            return -1;

        if (s_2.X_() == t_2.X_() && s_1.X_() == t_1.X_())
            return 0;

        double d_diff = (t_2.Y_() - s_2.Y_()) / (t_2.X_() - s_2.X_()) - 
        (t_1.Y_() - s_1.Y_()) / (t_1.X_() - s_1.X_());

        if (d_diff < 0)
            return -1;
        if (d_diff > 0)
            return 1;

        return 0;    
    }
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

PointCart PointCart::intersection(
    PointCart& s_1, PointCart& t_1,
    PointCart& s_2, PointCart& t_2
) {
    predicate_count++;
    Segment_2 seg_1 = Segment_2(
        s_1.get_cgal_point(), t_1.get_cgal_point()
    );

    Segment_2 seg_2 = Segment_2(
        s_2.get_cgal_point(), t_2.get_cgal_point()
    );

    auto ip = CGAL::intersection(seg_1, seg_2);

    if (!ip)
        assert(false);

    Point_2 i_point = boost::get<Point_2>(*ip);

    return PointCart(CGAL::to_double(i_point.x()), CGAL::to_double(i_point.y()));
}

long long PointCart::predicate_count = 0;

int PointCart::points_created = 0;