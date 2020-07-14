template <class PointType, class OrderType>
class Segment {

    private:
    
        PointType source;
        PointType target;
        OrderType priority;
    
    public:

        Segment(PointType s, PointType t, OrderType pri) : source(s), target(t), priority(pri) {
            if (PointType::v_orientation(s, t) == -1) {
                source = s;
                target = t;
            } else {
                source = t;
                target = s;
            }
        }

        PointType get_source() { return source; }
        PointType get_target() { return target; }
        OrderType get_priority() { return priority; }
};
