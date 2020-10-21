#include "Segment.h"

template <class PointType, class OrderType>
bool Segment<PointType, OrderType>::operator==(Segment<PointType, OrderType>& other_seg) {
    return PointType::orientV(source, other_seg.get_source()) == 0 &&
    PointType::orientV(target, other_seg.get_target()) == 0;
}

template class Segment<PointCart, int>;
// template class Segment<PointS2ratss, int>;
