#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include "Cut.h"
#include "BoundingTrap.h"
#include <random>
#include "Point_2D.h"

namespace {
    Line_2 get_defining_line(Cut<PointCart, int> cut) {
        switch(cut.get_cut_type()) {
            case SOURCE: {
                Point_2 point = cut.get_segment()->get_source().get_cgal_point();
                return Line_2(point, Vector_2(0, -1));
            }
            case TARGET: {
                Point_2 point = cut.get_segment()->get_target().get_cgal_point();
                return Line_2(point, Vector_2(0, -1));
            }
            case INTERSECTION: {
                Line_2 line_1 = Line_2(
                    cut.get_segment()->get_source().get_cgal_point(),
                    cut.get_segment()->get_target().get_cgal_point()
                );
                Line_2 line_2 = Line_2(
                    cut.get_intersecting_seg()->get_source().get_cgal_point(),
                    cut.get_intersecting_seg()->get_target().get_cgal_point()
                );
                auto intersection = CGAL::intersection(line_1, line_2);
                Point_2 point = boost::get<Point_2>(*intersection);
                
                return Line_2(point, Vector_2(0, -1));
            }
            case EDGE: {
                return Line_2(
                    cut.get_segment()->get_source().get_cgal_point(),
                    cut.get_segment()->get_target().get_cgal_point()
                );
            }
        }
    }
}

namespace io {
    void write_trapezoid(BoundingTrap<PointCart, int> trap, std::fstream& file) {
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

    void write_trapezoids(std::vector<BoundingTrap<PointCart, int>> traps, std::string filename) {
        std::fstream file;
        file.open(filename, std::fstream::out | std::fstream::trunc);

        for (auto& trap: traps) {
            write_trapezoid(trap, file);
        }

        file.close();
    }

    void write_trapezoids(std::vector<Node<PointCart, int>*> nodes, std::string filename) {
        auto traps = std::vector<BoundingTrap<PointCart, int>> {};
        
        for (auto node: nodes) {
            traps.push_back(node->get_trapezoid());
        }

        write_trapezoids(traps, filename);
    }

    void write_segments(std::vector<Segment<PointCart, int>*> segs, int index, std::string filename) {
        
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

    template <class PointType, class OrderType>
    std::vector<Segment<PointType, OrderType>> read_segments(std::string filename) {
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
}