#include <QColor>
#include <QApplication>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include <cg/primitives/segment.h>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/algorithms/deloneTriangulation.h>

#define MAGIC_CONST 10

using cg::point_2f;
using cg::triangle_2f;
using cg::segment_2f;
using cg::face_2f;

template <class Iter>
void drawTriangles(Iter a, Iter c, cg::visualization::drawer_type & drawer)
{
    for (; a != c; a++)
        for (int i = 0; i < 3; i++)
        {
            face_2f as = *a;
            if (!as.isInf)
                drawer.draw_line(as.side(i));
        }
}

struct visibilityGraph_viewer : cg::visualization::viewer_adapter
{
    visibilityGraph_viewer(){}

    ~visibilityGraph_viewer()
    {
        input_points.clear();
        output_points.clear();
    }

    void draw(cg::visualization::drawer_type & drawer) const
    {
        drawer.set_color(Qt::yellow);
        for (unsigned int i = 0; i < input_points.size(); i++)
            drawer.draw_point(input_points[i], 2);
        drawer.set_color(Qt::white);
        drawTriangles(output_points.begin(), output_points.end(), drawer);
    }

    void print(cg::visualization::printer_type & p) const
    {
        p.corner_stream() << "deloneTriangulation" << cg::visualization::endl;
        p.corner_stream() << "Double click to clear, right-click to add point." << cg::visualization::endl;
    }

    bool on_press(const point_2f & p)
    {
        input_points.push_back(p);
        output_points.clear();
        output_points = cg::deloneTriangulation(input_points);
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
    std::vector<point_2f> input_points;
    std::vector<face_2f> output_points;
};

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    visibilityGraph_viewer viewer;
    cg::visualization::run_viewer(&viewer, "deloneTraingulation");
}
