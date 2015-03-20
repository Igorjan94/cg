#include "mo.h"

using namespace std;

void print(double epsi)
{
    eps = epsi;
    run(passiveSearch);
    run(dihotomia);
    run(gold);
    run(fibonacci);
    run(lomanie);
}

int main()
{
    double temp = 0.1;
    for (int i = 0; i < 4; i++)
        writeln("eps =", temp),
        print(temp),
        temp /= 10;
}
