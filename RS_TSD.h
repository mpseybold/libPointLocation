#pragma once

#include "TSD.h"

class RS_TSD {

    private:

        TSD<PointCart, int>* tsd;

    public:

        RS_TSD(std::vector<std::vector<long double>> segments, Segment<PointCart, int>* query);

        // std::vector<std::vector<long double>> query(std::vector<double> q);
        std::vector<std::vector<long double>> query(Segment<PointCart, int>* query);

        std::vector<std::vector<long double>> intersections;

        int query_time;
};