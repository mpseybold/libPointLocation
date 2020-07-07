#include <iostream>
#include <CGAL/Point_2.h>
// #include <CGAL/Exact_predicates_exact_constructions_kernel.h>
// #include <CGAL/Line_2.h>
#include "Cut.h"
#include "geom.h"


// using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
// using Point = CGAL::Point_2<Kernel>;

int main() {
    
    PointCart a = PointCart(0, 0);
    PointCart b = PointCart(1, 1);

    Segment<PointCart> segment = Segment<PointCart>(a, b);

    // Line_2 line = Line_2(a, b);

    Cut<PointCart, LineCart> cut = Cut<PointCart, LineCart>(EDGE, &segment, nullptr);

}
