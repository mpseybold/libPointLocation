#include "TSD.h"

#define VERBOSITY_LEVEL 100


// Returns the next priority descendants of 'node'
// whose regions intersect the segment.
// The nodes are ordered in the output vector in left-to-right
// stabbing order. 
template <class PointType, class OrderType>
void TSD<PointType, OrderType>::search_refinement(Segment<PointType, OrderType>* seg, Node<PointType, OrderType>* node) {
    
    intersecting_descendants = { nullptr, nullptr, nullptr, nullptr };
    
    if (node->get_L() != nullptr) 
        if (node->get_L()->get_trapezoid().intersects_segment(seg)) {
            auto trap = node->get_L()->get_trapezoid();
            auto val = trap.intersects_segment(seg);
            intersecting_descendants[0] = node->get_L();
        }
    
    if (node->get_B() != nullptr) {
        assert(node->get_A() != nullptr);
        Cut<PointType, OrderType> edge_cut = node
        ->get_destruction_cuts()
        .get_e();

        assert(edge_cut.get_cut_type() == EDGE);

        auto slope_comp = Segment<PointType, OrderType>::slope_comparison(
            *seg, *edge_cut.get_segment()
        );

        if (Segment<PointType, OrderType>::slope_comparison(
            *seg, *edge_cut.get_segment()
        ) > 0) {
            if (node->get_A()->get_trapezoid().intersects_segment(seg))
                intersecting_descendants[1] = node->get_A();
            // If B intersects the region adjacent to the right of
            // nodes' trapezoid, we don't check for the intersection
            // here to ensure the nodes are reported in the
            // correct order.
            if (node->get_A()->get_trapezoid().get_right()
            .defining_point_cut_comparison(
                node->get_B()->get_trapezoid().get_right()
            ) > -1)
                if (node->get_B()->get_trapezoid().intersects_segment(seg))
                    intersecting_descendants[2] = node->get_B();
        } else {
            if (node->get_B()->get_trapezoid().intersects_segment(seg))
                intersecting_descendants[1] = node->get_B();
            // If A intersects the region to the right of
            // nodes' trapezoid, we don't check for the intersection
            // here to ensure the nodes are reported in the
            // correct order.
            if (node->get_A()->get_trapezoid().get_right()
            .defining_point_cut_comparison(
                node->get_B()->get_trapezoid().get_right()
            ) < 1)
                if (node->get_A()->get_trapezoid().intersects_segment(seg))
                    intersecting_descendants[2] = node->get_A();
        }
    }

    if (node->get_R() != nullptr)
        if (node->get_R()->get_trapezoid().intersects_segment(seg))
            intersecting_descendants[3] = node->get_R();
}


template <class PointType, class OrderType>
TSD<PointType, OrderType>::TSD() {
    BoundingTrap<PointType, OrderType> trapezoid = BoundingTrap<PointType, OrderType>();
    trapezoid.set_top(
        Cut<PointType, OrderType>(
            EDGE, new Segment<PointType, OrderType>(
                PointCart(-10, 100), PointCart(110, 100), -1
            ), nullptr
        )
    );
    trapezoid.set_bottom(
        Cut<PointType, OrderType>(
            EDGE, new Segment<PointType, OrderType>(
                PointCart(-20, 0), PointCart(120, 0), -1
            ), nullptr
        )
    );
    trapezoid.set_left(
        Cut<PointType, OrderType>(
            TARGET, new Segment<PointType, OrderType>(
                PointCart(-30, 50), PointCart(0, 50), -1
            ), nullptr
        )
    );
    trapezoid.set_right(
        Cut<PointType, OrderType>(
            SOURCE, new Segment<PointType, OrderType>(
                PointCart(100, 50), PointCart(150, 50), -1
            ), nullptr
        )
    );
    root = new Node<PointType, OrderType>(trapezoid);
    segments = std::vector<Segment<PointType, OrderType>>();
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::affected_subdag_roots(Segment<PointType, OrderType>* seg) {

    int num_visited = 0;
    int num_subdag_roots = 0;

    visited_nodes = std::vector<Node<PointType, OrderType>*>();
    subdag_roots = std::vector<Node<PointType, OrderType>*>();
    
    // nodes are stored in left-to-right stabbing order on the stack
    search_stack = std::stack<Node<PointType, OrderType>*>();
    search_stack.push(root);

    while (!search_stack.empty()) {
        
        num_visited++;

        Node<PointType, OrderType>* top = search_stack.top();
        while(top->is_visited()) {
            search_stack.pop();
            top = search_stack.top();
            assert(top != nullptr);
        }
        search_stack.pop();

        if (top->is_leaf()) {
            top->toggle_visited();
            num_subdag_roots++;
            subdag_roots.push_back(top);
            visited_nodes.push_back(top);
            continue;
        }

        search_refinement(seg, top);

        for (int i = 0; i < intersecting_descendants.size(); ++i) {
            Node<PointType, OrderType>* node = intersecting_descendants[i];
            if (node != nullptr && !node->is_visited()) {
                if (node->get_priority() > seg->get_priority()) {
                    subdag_roots.push_back(node);
                    node->toggle_visited();
                    visited_nodes.push_back(node);
                    num_subdag_roots++;
                    intersecting_descendants[i] = nullptr;
                }
            } else {
                if (node != nullptr) {
                    std::cout << "node seen twice\n";
                    intersecting_descendants[i] = nullptr;
                }
            }
    
        }

        for (int i = intersecting_descendants.size()-1; i >= 0; --i) {
            Node<PointType, OrderType>* node = intersecting_descendants[i];
            if (node != nullptr)
                search_stack.push(node);
        }
    }

    for (auto node: visited_nodes) {
        assert(node != nullptr);
        node->toggle_visited();
    }

    if (VERBOSITY_LEVEL >= 100) {
        std::cout << "number of nodes visited:\t" << num_visited << "\n";
        std::cout << "number of subdag roots:\t" << num_subdag_roots << "\n";
    }
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::insert_segment(Segment<PointType, OrderType> seg) {
   //TODO: implement this properly/test the code
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::delete_segment(Segment<PointType, OrderType> seg) {
    //TODO: implement this properly/test the code
} 

// template class TSD<PointCart, int>;