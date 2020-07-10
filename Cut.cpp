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

template class Cut<PointCart, int>;