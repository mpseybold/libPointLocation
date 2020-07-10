#include "TSD.h"

template <class PointType, class OrderType>
TSD<PointType, OrderType>::TSD() {
    root = new Node<PointType, OrderType>(Cut<PointType, OrderType>(), BoundingTrap<PointType, OrderType>());
}

template class TSD<PointCart, int>;