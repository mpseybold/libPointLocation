#include "Cut.h"

template <class PointType, class OrderType>
int Cut<PointType, OrderType>::orientation(PointType p) {
    
    if (cut_type == SOURCE) {
        return PointType::v_orientation(
            segment->get_source(), p
        );
    }

    if (cut_type == TARGET) {
        return PointType::v_orientation(
            segment->get_target(), p
        );
    }

    if (cut_type == EDGE) {
        return PointType::line_orientation(
            segment->get_source(), segment->get_target(), p
        );
    }

    if (cut_type == INTERSECTION) {
        return PointType::intersection_v_orientation(
            segment->get_source(), segment->get_target(),
            intersecting_seg->get_source(), intersecting_seg->get_target(),
            p
        );
    }


    throw std::logic_error("failed to compare point with cut");
}

template <class PointType, class OrderType>
bool Cut<PointType, OrderType>::operator== (Cut<PointType, OrderType>& other_cut) {
    
    if (cut_type == NO_CUT && other_cut.get_cut_type() == NO_CUT)
        return true;

    if (cut_type != other_cut.get_cut_type())
        return false;

    bool are_equal = true;

    are_equal &= *segment == *other_cut.get_segment();

    if (cut_type == INTERSECTION)
        are_equal &= *intersecting_seg == *other_cut.get_intersecting_seg();

    return are_equal;
}

template <class PointType, class OrderType>
int Cut<PointType, OrderType>::defining_point_cut_comparison(
    Cut<PointType, OrderType> other_cut) {
    
    assert(cut_type != EDGE);

    if (cut_type == SOURCE)
        return other_cut.orientation(segment->get_source());

    if (cut_type == TARGET)
        return other_cut.orientation(segment->get_target());

    switch(other_cut.get_cut_type()) {
        case SOURCE: {
            return -PointType::intersection_v_orientation(
                segment->get_source(), segment->get_target(),
                intersecting_seg->get_source(), intersecting_seg->get_target(),
                other_cut.get_segment()->get_source()
            );
            break;
        }
        case TARGET: {
            return -PointType::intersection_v_orientation(
                segment->get_source(), segment->get_target(),
                intersecting_seg->get_source(), intersecting_seg->get_target(),
                other_cut.get_segment()->get_target()
            );
            break;
        }
        case INTERSECTION: {
            return PointType::intersection_v_orientation(
                other_cut.get_segment()->get_source(), other_cut.get_segment()->get_target(),
                other_cut.get_intersecting_seg()->get_source(), other_cut.get_intersecting_seg()->get_target(),
                segment->get_source(), segment->get_target(),
                intersecting_seg->get_source(), intersecting_seg->get_target()
            );
            break;
        }
    }
}

template <class PointType, class OrderType>
bool Cut<PointType, OrderType>::has_seg_on_pos_side(Segment<PointType, OrderType>* seg) {

    if (cut_type == EDGE && has_on(seg)) {
        return Segment<PointType, OrderType>::lexicographic_comparison(*get_segment(), *seg) == 1;
    }

    return orientation(seg->get_source()) == 1 || orientation(seg->get_target()) == 1;
};

template <class PointType, class OrderType>
bool Cut<PointType, OrderType>::has_seg_on_neg_side(Segment<PointType, OrderType>* seg) {

    if (cut_type == EDGE && has_on(seg)) {
        return Segment<PointType, OrderType>::lexicographic_comparison(*get_segment(), *seg) == -1;
    }

    return orientation(seg->get_source()) == -1 || orientation(seg->get_target()) == -1;
}

template <class PointType, class OrderType>
bool Cut<PointType, OrderType>::has_on(Segment<PointType, OrderType>* seg) {
    assert(cut_type == EDGE);
    return orientation(seg->get_source()) == 0 && orientation(seg->get_target()) == 0;
}

template <class PointType, class OrderType>
int Cut<PointType, OrderType>::v_cut_edge_orientation(Cut<PointType, OrderType>& v_cut, Cut<PointType, OrderType>& e_cut) {
    int def_point_orientation = v_cut.defining_point_cut_comparison(e_cut);
    if (v_cut.get_cut_type() != INTERSECTION && def_point_orientation == 0) {
        if (v_cut.get_cut_type() == TARGET) {
            def_point_orientation = e_cut.orientation(v_cut.get_segment(), 1);
        } else {
            assert(v_cut.get_cut_type() == SOURCE);
            def_point_orientation = e_cut.orientation(v_cut.get_segment(), 0);
        }
    }

    return def_point_orientation;
}

template class Cut<PointCart, int>;