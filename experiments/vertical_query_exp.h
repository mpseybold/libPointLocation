#include "../TSD.h"
#include <random>
#include <math.h>

#include <CGAL/Cartesian.h>
#include <CGAL/Segment_tree_k.h>
#include <CGAL/Range_segment_tree_traits.h>
#include "../segment_ds/SegmentDS.h"
typedef CGAL::Cartesian<double> K;
typedef CGAL::Range_segment_tree_set_traits_2<K> Traits;
typedef CGAL::Segment_tree_2<Traits> Segment_tree_2_type;





/*
* Compares the number of node visits of the naive and regular
* intersection query methods. The TSD is constructed on a ranndom
* set of horizontal segments and the queries are random vertical segments.
*
* TODO: add 
*/
// void vertical_query_exp() {

//     std::string results_dir = "vertical_query_exp_results";
//     std::string command = "mkdir " + results_dir;
//     system(&command[0]);

//     int seed = 12345;
//     std::uniform_real_distribution<double> dis(0.0, 1.0);

//     int number_of_queries = 1000;
//     std::mt19937 generator (seed);

//     std::vector<size_t> exp_sizes = { 128, 256, 512, 1024, 2048, 4096, 8192, 16384 };

//     for (auto n: exp_sizes) {

//         std::ofstream results(results_dir + "/" + std::to_string(n) + ".csv");

//         results << "k" << "," 
//             // << "naive" << "," 
//             << "non-naive" << "," 
//             << "k*log(n)" << ","
//             << "k+log(n)" << ","
//             << "seg tree" << "\n" ;

//         auto segments = std::vector<Segment<PointCart, int>*>();
//             // int n = 100;

//         std::set<double> x_coords = std::set<double>();
//         std::set<double> y_coords = std::set<double>();

//         for (int i = 1; i <= n; ++i) {
//             double x = std::floor((double)9999 * dis(generator));
//             double y = std::floor((double)9999 * dis(generator));
            
//             while (x_coords.find(x) != x_coords.end())
//                 x = std::floor((double)9999 * dis(generator));
            
//             x_coords.insert(x);

//             while (y_coords.find(y) != y_coords.end())
//                 y = std::floor((double)9999 * dis(generator));
            
//             y_coords.insert(y);
            
//             double length = 1 + std::floor((double)(9999 - x) * dis(generator));

//             x_coords.insert(x+length);

//             auto seg = new Segment<PointCart, int>(
//                 PointCart(x, y), PointCart(x + length, y), i
//                 // PointCart(2*i, i), PointCart(2*i + n, i), i//std::floor(dis(generator)*1000000)
//             );
//             segments.push_back(seg);
//         }

//         TSD<PointCart, int> tsd = TSD<PointCart, int>();

//         // typedef Traits::Interval Interval;
//         // typedef Traits::Key Key;
//         // std::list<Interval> InputList, OutputList1, OutputList2;

//         std::vector<SegmentDS::Segment*> seg_ds_input = std::vector<SegmentDS::Segment*>();
//         auto start_time = std::chrono::high_resolution_clock::now();
//         for (int j = 0; j < segments.size(); ++j) {
//             if (j % 50 == 0)
//                 std::cout << j << ", n=" << n << std::endl;
//             tsd.insert_segment(*segments[j]);

//             double x1 = CGAL::to_double((*segments[j]).get_source().x());
//             double y1 = CGAL::to_double((*segments[j]).get_source().y());
//             double x2 = CGAL::to_double((*segments[j]).get_target().x());
//             double y2 = CGAL::to_double((*segments[j]).get_target().y());

//             seg_ds_input.push_back(new SegmentDS::Segment(x1, y1, x2, y2));

//             // std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;

//             // InputList.push_back(Interval(Key(x1,y1), Key(x2,y2+0.0001)));
//         }

//         auto end_time = std::chrono::high_resolution_clock::now();
//         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
//         std::cout << "TSD duration: " + std::to_string(duration) + "\n";

//         start_time = std::chrono::high_resolution_clock::now();
//         // Segment_tree_2_type Segment_tree_2(InputList.begin(),InputList.end());
//         auto segDS = SegmentDS::SegmentDS(seg_ds_input);
//         end_time = std::chrono::high_resolution_clock::now();
//         duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
//         std::cout << "seg tree duration: " + std::to_string(duration) + "\n";





        
//         for (int i = 0; i < number_of_queries; ++i) {
//             int query_x = std::floor((double)9999 * dis(generator));

//             while (x_coords.find(query_x) != x_coords.end())
//                 query_x = std::floor((double)9999 * dis(generator));

//             int query_y = std::floor((double)9999 * dis(generator));

//             int query_length = 1 + std::floor((double)(10000 - query_y) * dis(generator));

//             Segment<PointCart, int>* query_seg = new Segment<PointCart, int>(
//                 PointCart(query_x, query_y), PointCart(query_x, query_y+query_length), 100000
//             );

//             // auto segs = tsd.naive_intersection_query(query_seg);
//             tsd.affected_subdag_roots(query_seg, true);
//             int k = tsd.get_subdag_roots().size() - 1;



//             double qx1 = CGAL::to_double(query_seg->get_source().x());
//             double qy1 = CGAL::to_double(query_seg->get_source().y());
//             double qx2 = CGAL::to_double(query_seg->get_target().x());
//             double qy2 = CGAL::to_double(query_seg->get_target().y());
//             // auto query_int = Interval(Key(qx2,qy2), Key(qx1+0.00000001,qy1));
//             auto _query_seg = SegmentDS::Segment(qx1, qy2, qx2, qy1);
//             auto output = std::vector<SegmentDS::Segment>();
//             segDS.vertical_query(_query_seg, output);
//             // CGAL::variables_are_our_friends = 0;
//             // Segment_tree_2.window_query(query_int, std::back_inserter(OutputList1));
//             // std::cout << "seg tree visits: " << CGAL::variables_are_our_friends << std::endl;
//             // std::cout << "tsd visits: " << tsd.get_node_visits() << std::endl;
            
//             std::cout << "seg tree: " << output.size() << std::endl;
//             std::cout << "k: " << k << std::endl;
    

//             results << k << "," 
//             // << tsd.get_naive_node_visits() << "," 
//             << tsd.get_node_visits() << ","
//             << k*log2(n) << ","
//             << k + log2(n) << "," 
//             << SegmentDS::node_visits << "\n";
//         }
//         results.close();
//     }

// }