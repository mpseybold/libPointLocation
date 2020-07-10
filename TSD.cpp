#include "TSD.h"
#include <stack>

namespace {
    template <class PointType, class OrderType>
    std::pair<Node<PointType, OrderType>*, Node<PointType, OrderType>*> 
    search_refinement(Node<PointType, OrderType>* node, std::set<Node<PointType, OrderType>*>& visited) {
        return {nullptr, nullptr};
    }
}

template <class PointType, class OrderType>
TSD<PointType, OrderType>::TSD() {
    root = new Node<PointType, OrderType>(Cut<PointType, OrderType>(), BoundingTrap<PointType, OrderType>());
    segments = std::vector<Segment<PointType, OrderType>>();
    //TODO: Implement this
}

template <class PointType, class OrderType>
Node<PointType, OrderType>* TSD<PointType, OrderType>::deepest_node_containing_segment(Segment<PointType, OrderType>* seg) {
    //TODO: Implement this properly/test the code

    Node<PointType, OrderType>* node = root;

    while ((node->get_positive_child()->contains(seg) && node->get_positive_child()->get_priority() < seg->get_priority()) 
    || (node->get_negative_child()->contains(seg) && node->get_negative_child()->get_priority() < seg->get_priority())) {
            
        if (node->get_positive_child()->contains(seg) && node->get_positive_child()->get_priority() < seg->get_priority()) {
            node = node->get_positive_child();
        } else if (node->get_negative_child()->contains(seg) && node->get_negative_child()->get_priority() < seg->get_priority()) {
            node = node->get_negative_child();
        }
    }

    return node;
}

template <class PointType, class OrderType>
std::vector<Node<PointType, OrderType>*> TSD<PointType, OrderType>::affected_subdag_roots(Segment<PointType, OrderType>* seg) {
    //TODO: Implement this properly/test the code

    std::set<Node<PointType, OrderType>*> visited = std::set<Node<PointType, OrderType>*>();
    std::vector<Node<PointType, OrderType>*> output = std::vector<Node<PointType, OrderType>*>();
    std::stack<Node<PointType, OrderType>*> stack = std::stack<Node<PointType, OrderType>*>();

    Node<PointType, OrderType>* top = root;
    std::pair<Node<PointType, OrderType>*, Node<PointType, OrderType>*> next_pair = search_refinement(top, visited);

    if (next_pair.first == nullptr) {
        output.push_back(top);
    } else {
        if (next_pair.second != nullptr) {
            stack.push(next_pair.second);
        }
        stack.push(next_pair.first);
    }

    while (stack.empty()) {
        top = stack.top();
        stack.pop();
        next_pair = search_refinement(top, visited);
        if (next_pair.first == nullptr) {
        output.push_back(top);
        } else {
            if (next_pair.second != nullptr) {
                stack.push(next_pair.second);
            }
            stack.push(next_pair.first);
        }
    }

    return output;
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::insert_segment(Segment<PointType, OrderType> seg) {
   //TODO: implement this properly/test the code
}

template <class PointType, class OrderType>
void TSD<PointType, OrderType>::delete_segment(Segment<PointType, OrderType> seg) {
    //TODO: implement this properly/test the code
} 

template class TSD<PointCart, int>;