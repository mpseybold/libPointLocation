#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include "TSD.h"

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

        cuts.push_back(trap.get_bottom());
        if (trap.get_right().get_cut_type() != NO_CUT)
            cuts.push_back(trap.get_right());
        cuts.push_back(trap.get_top());
        if (trap.get_left().get_cut_type() != NO_CUT)
            cuts.push_back(trap.get_left());

        std::vector<std::pair<Point_2, Point_2>> segments = std::vector<std::pair<Point_2, Point_2>>();

        for (int i = 1; i <= cuts.size(); ++i) {
            int index = i % cuts.size();
            int prev_index = index == 0 ? cuts.size() - 1 : index - 1;
            Line_2 defining_line = get_defining_line(cuts[index]);
            Line_2 prev_line = get_defining_line(cuts[prev_index]);
            Line_2 next_line = get_defining_line(cuts[(index + 1) % cuts.size()]);

            auto intersection_1 = CGAL::intersection(prev_line, defining_line);
            Point_2 source;
            if (!CGAL::assign(source, intersection_1))
                throw std::logic_error("lines do not intersect");

            auto intersection_2 = CGAL::intersection(defining_line, next_line);
            Point_2 target;
            if (! CGAL::assign(target, intersection_2))
                throw std::logic_error("lines do not intersect");

            file << source.x() << " " << source.y() << 
            " " << target.x() << " " << target.y() << std::endl;
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
}