#pragma once 

namespace SegmentDS {
    class Segment {
        private:
            double _x1;
            double _y1;
            double _x2;
            double _y2;

            

        public:
            
            bool real;

            Segment(double x1, double y1,
            double x2, double y2) {
                _x1 = x1;
                _y1 = y1;
                _x2 = x2;
                _y2 = y2;

                if (_x1 > _x2) {
                    _x1 = x2;
                    _x2 = x1;
                }

                // if (_y1 > _y2) {
                //     _y1 = y2;
                //     _y2 = y1;
                // }

                real = true;
            }

            Segment() {
                _x1 = 0;
                _x2 = 0;
                _y1 = 0;
                _y2 = 0;

                real = false;
            }

            void swap() {
                double aux = _y1;
                _y1 = _y2;
                _y2 = aux;
            }

            double x1() { return _x1; }
            double y1() { return _y1; }
            double x2() { return _x2; }
            double y2() { return _y2; }
    };
}