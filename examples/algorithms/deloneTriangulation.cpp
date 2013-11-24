#include <QColor>
#include <QApplication>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include <cg/primitives/segment.h>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/algorithms/deloneTriangulation.h>

using cg::point_2f;
using cg::triangle_2f;
using cg::segment_2f;
using cg::face_2f;

struct visibilityGraph_viewer : cg::visualization::viewer_adapter
{
template <class Iter>
void drawTriangles(Iter a, Iter c, cg::visualization::drawer_type & drawer) const
{
    for (; a != c; a++)
    {
        face_2f as = *a;
        //as.writeln();
        if (!as.isInf)
            for (int i = 0; i < 3; i++)
                {
                    drawer.set_color(Qt::white);
                    drawer.draw_line(as.side(i));
                    drawer.set_color(Qt::blue);
                    draw_circle(drawer, as.triangle());
                }
    }
}
///copied from qwerty787788
    double area(triangle_2f const & tr) const {
        double res = 0;
        for (int i = 0; i < 3; i++) {
           double dx = tr[i].x - tr[(i+1)%3].x;
           double dy = tr[i].y + tr[(i+1)%3].y;
           res += dx*dy;
        }
        return fabs(res) / 2.;
    }

    double distance(point_2f const & p1, point_2f const & p2) const {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return sqrt(dx*dx+dy*dy);
    }

    point_2f intersect(segment_2f const & s1, segment_2f const & s2) const {
        double a1 = s1[0].y - s1[1].y;
        double b1 = -s1[0].x + s1[1].x;
        double c1 = -(a1 * s1[0].x + b1 * s1[0].y);
        double a2 = s2[0].y - s2[1].y;
        double b2 = -s2[0].x + s2[1].x;
        double c2 = -(a2 * s2[0].x + b2 * s2[0].y);
        double zn = a1*b2-a2*b1;
        double x = (c2*b1-c1*b2) / zn;
        double y = (c1*a2-c2*a1) / zn;
        return point_2f(x, y);
    }

    void draw_circle2(point_2f const &c, double r, cg::visualization::drawer_type & drawer) const
    {
        double PI = asin(1) * 2;
        const int MAX_ITER = 360;
        for (int it = 0; it < MAX_ITER; it++)
        {
            double angle1 = PI * 2 *it / MAX_ITER;
            double angle2 = PI * 2 * (it + 1) / MAX_ITER;
            double x1 = c.x + r * cos(angle1);
            double x2 = c.x + r * cos(angle2);
            double y1 = c.y + r * sin(angle1);
            double y2 = c.y + r * sin(angle2);
           drawer.draw_line(point_2f(x1, y1), point_2f(x2, y2));
        }
    }

    void draw_circle(cg::visualization::drawer_type & drawer, triangle_2f const & tr) const {
        double s = area(tr);
        segment_2f s1, s2;
        {
            point_2f ab((tr[0].x+tr[1].x)/2, (tr[0].y+tr[1].y)/2);
            double dx = ab.x - tr[0].x;
            double dy = ab.y - tr[0].y;
            point_2f p2(ab.x + dy, ab.y - dx);
            s1 = segment_2f(ab, p2);
        }
        {
            point_2f ac((tr[0].x+tr[2].x)/2, (tr[0].y+tr[2].y)/2);
            double dx = ac.x - tr[0].x;
            double dy = ac.y - tr[0].y;
            point_2f p2(ac.x + dy, ac.y - dx);
            s2 = segment_2f(ac, p2);
        }
        point_2f center = intersect(s1, s2);
        double a = distance(tr[0], tr[1]);
        double b = distance(tr[1], tr[2]);
        double c = distance(tr[2], tr[0]);
        double r = a *b * c / 4 / s;
        draw_circle2(center, r, drawer);
   }
///end of copying
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
