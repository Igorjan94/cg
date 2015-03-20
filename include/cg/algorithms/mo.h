#include <bits/stdc++.h>
#ifndef CGGG
#include <cg/primitives/point.h>
using cg::point_2f;
#endif
#include "/home/igorjan/206round/staff/writeln.h"
#include "/home/igorjan/206round/staff/xirtaM.cpp"

#define MAGICCONST 1

using namespace std;

double L = 10;
double l = -MAGICCONST * 10;
double r = MAGICCONST * 10 - 4;
double eps = 0.0001;
double delta = eps / 10;
int N = 50;

bool debug = true;

double axis = MAGICCONST * 10;
long long countOfOperations;

double f(double x)
{
    countOfOperations++;
    return sin(x) * x - x / 2;
}

double g(double u0, double u1)
{
    return (f(u0) - f(u1)) / 2 / L + (u0 + u1) / 2;
}

template<typename F>
double run(F f)
{
    countOfOperations = 0;
    double ans = f();
    wr(countOfOperations, ans);
    writeln();
    return ans;
}

double passiveSearch()
{
    writeln(__FUNCTION__);
    int iterationCount = 1;
    double mn = f(l), ans = l, temp;
    for (double x = l; x <= r; x += eps, iterationCount++)
        if ((temp = f(x)) < mn)
            mn = temp,
            ans = x;
    wr(iterationCount);
    return ans;
}

double dihotomia()
{
    writeln(__FUNCTION__);
    double c, d, a = l, b = r;
    int iterationCount = 1;
    while (b - a > eps)
    {
        double preva = a, prevb = b;
        iterationCount++;
        c = a + (b - a) / 2 - delta / 2;
        d = a + (b - a) / 2 + delta / 2;
        f(c) >= f(d) ? a = c : b = d;
        if (debug)
            writeln("delta =", (b - a) / (prevb - preva));
    }
    wr(iterationCount);
    return a + (b - a) / 2;
}

double gold()
{
    writeln(__FUNCTION__);
    int iterationCount = 1;
    double 
           a = l, 
           b = r, 
           fi = (1 + sqrt(5)) / 2, 
           d = a + (b - a) / fi,
           c = b - (b - a) / fi,
           fc = f(c), 
           fd = f(d);
    while (b - a > eps)
    {
        double preva = a, prevb = b;
        iterationCount++;
        if (fc <= fd)
            b = d, 
            d = c,
            c = b - (b - a) / fi, 
            fd = fc,
            fc = f(c);
        else
            a = c,
            c = d,
            d = a + (b - a) / fi,
            fc = fd,
            fd = f(d);
        if (debug)
            writeln("delta =", (b - a) / (prevb - preva));
    }
    wr(iterationCount);
    return a + (b - a) / 2;
}

double fibonacci()
{
    writeln(__FUNCTION__);
    int iterationCount = 1;
    vector<vector<double>> m = {{0, 1}, {1, 1}};
    vector<double> FN = {1, 1};
    Matrix<double> T(m);
    FN = FN * (T ^ (N - 1));
    double fn_2 = FN[0];
    double fn_1 = FN[1];
    FN = FN * T;
    double fn = FN[1];
    double 
           a = l, 
           b = r, 
           c = a + (b - a) * fn_2 / fn,
           d = a + (b - a) * fn_1 / fn,
           fc = f(c), 
           fd = f(d);
    int n = N - 1;
    while (--n)
    {
        double preva = a, prevb = b;
        iterationCount++;
        if (fc <= fd)
            b = d, 
            d = c,
            c = a + b - d,
            fd = fc,
            fc = f(c);
        else
            a = c,
            c = d,
            d = b - c + a,
            fc = fd,
            fd = f(d);
        if (debug)
            writeln("delta =", (b - a) / (prevb - preva));
    }
    wr(iterationCount);
    return f(c) < f(d) ? c : d;
}

set<double> m;

double lomanie()
{
    writeln(__FUNCTION__);
    int iterationCount = 1;
    double ans;
    m.insert(l);
    m.insert(r);

    while (true)
    {
        iterationCount++;
        double mn = std::numeric_limits<double>::max();
        double temp;
        auto x = m.begin();
        for (auto it = m.begin(); it != m.end(); it++)
            if ((temp = f(*it)) < mn)
                mn = temp,
                x = it;
        ans = *x;
        auto prev = x; --prev;
        auto next = x; ++next;
        double u;
        if (x != m.begin())
            m.insert(u = g(*prev, *x));
        if (next != m.end())
            m.insert(u = g(*x, *next));
        if (f(*x) - (f(u) - L * fabs(*x - u)) < eps || iterationCount > 1000)
            break;
    }
    wr(iterationCount);
    return ans;
}

#ifndef CGGG
vector<point_2f> lines()
{
    lomanie();
    vector<point_2f> p;
    double t = *(m.begin()), u;
    for (auto x : m)
        u = g(t, x),
        p.push_back({u, f(u) - L * fabs(x - u)}),
        p.push_back({x, f(x)}),
        t = x;
    return std::move(p);
}
#endif
