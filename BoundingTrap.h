#include "Cut.h"

enum TrapType { BRTL, BTL, BRT, NONE };

template <class PointType, class OrderType>
class BoundingTrap {

    private:
    
        std::vector<Cut<PointType, OrderType>> cuts;
        TrapType type;

    public:

        BoundingTrap(
            Cut<PointType, OrderType> c_1,
            Cut<PointType, OrderType> c_2,
            Cut<PointType OrderType> c_3,
            Cut<PointType, OrderType> c_4
        ) { 
            cuts = {c_1, c_2, c_3, c_4};
            type = BRTL; 
        }

        BoundingTrap() { 
            cuts = std::vector<Cut<PointType, OrderType>>();
            type = NONE;
        };

        std::pair<BoundingTrap<PointType, OrderType>> destroy(Cut<PointType, OrderType>);

        BoundingTrap<PointType, OrderType> merge(BoundingTrap<PointType, OrderType>);
}