#include <bits/stdc++.h>
#include <ctime>
#include <writeln.h>
#define MAXN 5
#define count asdfasdfsdfg
using namespace std;
using namespace std::placeholders;

static const double l = 0.;
static const double x10 = 1., x20 = 0.;
static const double coef = 100.;
static const double denum = 1.;
static const double eps = 0.01;// / coef;
static const double delta = 0.001;// / coef;

long long count;

long long cnk[MAXN][MAXN];

void build_cnk()
{
    for (int i = 0; i < MAXN; i++)
        cnk[i][0] = 1ll;
    for (int i = 1; i < MAXN; i++)
        for (int j = 1; j < MAXN; j++)
            cnk[i][j] = cnk[i - 1][j] + cnk[i - 1][j - 1];
}

auto d2f(std::function<double(double, double)> f) -> decltype(f)
{
    return [f](double x, double y)
    {
        double ans = f(x + delta, y) - f(x, y);
        double t = ans;
        int sign = -1;
        for (int i = 1; i <= MAXN; i++, sign *= -1)
        {
            double tcur = 0;
            for (int j = 0, s = 1; j <= i; j++, s *= -1)
                tcur += s * cnk[i][j] * f(x + delta * (i - j + 1), y);
            t = tcur - t;
            ans += sign * t / (i + 1);
        }
        return ans / delta;
    };
}

auto df(std::function<double(double)> f) -> decltype(f)
{
    return bind(d2f([f](double x, double y){return f(x);}), _1, 0);
}

double f(double x1, double x2)
{
    count++;
    return (pow(x1, 4) + pow(x2, 4) - 5 * (x1 * x2 - x1 * x1 * x2 * x2)) / denum;
}

double minimize(std::function<double(double)> f) //Newton method for minimization of one-dimensional function
{
    for (double x = -df(f)(0) / df(df(f))(0); ; x -= df(f)(x) / df(df(f))(x))
        if (fabs(df(f)(x)) <= eps)
            return x;
}

double fx1(double x1, double x2)
{
    return df(bind(f, _1, x2))(x1);
}

double fx2(double x1, double x2)
{
    return df(bind(f, x1, _1))(x2);
}

bool check(double x1, double x2)
{
    cout << x1 << " " << x2 << "\n";
    return (fabs(fx1(x1, x2)) <= eps && fabs(fx2(x1, x2)) <= eps);
}

pair<double, double> gradientDownFragmentation()
{
    double x1 = x10, x2 = x20;
    int countOfOperations = 0;
    double alpha = 1, x11, x21, t, f1, f2;
    while (!check(x1, x2))
    {
        countOfOperations++;
        t = f(x1, x2);
        while (true)
        {
            f1 = fx1(x1, x2);
            f2 = fx2(x1, x2);
            x11 = x1 - alpha * f1;
            x21 = x2 - alpha * f2;
            if (f(x11, x21) > t - eps * alpha * (f1 * f1 + f2 * f2))
                alpha /= 2;
            else
                break;
        }
        x1 = x11;
        x2 = x21;
    }
    cout << "count of operations = " << countOfOperations << endl;
    return {x1, x2};
}

pair<double, double> fastestGradientDown()
{
    double x1 = x10, x2 = x20;
    int countOfOperations = 0;
    double alpha = 1, f1, f2;
    auto fun = [&x1, &x2](double lambda){return f(x1 - lambda * fx1(x1, x2), x2 - lambda * fx2(x1, x2));};
    while (!check(x1, x2))
    {
        countOfOperations++;
        f1 = fx1(x1, x2);
        f2 = fx2(x1, x2);
        alpha = minimize(fun);
        x1 = x1 - alpha * f1;
        x2 = x2 - alpha * f2;
    }
    cout << "count of operations = " << countOfOperations << endl;
    return {x1, x2};
}

int main()
{
    pair<double, double> temp;
    cout.precision(10);
    build_cnk();
    freopen("output.txt", "w", stdout);
    count = 0;
    cout << "gradient down with fragmentation:\n" << (temp = gradientDownFragmentation()).first << " ";cout << temp.second << endl << endl;
    cout << "count = " << count << endl;
    count = 0;
    cout << "fastest gradient down:\n" << (temp = fastestGradientDown()).first << " ";cout << temp.second << endl << endl;
    cout << "count = " << count << endl;
    fclose(stdout);
    system("javac *.java");
    FILE *file = fopen("MainFrame.class", "r");
    if (file == NULL)
        system("sleep 1");
    else
        fclose(file);
    system("java MainFrame &");
    return 0;
}
