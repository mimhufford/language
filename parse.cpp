#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdint>

#include "lex.cpp"
 
using namespace std;
 
void testparse()
{
    Token ta; ta.type = INT; ta.ival = 2;
    Token tb; tb.type = INT; tb.ival = 3;
    Token to; to.type = ADD;

    cout << "=PARSE==============\n";
    cout << "                    \n";
}