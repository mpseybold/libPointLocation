#include "io.h"

void io::write_trapezoid(BoundingTrap<PointCart, int> trap, std::fstream& file) {
        return;
        std::vector<Cut<PointCart, int>> cuts = std::vector<Cut<PointCart, int>>();
        cuts.push_back(*trap.get_bottom());
        if (trap.get_right() != nullptr)
            cuts.push_back(*(trap.get_right()));
        cuts.push_back(*(trap.get_top()));
        if (trap.get_left() != nullptr)
            cuts.push_back(*(trap.get_left()));

        auto top_seg = trap.get_top()->get_segment();

        if (top_seg->get_source().x() == top_seg->get_target().x())
            return;

        auto bottom_seg = trap.get_bottom()->get_segment();

        if (bottom_seg->get_source().x() == bottom_seg->get_target().x())
            return;
        

        for (int i = 1; i <= cuts.size(); ++i) {
            int index = i % cuts.size();
            int prev_index = index == 0 ? cuts.size() - 1 : index - 1;
            Line_2 defining_line = get_defining_line(cuts[index]);
            Line_2 prev_line = get_defining_line(cuts[prev_index]);
            Line_2 next_line = get_defining_line(cuts[(index + 1) % cuts.size()]);

            auto intersection_1 = CGAL::intersection(prev_line, defining_line);
            Point_2 source;
            if (!CGAL::assign(source, intersection_1)) 
                continue;
                //assert(false);
            
            auto intersection_2 = CGAL::intersection(defining_line, next_line);
            Point_2 target;
            if (!CGAL::assign(target, intersection_2))
                continue;
                // assert(false);
            file << std::setprecision(std::numeric_limits<long double>::digits10);
            file << CGAL::to_double(source.x()) << " " << CGAL::to_double(source.y()) << 
            " " << CGAL::to_double(target.x()) << " " << CGAL::to_double(target.y()) << std::endl;
        }
    }

    void io::write_trapezoids(std::vector<BoundingTrap<PointCart, int>> traps, std::string filename) {
        return;
        std::fstream file;
        file.open(filename, std::fstream::out | std::fstream::trunc);

        for (auto& trap: traps) {
            io::write_trapezoid(trap, file);
        }

        file.close();
    }

    void io::write_trapezoids(std::vector<Node<PointCart, int>*> nodes, std::string filename) {
        auto traps = std::vector<BoundingTrap<PointCart, int>> {};
        
        for (auto node: nodes) {
            traps.push_back(node->get_trapezoid());
        }

        io::write_trapezoids(traps, filename);
    }

    void io::write_segments(std::vector<Segment<PointCart, int>*> segs, int index, std::string filename) {
        return;
        assert(index < segs.size());

        std::ofstream file(filename);

        for (int i = 0; i <= index; ++i) {
            auto seg = segs[i];
            file << CGAL::to_double(seg->get_source().x()) 
            << " " << CGAL::to_double(seg->get_source().y()) 
            << " " << CGAL::to_double(seg->get_target().x()) 
            << " " << CGAL::to_double(seg->get_target().y())
            << " " << seg->get_priority() << std::endl;
        }

        file.close();
    }

    void io::write_segments(std::vector<std::vector<double>> segments, std::string filename) {
        std::cout << "writing...";
        std::ofstream file(filename);
        file << std::setprecision(std::numeric_limits<long double>::digits10);

        for (auto seg: segments) {
            // std::cout << seg[0] << std::endl;
            file << seg[0] << " " << seg[1] 
            << " " << seg[2] << " " << seg[3] << std::endl; 
        }

        file.close();
    }

    void io::write_segments(std::vector<std::vector<long double>> segments, std::string filename) {
        std::cout << "writing...";
        std::ofstream file(filename);
        file << std::setprecision(std::numeric_limits<long double>::digits10);

        for (auto seg: segments) {
            // std::cout << seg[0] << std::endl;
            file << seg[0] << " " << seg[1] 
            << " " << seg[2] << " " << seg[3] << std::endl; 
        }

        file.close();
    }

    std::vector<std::vector<long double>> io::read_segments(std::string filename) {
        std::string path = filename;
        
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cout << "failed to open file\n";
        } else 
            std::cout << "file open...\n";

        std::vector<std::vector<long double>> output;
        std::vector<std::vector<long double>> seg_coords;
        long double x_1, y_1, x_2, y_2;
        while (file >> x_1 >> y_1 >> x_2 >> y_2) {
            // std::cout << x_1 << " " << y_1 << " " << x_2 << " " << y_2 << std::endl;
            std::vector<long double> coords;
            coords.push_back(x_1);
            coords.push_back(y_1);
            coords.push_back(x_2);
            coords.push_back(y_2);
            seg_coords.push_back(coords);
        }

        file.close();

        for (int i = 0; i < seg_coords.size(); ++i) {
            output.push_back(seg_coords[i]);
        }

        return output;
    }

    template <class PointType, class OrderType>
    std::vector<Segment<PointType, OrderType>> io::read_segments(std::string filename) {
        std::string path = "data/" + filename;

        std::cout << path << std::endl;
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cout << "failed to open file\n";
        } 

        std::vector<Segment<PointType, OrderType>> output;
        std::vector<std::vector<long double>> seg_coords;
        long double x_1, y_1, x_2, y_2;
        while (file >> x_1 >> y_1 >> x_2 >> y_2) {
            std::vector<long double> coords;
            coords.push_back(x_1);
            coords.push_back(y_1);
            coords.push_back(x_2);
            coords.push_back(y_2);
            seg_coords.push_back(coords);
        }

        file.close();

        // std::random_device rd;
        // std::mt19937 g(rd());

        std::random_shuffle(seg_coords.begin(), seg_coords.end());

        for (int i = 0; i < seg_coords.size(); ++i) {
            output.push_back(
                Segment<PointType, OrderType>(
                    PointType(seg_coords[i][0], seg_coords[i][1]), PointType(seg_coords[i][2], seg_coords[i][3]), i
                )
            );
        }

        return output;
    }