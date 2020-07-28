#include "gtest/gtest.h"
#include "../TSD.h"
#include "../io.h"


TEST(TSDTests, searchTest) {
    TSD<PointCart, int> tsd = TSD<PointCart, int>();
    Node<PointCart, int>* root = tsd.get_root();

    tsd.affected_subdag_roots(new Segment<PointCart, int>(
        PointCart(1, 1), PointCart(99, 99), 1
    ));
    
    ASSERT_TRUE(root == tsd.get_subdag_roots()[0]);

    //see figures/figure_1.png

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

    std::vector<BoundingTrap<PointCart, int>> leaves = {
        L->get_trapezoid(), A->get_trapezoid(), 
        BL->get_trapezoid(), BA->get_trapezoid(), 
        BB->get_trapezoid(), RA->get_trapezoid(), 
        RR->get_trapezoid()
    };
    io::write_trapezoids(leaves, "plotting/leaves.dat");

    Segment<PointCart, int> query_seg_1 = Segment<PointCart, int>(
        PointCart(10, 5), PointCart(90, 5), 2
    );

    tsd.affected_subdag_roots(&query_seg_1);
    auto roots = tsd.get_subdag_roots();
    ASSERT_EQ(roots.size(), 4);
    ASSERT_EQ(roots[0]->name, "L");
    ASSERT_EQ(roots[1]->name, "BL");
    ASSERT_EQ(roots[2]->name, "BB");
    ASSERT_EQ(roots[3]->name, "RR");

    Segment<PointCart, int> query_seg_2 = Segment<PointCart, int>(
        PointCart(39, 1), PointCart(89, 42.66666666666), 2
    );
    tsd.affected_subdag_roots(&query_seg_2);
    roots = tsd.get_subdag_roots();
    ASSERT_EQ(roots.size(), 5);
    ASSERT_EQ(roots[0]->name, "BL");
    ASSERT_EQ(roots[1]->name, "BB");
    ASSERT_EQ(roots[2]->name, "BA");
    ASSERT_EQ(roots[3]->name, "RA");
    ASSERT_EQ(roots[4]->name, "RR");

    Segment<PointCart, int> query_seg_3 = Segment<PointCart, int>(
        PointCart(0, 100), PointCart(100, 0), 2
    );
    tsd.affected_subdag_roots(&query_seg_3);
    roots = tsd.get_subdag_roots();
    ASSERT_EQ(roots.size(), 6);
    ASSERT_EQ(roots[0]->name, "L");
    ASSERT_EQ(roots[1]->name, "A");
    ASSERT_EQ(roots[2]->name, "BA");
    ASSERT_EQ(roots[3]->name, "RA");
    ASSERT_EQ(roots[4]->name, "BB");
    ASSERT_EQ(roots[5]->name, "RR");
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