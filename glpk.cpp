#include "instance.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    Instance i = processInput();
    for (int f : i.pathesWithFacility[1])
    {
        cout << f << endl;
    }
    i.writeGLPKFile(argc > 1 ? argv[1] : "outglpk.mod");
}
