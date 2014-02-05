#include <QColor>
#include <QApplication>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include <cg/primitives/contour.h>
#include <cg/primitives/point.h>
#include <cg/algorithms/distanceTwoPoligons.h>

using cg::point_2;
using cg::point_2f;
using cg::contour_2f;

void draw_contour(contour_2f const& p, cg::visualization::drawer_type & drawer)
{
    if (p.size() <= 1)
         return;
    for (auto a = p.begin(), b = a + 1; b != p.end(); a++, b++)
        drawer.draw_line(*a, *b);
}

struct fourTangent_viewer : cg::visualization::viewer_adapter
{
    fourTangent_viewer(){}

    ~fourTangent_viewer()
    {
        a.clear();
        b.clear();
    }

    void draw(cg::visualization::drawer_type & drawer) const
    {
        if (!readyA && a.size() >= 2)
        {
            drawer.set_color(Qt::yellow);
            drawer.draw_point(*a.begin(), MAGIC);
            drawer.set_color(Qt::black);
            drawer.draw_point(*a.begin(), MAGIC - 2);
        }
        if (readyA && !readyB && b.size() >= 2)
        {
            drawer.set_color(Qt::yellow);
            drawer.draw_point(*b.begin(), MAGIC);
            drawer.set_color(Qt::black);
            drawer.draw_point(*b.begin(), MAGIC - 2);
        }
        drawer.set_color(Qt::white);
        draw_contour(a, drawer);
        drawer.set_color(Qt::yellow);
        draw_contour(b, drawer);
        drawer.set_color(Qt::blue);
        if (readyA && readyB)
            drawer.draw_line(c.first, c.second, 2);
    }

    void print(cg::visualization::printer_type & p) const
    {
        p.corner_stream() << "Distance between two convex contours." << cg::visualization::endl;
        p.corner_stream() << "Double click to clear, right-click to add point.";
    }

    bool on_press(const point_2f & p)
    {
        if (!readyA)
        {
            if (a.size() > 1 && cg::dist(*a.begin(), p) < MAGIC)
            {
                a.add_point(*a.begin());
                return readyA = true;
            }
            a.add_point(p);
            return true;
        }
        if (!readyB)
        {
            if (b.size() > 1 && cg::dist(*b.begin(), p) < MAGIC)
            {
                b.add_point(*b.begin());
                c = cg::distanceTwoPoligons(a, b);
                return readyB = true;
            }
            b.add_point(p);
            return true;
        }
        return true;
    }

    bool on_double_click(point_2f const&)
    {
        readyA = false;
        readyB = false;
        a.clear();
        b.clear();
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

    static constexpr double MAGIC = 20.0;
    bool readyA = false, readyB = false;
    contour_2f a, b;
    std::pair<cg::point_2f, cg::point_2f> c;
};

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    fourTangent_viewer viewer;
    cg::visualization::run_viewer(&viewer, "Distance between two convex poligons");
}
