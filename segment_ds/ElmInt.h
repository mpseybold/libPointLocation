#pragma once

#include "Segment.h"

enum INTERVAL_TYPE { CO, OC, OO, CC };
namespace SegmentDS {
    class ElmInt {

        private:
            double left;
            double right;
            INTERVAL_TYPE type;

        public:

            ElmInt() {
                left = 0;
                right = 0;
                type = CC;
            }

            ElmInt(double l, double r, INTERVAL_TYPE t) {
                left = l;
                right = r;
                type = t;
            }

            double get_left() { return left; }
            double get_right() { return right; }
            INTERVAL_TYPE get_type() { return type; }


            bool operator>(ElmInt other){
                if (left >= other.get_right())
                    return true;
            }

            bool is_covered(Segment seg) {
                return seg.x1() <= left && seg.x2() >= right;
            }

            bool intersects(Segment seg) {
                return is_covered(seg) || ((seg.x1() >= left && seg.x1() <= right)
                || (seg.x2() >= left && seg.x2() <= right));
            }

            bool contains(double qx) {
                switch (type) {
                    case CC: {
                        return qx >= left && qx <= right;
                        break;
                    }
                    case OC: {
                        return qx > left && qx <= right;
                        break;
                    }
                    case CO: {
                        return qx >= left && qx < right;
                        break;
                    }
                    case OO: {
                        return qx > left && qx < right;
                        break;
                    }
                }
            }


            ElmInt const operator+(ElmInt& other) {
                if (!(other > *this))
                    throw std::logic_error(
                        "the left operand should be left of the right operand"
                    );
                
                INTERVAL_TYPE new_type;

                auto o_type = other.get_type();

                if (type == OO || type == OC) {
                    if (o_type == OC || o_type == CC) {
                        new_type = OC;
                    } else {
                        new_type = OO;
                    }
                }
                else if (type == CO || type == CC) {
                    if (o_type == CO || o_type == OO) {
                        new_type = CO;
                    } else {
                        new_type = CC;
                    }
                }

                return ElmInt(left, other.get_right(), new_type);
            }
    };
}