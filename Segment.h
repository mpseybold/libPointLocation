#pragma once

#include "Point_2D.h"
#include "Point_S2ratss.h"

template <class PointType, class OrderType>
class Segment {

    private:
    
        PointType source;
        PointType target;
        OrderType priority;

        double source_x, source_y, target_x, target_y;
    
    public:

        Segment(PointType s, PointType t, OrderType pri) : source(s), target(t), priority(pri) {
            if (PointType::orientV(s, t) == 1) {
                source = s;
                target = t;
            } else {
                source = t;
                target = s;
            }

            source_x = CGAL::to_double(source.get_cgal_point().x());
            source_y = CGAL::to_double(source.get_cgal_point().y());
            target_x = CGAL::to_double(target.get_cgal_point().x());
            target_y = CGAL::to_double(target.get_cgal_point().y());
            // std::cout << CGAL::to_double(s.get_cgal_point().x()) << " " << CGAL::to_double(s.get_cgal_point().y()) << "\n";
        }

        PointType& get_source() { return source; }
        PointType& get_target() { return target; }
        OrderType get_priority() { return priority; }

        static int slope_comparison(
            Segment<PointType, OrderType> seg, Segment<PointType, OrderType> other_seg
        ) { 
            return PointType::slope_comparison(
                seg.get_source(), seg.get_target(),
                other_seg.get_source(), other_seg.get_target()
            ); 
        }

        bool operator== (Segment<PointType, OrderType>& other_seg);

        // returns -1 if other_seg is lexicographically smaller than seg
        // returns 1 otherwise
        static int lexicographic_comparison(Segment<PointType, OrderType> seg, Segment<PointType, OrderType> other_seg) {
            return -1;
        }

        bool operator< (Segment<PointType, OrderType>& other) {
            return (long)this < (long)&other; 
        }
};

#include "Segment.cpp"
