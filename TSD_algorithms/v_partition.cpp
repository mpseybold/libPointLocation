#include "../TSD.h"

namespace {

    template<class PointType, class OrderType>
    void handle_VE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType> v_cut) {
        //TODO: implement this
    }

    template<class PointType, class OrderType>
    void handle_VVE_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType> v_cut) {
        //TODO: implement this
    }
}

template<class PointType, class OrderType>
    void TSD<PointType, OrderType>::v_part_handle_E_case(Node<PointType, OrderType>* node, Cut<PointType, OrderType> v_cut) {
        //TODO: implement this

        Cut<PointType, OrderType> edge_cut = node->get_cut();
        BoundingTrap<PointType, OrderType> trap = node->get_trapezoid();
        BoundingTrap<PointType, OrderType> A_trap = node->get_A()->get_trapezoid();
        BoundingTrap<PointType, OrderType> B_trap = node->get_B()->get_trapezoid();
        int def_point_orientation = v_cut.defining_point_cut_comparrison(edge_cut);

        switch(def_point_orientation) {
            case 1:
                std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> 
                pos_neg = trap.destroy(v_cut);

                BoundingTrap<PointType, OrderType> pos_trap = pos_neg.first;
                BoundingTrap<PointType, OrderType> neg_trap = pos_neg.second;
                std::pair<BoundingTrap<PointType, OrderType>, BoundingTrap<PointType, OrderType>> 
                pos_pos_neg = A_trap.destroy(v_cut);
                BoundingTrap<PointType, OrderType> pos_pos_trap = pos_pos_neg.first;
                BoundingTrap<PointType, OrderType> pos_neg_trap = pos_pos_neg.second;


                Node<PointType, OrderType>* pos_child = new Node<PointType, OrderType>(edge_cut, pos_trap);
                Node<PointType, OrderType>* neg_child = new Node<PointType, OrderType>(edge_cut, neg_trap);
                v_partition(node->get_A(), v_cut);
                Node<PointType, OrderType>* pos_pos_child = node->get_A()->get_positive_child();
                Node<PointType, OrderType>* neg_pos_child = node->get_A()->get_negative_child();
                Node<PointType, OrderType>* pos_neg_child = node->get_B();
                Node<PointType, OrderType>* neg_neg_child = node->get_B();

                delete node->get_positive_child();

                node->set_positive_child(pos_child);
                node->set_negative_child(neg_child);
                pos_child->set_positive_child(pos_pos_child);
                pos_child->set_negative_child(pos_neg_child);
                neg_child->set_positive_child(neg_pos_child);
                neg_child->set_negative_child(neg_neg_child);

                node->set_L(pos_child);
                node->set_A(neg_pos_child);
                node->set_B(neg_neg_child);
                node->set_R(nullptr);

                break;
        }
    }


template <class PointType, class OrderType>
void TSD<PointType, OrderType>::v_partition(Node<PointType, OrderType>* node, Cut<PointType, OrderType> v_cut) {
    //TODO: Implement this function.
    DestructionPattern pattern = node->get_dest_pattern();

    switch(pattern) {
        case E:
             (node, v_cut);
            break;
        case VE:
            handle_VE_case<PointType, OrderType>(node, v_cut);
            break;
        case VVE:
            handle_VVE_case<PointType, OrderType>(node, v_cut);
            break;
    }
}

template class TSD<PointCart, int>;