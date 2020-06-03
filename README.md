# libpointlocation
Point location for fully-dynamic line segments in the plane R^2 and great circle segments on the sphere S^2

* DynTSD<Point_Type, Order_Type> class provides an implementation of the fully-dynamic Trapezoidal Search Dag based on the orientation predicates of Point_Type and the randomness strategy Order_Type
* Point_R2
* Point_S2 
* Segment<Point_Type>: Static, geometric orientation predicates left/on/right and below/on/right for point/point point/segment where points may be represented explicitly or implicitly as intersection of two segments.
* Order_Simple: Pick random uint64_t as random priority for new segments and resolve (, the highly unlikely,) collitions of the values of two segments with the segment_index.
* Order_Treap: Treap to maintain small codes for elements in a total order.


Roadmap:
TBD
