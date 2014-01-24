#include <QColor>
#include <QApplication>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include <cg/primitives/segment.h>
#include <cg/primitives/point.h>
#include <cg/algorithms/douglasPeucker.h>

#include <cg/operations/has_intersection/segment_segment.h>

using cg::point_2;
using cg::point_2f;

void draw_vector(std::vector<point_2> const& p, cg::visualization::drawer_type & drawer)
{
    if (p.size() <= 1)
         return;
    for (auto a = p.begin(), b = a + 1; b != p.end(); a++, b++)
        drawer.draw_line(*a, *b);
}

struct douglasPeucker_viewer : cg::visualization::viewer_adapter
{
    douglasPeucker_viewer(){}

    ~douglasPeucker_viewer()
    {
        input_points.clear();
        output_points.clear();
    }

    void draw(cg::visualization::drawer_type & drawer) const
    {
        drawer.set_color(Qt::white);
        draw_vector(input_points, drawer);
        drawer.set_color(Qt::yellow);
        draw_vector(output_points, drawer);
    }

    void print(cg::visualization::printer_type & p) const
    {
        p.corner_stream() << "Simplifying polyline. DouglasPeucker algorithm" << cg::visualization::endl;
        p.corner_stream() << "Double click to clear, right-click to add point.";
    }

    bool on_press(const point_2f & p)
    {
        input_points.push_back({p.x, p.y});
        output_points.clear();
        cg::douglasPeucker(input_points.begin(), input_points.end(), 5.0, std::back_inserter(output_points));
        return true;
    }

    bool on_double_click(point_2f const&)
    {
        input_points.clear();
        output_points.clear();
        return true;
    }

    bool on_release(const point_2f & p)
    {
        return false;
    }

    bool on_move(const point_2f & p)
    {
        return false;
    }

private:

    std::vector<point_2> input_points;
    std::vector<point_2> output_points;
};

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    douglasPeucker_viewer viewer;
    cg::visualization::run_viewer(&viewer, "douglasPeucker");
}
