#include <QColor>
#include <QApplication>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include <cg/primitives/segment.h>
#include <cg/primitives/point.h>
#include <cg/primitives/contour.h>
#include <cg/algorithms/douglasPeucker.h>

#include <cg/operations/has_intersection/segment_segment.h>

#define MAGIC_CONST 10

using cg::point_2f;
using cg::contour_2f;
using cg::point_2f;

template <class Iter>
void draw_vector(Iter a, Iter c, cg::visualization::drawer_type & drawer)
{
    if (a == c)
        return;
    Iter b = a + 1;
    for (; b != c; a++, b++)
        drawer.draw_line(*a, *b);
}

struct visibilityGraph_viewer : cg::visualization::viewer_adapter
{
    visibilityGraph_viewer(){}

    ~visibilityGraph_viewer()
    {
        input_points.clear();
        output_points.clear();
        contour.clear();
    }

    void draw(cg::visualization::drawer_type & drawer) const
    {
        drawer.set_color(Qt::white);
        for (unsigned int i = 0; i < input_points.size(); i++)
            draw_vector(input_points[i].begin(), input_points[i].end(), drawer);
        draw_vector(contour.begin(), contour.end(), drawer);
        drawer.set_color(Qt::yellow);
        draw_vector(output_points.begin(), output_points.end(), drawer);
    }

    void print(cg::visualization::printer_type & p) const
    {
        p.corner_stream() << "visibilityGraph" << cg::visualization::endl;
        p.corner_stream() << "Double click to clear, right-click to add point." << cg::visualization::endl;
        p.corner_stream() << "To end contour, click close to first point";
    }

    bool on_press(const point_2f & p)
    {
        if (contour.size() >= 2 && cg::dist_2(p, contour[0]) < MAGIC_CONST)//if contour.begin belongs U(p), this is end of contour
        {
            contour.add_point(contour[0]);
            input_points.push_back(contour);
            output_points.clear();
            contour.clear();
           // cg::douglasPeucker(input_points.begin(), input_points.end(), 5.0, std::back_inserter(output_points));
        } else
            contour.add_point(p);
        return true;
    }

    bool on_double_click(point_2f const&)
    {
        input_points.clear();
        output_points.clear();
        contour.clear();
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

    cg::contour_2f contour;
    std::vector<contour_2f> input_points;
    std::vector<point_2f> output_points;
};

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    visibilityGraph_viewer viewer;
    cg::visualization::run_viewer(&viewer, "visibilityGraph");
}
