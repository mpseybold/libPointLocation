#include "Segment.h"

template <class PointType, class OrderType>
bool Segment<PointType, OrderType>::operator==(Segment<PointType, OrderType>& other_seg) {
    return PointType::v_orientation(source, other_seg.get_source()) == 0 &&
    PointType::v_orientation(target, other_seg.get_target()) == 0;
}

// template class Segment<PointCart, int>;
