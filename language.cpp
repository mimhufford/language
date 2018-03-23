#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdint>

#include "parse.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    cout << '\n';
    testlex();
    testparse();
}