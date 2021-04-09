#include <CGAL/Cartesian.h>
#include <CGAL/Segment_tree_k.h>
#include <CGAL/Range_segment_tree_traits.h>
typedef CGAL::Cartesian<double> K;
typedef CGAL::Segment_tree_map_traits_2<K, char> Traits;
typedef CGAL::Segment_tree_2<Traits > Segment_tree_2_type;

void example() {
  typedef Traits::Interval Interval;
  typedef Traits::Pure_interval Pure_interval;
  typedef Traits::Key Key;
  std::list<Interval> InputList, OutputList1, OutputList2;
  InputList.push_back(Interval(Pure_interval(Key(1,5), Key(2,7)),'a'));
  InputList.push_back(Interval(Pure_interval(Key(2,7), Key(3,8)),'b'));
  InputList.push_back(Interval(Pure_interval(Key(6,9), Key(9,13)),'c'));
  InputList.push_back(Interval(Pure_interval(Key(1,3), Key(3,9)),'d'));
  InputList.push_back(Interval(Pure_interval(Key(5,8), Key(7, 13)),'g'));
  Segment_tree_2_type Segment_tree_2(InputList.begin(),InputList.end());
  Interval a=Interval(Pure_interval(Key(3,6), Key(7,12)),'e');
  Segment_tree_2.window_query(a,std::back_inserter(OutputList1));
  std::list<Interval>::iterator j = OutputList1.begin();
  std::cout << "\n window_query (3,6),(7,12)\n";
  while(j!=OutputList1.end()){
    std::cout << (*j).first.first.x() << "-" << (*j).first.second.x() << " "
              << (*j).first.first.y() << "-" << (*j).first.second.y() << std::endl;
    j++;
  }
  Interval b=Interval(Pure_interval(Key(6,10),Key(7,11)), 'f');
  Segment_tree_2.enclosing_query(b,std::back_inserter(OutputList2));
  j = OutputList2.begin();
  std::cout << "\n enclosing_query (6,10),(7,11)\n";
  while(j!=OutputList2.end()){
    std::cout << (*j).first.first.x() << "-" << (*j).first.second.x() << " "
              << (*j).first.first.y() << "-" << (*j).first.second.y() << std::endl;
    j++;
  }
}