#include "BoundingTrap.h"

template <class PointType, class OrderType>
std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>
BoundingTrap<PointType, OrderType>::destroy(Cut<PointType, OrderType> cut) {

    switch(type) {
        case BRTL:
            break;
        case BRT:
            break;
        case BTL:
            break;
    }

    return std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>>();
}

template class BoundingTrap<PointCart, int>;