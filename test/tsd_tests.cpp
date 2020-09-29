#include "gtest/gtest.h"
#include "../TSD.h"
#include "../io.h"
#include <random>


TEST(TSDTests, searchTest) {
    // TSD<PointCart, int> tsd = TSD<PointCart, int>();
    // Node<PointCart, int>* root = tsd.get_root();

    // tsd.affected_subdag_roots(new Segment<PointCart, int>(
    //     PointCart(1, 1), PointCart(99, 99), 1
    // ));
    
    // ASSERT_TRUE(root == tsd.get_subdag_roots()[0]);

    // //see figures/figure_1.png

    // Segment<PointCart, int> seg_1 = Segment<PointCart, int>(
    //     PointCart(30, 50), PointCart(70, 50), 0
    // );


    // Segment<PointCart, int> seg_2 = Segment<PointCart, int>(
    //     PointCart(40, 25), PointCart(80, 25), 1
    // );


    // Cut<PointCart, int> source_seg_1 = Cut<PointCart, int>(
    //     SOURCE, &seg_1, nullptr
    // );
    // Cut<PointCart, int> target_seg_1 = Cut<PointCart, int>(
    //     TARGET, &seg_1, nullptr
    // );
    // Cut<PointCart, int> edge_seg_1 = Cut<PointCart, int>(
    //     EDGE, &seg_1, nullptr
    // );
    // Cut<PointCart, int> source_seg_2 = Cut<PointCart, int>(
    //     SOURCE, &seg_2, nullptr
    // );
    // Cut<PointCart, int> target_seg_2 = Cut<PointCart, int>(
    //     TARGET, &seg_2, nullptr
    // );
    // Cut<PointCart, int> edge_seg_2 = Cut<PointCart, int>(
    //     EDGE, &seg_2, nullptr
    // );

    // auto trap = root->get_trapezoid();

    // Node<PointCart, int>* L = new Node<PointCart, int>(
    //     BoundingTrap<PointCart, int>(
    //         trap.get_bottom(), source_seg_1,
    //         trap.get_top(), trap.get_left()
    //     )
    // );
    // L->name = "L";
    // Node<PointCart, int>* A = new Node<PointCart, int>(
    //     BoundingTrap<PointCart, int>(
    //         edge_seg_1, target_seg_1,
    //         trap.get_top(), source_seg_1
    //     )
    // );
    // A->name = "A";
    // Node<PointCart, int>* B = new Node<PointCart, int>(
    //     BoundingTrap<PointCart, int>(
    //         trap.get_bottom(), target_seg_1,
    //         edge_seg_1, source_seg_1
    //     )
    // );
    // B->name = "B";

    // Node<PointCart, int>* R = new Node<PointCart, int>(
    //     BoundingTrap<PointCart, int>(
    //         trap.get_bottom(), trap.get_right(),
    //         trap.get_top(), target_seg_1
    //     )
    // );
    // R->name = "R";
    // Node<PointCart, int>* BL = new Node<PointCart, int>(
    //     BoundingTrap<PointCart, int>(
    //         trap.get_bottom(), source_seg_2,
    //         edge_seg_1, source_seg_1
    //     )
    // );
    // BL->name = "BL";
    // Node<PointCart, int>* BA = new Node<PointCart, int>(
    //     BoundingTrap<PointCart, int>(
    //         edge_seg_2, target_seg_1,
    //         edge_seg_1, source_seg_2
    //     )
    // );
    // BA->name = "BA";
    // Node<PointCart, int>* BB = new Node<PointCart, int>(
    //     BoundingTrap<PointCart, int>(
    //         trap.get_bottom(), target_seg_2,
    //         edge_seg_2, source_seg_2
    //     )
    // );
    // BB->name = "BB";
    // Node<PointCart, int>* RR = new Node<PointCart, int>(
    //     BoundingTrap<PointCart, int>(
    //         trap.get_bottom(), trap.get_right(),
    //         trap.get_top(), target_seg_2
    //     )
    // );
    // RR->name = "RR";
    // Node<PointCart, int>* RA = new Node<PointCart, int>(
    //     BoundingTrap<PointCart, int>(
    //         edge_seg_2, target_seg_2,
    //         trap.get_top(), target_seg_1
    //     )
    // );
    // RA->name = "RA";
    // Node<PointCart, int>* RB = BB;

    // root->set_L(L);
    // root->set_R(R);
    // root->set_A(A);
    // root->set_B(B);
    // root->name = "root";
    // B->set_L(BL);
    // B->set_A(BA);
    // B->set_B(BB);
    // R->set_R(RR);
    // R->set_A(RA);
    // R->set_B(RB);


    // root->get_destruction_cuts().set_v_1(source_seg_1);
    // root->get_destruction_cuts().set_v_2(target_seg_1);
    // root->get_destruction_cuts().set_e(edge_seg_1);
    // B->get_destruction_cuts().set_v_1(source_seg_2);
    // B->get_destruction_cuts().set_e(edge_seg_2);
    // R->get_destruction_cuts().set_v_2(target_seg_2);
    // R->get_destruction_cuts().set_e(edge_seg_2);

    // std::vector<BoundingTrap<PointCart, int>> leaves = {
    //     L->get_trapezoid(), A->get_trapezoid(), 
    //     BL->get_trapezoid(), BA->get_trapezoid(), 
    //     BB->get_trapezoid(), RA->get_trapezoid(), 
    //     RR->get_trapezoid()
    // };
    // io::write_trapezoids(leaves, "plotting/leaves.dat");

    // Segment<PointCart, int> query_seg_1 = Segment<PointCart, int>(
    //     PointCart(10, 5), PointCart(90, 5), 2
    // );

    // tsd.affected_subdag_roots(&query_seg_1);
    // auto roots = tsd.get_subdag_roots();
    // ASSERT_EQ(roots.size(), 4);
    // ASSERT_EQ(roots[0]->name, "L");
    // ASSERT_EQ(roots[1]->name, "BL");
    // ASSERT_EQ(roots[2]->name, "BB");
    // ASSERT_EQ(roots[3]->name, "RR");

    // Segment<PointCart, int> query_seg_2 = Segment<PointCart, int>(
    //     PointCart(39, 1), PointCart(89, 42.66666666666), 2
    // );
    // tsd.affected_subdag_roots(&query_seg_2);
    // roots = tsd.get_subdag_roots();
    // ASSERT_EQ(roots.size(), 5);
    // ASSERT_EQ(roots[0]->name, "BL");
    // ASSERT_EQ(roots[1]->name, "BB");
    // ASSERT_EQ(roots[2]->name, "BA");
    // ASSERT_EQ(roots[3]->name, "RA");
    // ASSERT_EQ(roots[4]->name, "RR");

    // Segment<PointCart, int> query_seg_3 = Segment<PointCart, int>(
    //     PointCart(0, 100), PointCart(100, 0), 2
    // );
    // tsd.affected_subdag_roots(&query_seg_3);
    // roots = tsd.get_subdag_roots();
    // ASSERT_EQ(roots.size(), 6);
    // ASSERT_EQ(roots[0]->name, "L");
    // ASSERT_EQ(roots[1]->name, "A");
    // ASSERT_EQ(roots[2]->name, "BA");
    // ASSERT_EQ(roots[3]->name, "RA");
    // ASSERT_EQ(roots[4]->name, "BB");
    // ASSERT_EQ(roots[5]->name, "RR");

    // Segment<PointCart, int> query_seg_4 = Segment<PointCart, int>(
    //     PointCart(29, 1), PointCart(81, 99), 2
    // );
    // tsd.affected_subdag_roots(&query_seg_4);
    // roots = tsd.get_subdag_roots();
    // ASSERT_EQ(roots.size(), 7);
    // ASSERT_EQ(roots[0]->name, "L");
    // ASSERT_EQ(roots[1]->name, "BL");
    // ASSERT_EQ(roots[2]->name, "BB");
    // ASSERT_EQ(roots[3]->name, "BA");
    // ASSERT_EQ(roots[4]->name, "A");
    // ASSERT_EQ(roots[5]->name, "RA");
    // ASSERT_EQ(roots[6]->name, "RR");
}

TEST(TSDTests, vPartTest) {
    TSD<PointCart, int> tsd = TSD<PointCart, int>();
    Node<PointCart, int>* root = tsd.get_root();

    Segment<PointCart, int> seg_1 = Segment<PointCart, int>(
    PointCart(30, 50), PointCart(70, 50), 0
    );

    Segment<PointCart, int> seg_2 = Segment<PointCart, int>(
        PointCart(40, 25), PointCart(80, 25), 1
    );

    Cut<PointCart, int> source_seg_1 = Cut<PointCart, int>(
        SOURCE, &seg_1, nullptr
    );
    Cut<PointCart, int> target_seg_1 = Cut<PointCart, int>(
        TARGET, &seg_1, nullptr
    );
    Cut<PointCart, int> edge_seg_1 = Cut<PointCart, int>(
        EDGE, &seg_1, nullptr
    );
    Cut<PointCart, int> source_seg_2 = Cut<PointCart, int>(
        SOURCE, &seg_2, nullptr
    );
    Cut<PointCart, int> target_seg_2 = Cut<PointCart, int>(
        TARGET, &seg_2, nullptr
    );
    Cut<PointCart, int> edge_seg_2 = Cut<PointCart, int>(
        EDGE, &seg_2, nullptr
    );

    auto trap = root->get_trapezoid();

    Node<PointCart, int>* L = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            trap.get_bottom(), source_seg_1,
            trap.get_top(), trap.get_left()
        )
    );
    L->name = "L";
    Node<PointCart, int>* A = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            edge_seg_1, target_seg_1,
            trap.get_top(), source_seg_1
        )
    );
    A->name = "A";
    Node<PointCart, int>* B = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            trap.get_bottom(), target_seg_1,
            edge_seg_1, source_seg_1
        )
    );
    B->name = "B";

    Node<PointCart, int>* R = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            trap.get_bottom(), trap.get_right(),
            trap.get_top(), target_seg_1
        )
    );
    R->name = "R";
    Node<PointCart, int>* BL = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            trap.get_bottom(), source_seg_2,
            edge_seg_1, source_seg_1
        )
    );
    BL->name = "BL";
    Node<PointCart, int>* BA = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            edge_seg_2, target_seg_1,
            edge_seg_1, source_seg_2
        )
    );
    BA->name = "BA";
    Node<PointCart, int>* BB = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            trap.get_bottom(), target_seg_2,
            edge_seg_2, source_seg_2
        )
    );
    BB->name = "BB";
    Node<PointCart, int>* RR = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            trap.get_bottom(), trap.get_right(),
            trap.get_top(), target_seg_2
        )
    );
    RR->name = "RR";
    Node<PointCart, int>* RA = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            edge_seg_2, target_seg_2,
            trap.get_top(), target_seg_1
        )
    );
    RA->name = "RA";
    Node<PointCart, int>* RB = BB;

    root->set_L(L);
    root->set_R(R);
    root->set_A(A);
    root->set_B(B);
    root->name = "root";
    B->set_L(BL);
    B->set_A(BA);
    B->set_B(BB);
    R->set_R(RR);
    R->set_A(RA);
    R->set_B(RB);


    root->get_destruction_cuts().set_v_1(source_seg_1);
    root->get_destruction_cuts().set_v_2(target_seg_1);
    root->get_destruction_cuts().set_e(edge_seg_1);
    B->get_destruction_cuts().set_v_1(source_seg_2);
    B->get_destruction_cuts().set_e(edge_seg_2);
    R->get_destruction_cuts().set_v_2(target_seg_2);
    R->get_destruction_cuts().set_e(edge_seg_2);

    TSD<PointCart, int> tsd_2 = TSD<PointCart, int>();
    auto root_2 = tsd_2.get_root();
    auto trap_2 = root_2->get_trapezoid();
    Segment<PointCart, int> big_seg = Segment<PointCart, int>(
        PointCart(-10, 50), PointCart(110, 50), 1
    );
    auto big_edge_cut = Cut<PointCart, int>(
        EDGE, &big_seg, nullptr
    );

    auto A_2 = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            big_edge_cut, trap_2.get_right(),
            trap_2.get_top(), trap_2.get_left()
        )
    );
    auto B_2 = new Node<PointCart, int>(
        BoundingTrap<PointCart, int>(
            trap_2.get_bottom(), trap_2.get_right(),
            big_edge_cut, trap_2.get_left()
        )
    );
    root_2->set_e(big_edge_cut);
    root_2->set_A(A_2);
    root_2->set_B(B_2);



    Segment<PointCart, int> v_part_seg_1 = Segment<PointCart, int>(
        PointCart(0, 60), PointCart(50, 60), 0
    );
    Cut<PointCart, int> v_cut_1 = Cut<PointCart, int>(
        TARGET, &v_part_seg_1, nullptr
    );


    tsd_2.v_partition(root_2, v_cut_1);
    
    ASSERT_TRUE(root_2->get_A() == nullptr);
    ASSERT_TRUE(root_2->get_B() == nullptr);
    ASSERT_TRUE(root_2->get_L() != nullptr);
    ASSERT_TRUE(root_2->get_R() != nullptr);
}

void write_leaf_traps(Node<PointCart, int>* node, std::vector<BoundingTrap<PointCart, int>>& traps) {
    if (node->is_leaf()) {
        if (node->is_flat()) {
            std::cout << "flat node!\n";
        }
        traps.push_back(node->get_trapezoid());
    } else {
        if (node->get_L() != nullptr) {
            write_leaf_traps(node->get_L(), traps);
        }

        if (node->get_R() != nullptr) {
            write_leaf_traps(node->get_R(), traps);
        }

        if (node->get_A() != nullptr) {
            write_leaf_traps(node->get_A(), traps);
        }

        if (node->get_B() != nullptr) {
            write_leaf_traps(node->get_B(), traps);
        }
    }
}

TEST(TSDTests, leafInsertTest) {
    auto tsd = TSD<PointCart, int>();

    Segment<PointCart, int> seg_1 = Segment<PointCart, int>(
        PointCart(30, 50), PointCart(50, 50), 2
    );

    Segment<PointCart, int> seg_2 = Segment<PointCart, int>(
        PointCart(10, 50), PointCart(60, 50), 1
    );

    Segment<PointCart, int> seg_3 = Segment<PointCart, int>(
        PointCart(40, 80), PointCart(40, 10), 3
    );
    Segment<PointCart, int> seg_4 = Segment<PointCart, int>(
        PointCart(0, 50), PointCart(100, 50), 4
    );
    Segment<PointCart, int> seg_5 = Segment<PointCart, int>(
        PointCart(41, 80), PointCart(41, 10), 5
    );
    // Segment<PointCart, int> seg_6 = Segment<PointCart, int>(
    //     PointCart(3, 98), PointCart(97, 3), 6
    // );
    // Segment<PointCart, int> seg_7 = Segment<PointCart, int>(
    //     PointCart(31, 51), PointCart(71, 61), 7
    // );

    // Segment<PointCart, int> seg_8 = Segment<PointCart, int>(
    //     PointCart(41, 31), PointCart(81, 31), 8
    // );

    // Segment<PointCart, int> seg_9 = Segment<PointCart, int>(
    //     PointCart(51, 11), PointCart(91, 41), 9
    // );
    // Segment<PointCart, int> seg_10 = Segment<PointCart, int>(
    //     PointCart(11, 11), PointCart(51, 11), 10
    // );
    // Segment<PointCart, int> seg_11 = Segment<PointCart, int>(
    //     PointCart(6, 91), PointCart(96, 91), 11
    // );
    // Segment<PointCart, int> seg_12 = Segment<PointCart, int>(
    //     PointCart(4, 99), PointCart(98, 4), 12
    // );

    // Segment<PointCart, int> seg_13 = Segment<PointCart, int>(
    //     PointCart(4, 99), PointCart(4, 72), 13
    // );

    // Segment<PointCart, int> seg_14 = Segment<PointCart, int>(
    //     PointCart(3, 89), PointCart(99.999, 89), 14
    // );

    // std::mt19937 generator (1235);
    // std::uniform_real_distribution<double> dis(0.0, 1.0);

    // auto segments = std::vector<Segment<PointCart, int>>();

    // std::cout << "start...\n";
    // for (int i = 1; i <= 17; ++i) {

    //     std::cout << i << std::endl;
        
    //     PointCart s = PointCart(std::floor((double)10* dis(generator)) * 10, std::floor((double)10 * dis(generator)) * 10);
    //     PointCart t = PointCart(std::floor((double)10* dis(generator)) * 10, std::floor((double)10 * dis(generator)) * 10);

    //     while (s.x() == t.x() && s.y() == t.y())
    //         t = PointCart(std::floor((double)10* dis(generator)) * 10, std::floor((double)10 * dis(generator)) * 10);

    //     auto seg = new Segment<PointCart, int>(s, t, i); 

    //     if (i == 100) {
    //         std::cout << seg->get_source().x() << " " << seg->get_source().y()
    //         << " " << seg->get_target().x() << " " << seg->get_target().y() << "\n";
    //         tsd.affected_subdag_roots(seg);
    //         std::vector<BoundingTrap<PointCart, int>> roots = std::vector<BoundingTrap<PointCart, int>>();
    //         for (auto node: tsd.get_subdag_roots()) {
    //             roots.push_back(node->get_trapezoid());
    //         }
    //         io::write_trapezoids(roots, "plotting/roots.dat");
    //         // tsd.insert_segment(*seg);
    //     } else {
    //         tsd.insert_segment(*seg);
    //     }
    // }
    // std::cout << "end...\n";

    tsd.insert_segment(seg_2);
    tsd.insert_segment(seg_1);
    tsd.affected_subdag_roots(&seg_3);
    std::cout << tsd.get_subdag_roots().size() << "\n";
    // tsd.insert_segment(seg_4);
    // tsd.insert_segment(seg_5);
    // tsd.insert_segment(seg_6);
    // tsd.insert_segment(seg_7);
    // tsd.insert_segment(seg_8);
    // tsd.insert_segment(seg_9);
    // tsd.insert_segment(seg_10);
    // tsd.insert_segment(seg_11);
    // tsd.insert_segment(seg_12);
    // tsd.insert_segment(seg_13);
    // tsd.insert_segment(seg_14);
    // tsd.affected_subdag_roots(&seg_12);

    auto subdag_traps = std::vector<BoundingTrap<PointCart,int>>();
    for (auto node: tsd.get_subdag_roots()) {
        subdag_traps.push_back(node->get_trapezoid());
    }

    io::write_trapezoids(subdag_traps, "plotting/subdag_roots_2.dat");

    ASSERT_FALSE(tsd.get_root()->is_leaf());
    ASSERT_EQ(tsd.get_root()->get_dest_pattern(), VVE);

    auto traps = std::vector<BoundingTrap<PointCart, int>>();

    write_leaf_traps(tsd.get_root(), traps);

    io::write_trapezoids(traps, "plotting/leaf_insert.dat");    
}