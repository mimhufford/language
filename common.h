#pragma once

#include <stdio.h>

int CharToDec(char c)
{
    switch (tolower(c))
    {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'a': return 10;
        case 'b': return 11;
        case 'c': return 12;
        case 'd': return 13;
        case 'e': return 14;
        case 'f': return 15;
        default:  return -1;
    }
};
 
bool    isop(char c) { return c == '=' || c == '+' || c == '/' || c == '-' || c == '%' || c == '*' || c == '=' || c == '<' || c == '>' || c == '&' || c == '|' || c == '~' || c == '!' || c == '^'; }
bool  isopen(char c) { return c == '(' || c == '{' || c == '['; }
bool isclose(char c) { return c == ')' || c == '}' || c == ']'; }
bool isquote(char c) { return c == '"' || c == '\''; }
bool isnumch(char c) { return isdigit(c) || c == '_' || c == '.' || (tolower(c) >= 'a' && tolower(c) <= 'f'); }