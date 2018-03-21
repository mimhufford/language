#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum TokenType
{
    INT, FLOAT, CHAR, STRING, IDENTIFIER, BINARY, UNARY, 
};

enum TokenMod
{
    // BINARY OPERATORS
    ADD, SUB, MUL, DIV, MOD, LSHIFT, RSHIFT,
    ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN,
    MOD_ASSIGN, LSHIFT_ASSIGN, RSHIFT_ASSIGN, ASSIGN,
    LT, LTE, GT, GTE, EQ, NEQ, AND, OR,
    AND_BIT, OR_BIT, XOR_BIT,

    // UNARY OPERATORS
    DEREF, MINUS, NOT_BIT, NOT,

    // INT TYPES
    BIN, OCT, DEC, HEX, CHR,
};

struct Token
{
    TokenType type;
    TokenMod  mod;
    int       ival;
    double    fval;
    string    sval;
};

int chartodec(char c)
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

void num(istream& i, ostream& o)
{
    int base = 10;
    int result = 0;
    char first = i.get();

    if (first == '0')
    {
        char next = tolower(i.peek());

        if      (next == 'x')   { base = 16; i.get(); }
        else if (next == 'b')   { base = 2;  i.get(); }
        else if (isdigit(next)) { base = 8;           }
    }
    else 
    {
        result = chartodec(first);
    }

    while (isdigit(i.peek()) || i.peek() == '_' || (tolower(i.peek()) >= 'a' && tolower(i.peek()) <= 'f'))
    {
        char c = i.get();
        if (c == '_') continue;
        int digit = chartodec(c);
        result = result*base + digit;
    }

    o << result;
}

void ident(istream& i, ostream& o)
{
    string id = "";

    while (isalnum(i.peek()) || i.peek() == '_')
    {
        char c = i.get();
        id += c;
    }

    o << id;
}

void quote(istream& i, ostream& o)
{
    char opener = i.get();

    while (i.peek() != opener)
    {
        char c = i.get();
        if (c == '\\' && i.peek() == opener) i.get(c); // dump the escape char
        o << c;
    }

    i.get(); // dump the end quote
}

void op(istream& i, ostream& o)
{
    char first = i.get();
    char next  = i.peek();

    o << first;

    switch (first)
    {
        case '+': if (next == '=') o << (char)i.get(); break;
        case '-': if (next == '=') o << (char)i.get(); break;
        case '/': if (next == '=') o << (char)i.get(); break;
        case '*': if (next == '=') o << (char)i.get(); break;
        case '>': if (next == '=') o << (char)i.get(); break;
        case '<': if (next == '=') o << (char)i.get(); break;
        case '%': if (next == '=') o << (char)i.get(); break;
        case '~': if (next == '=') o << (char)i.get(); break;
        case '&': if (next == '&') o << (char)i.get(); 
                  if (next == '=') o << (char)i.get(); break;
        case '|': if (next == '|') o << (char)i.get(); 
                  if (next == '=') o << (char)i.get(); break;
        default : break;
    }
}

bool    isop(char c) { return c == '+' || c == '/' || c == '*' || c == '-' || c == '%' || c == '*' || c == '=' || c == '<' || c == '>' || c == '&' || c == '|' || c == '~'; }
bool  isopen(char c) { return c == '(' || c == '{' || c == '['; }
bool isclose(char c) { return c == ')' || c == '}' || c == ']'; }
bool isquote(char c) { return c == '"' || c == '\''; }

string lex(string input)
{
    istringstream i(input);
    ostringstream o;
    vector<Token> tokens;

    while (i.peek() != -1)
    {
        char c = i.peek();

        if      (isdigit(c)) { o << "NUMBER "     ;   num(i, o); o << "\n"; }
        else if (isalpha(c)) { o << "IDENT  "     ; ident(i, o); o << "\n"; }
        else if (isquote(c)) { o << "STRING "     ; quote(i, o); o << "\n"; }
        else if (isop(c))    { o << "OP     "     ;    op(i, o); o << "\n"; }
        else if (isopen(c))  { o << "OPEN   " << c; i.get();     o << "\n"; }
        else if (isclose(c)) { o << "CLOSE  " << c; i.get();     o << "\n"; }
        else if (c == ',')   { o << "COMMA  " << c; i.get();     o << "\n"; }
        else if (isspace(c)) { i.get(); /* dump whitespace */               }
        else                 { o << "?????? " << c; i.get();     o << "\n"; }
    }

    return o.str();
}

int main(int argc, char* argv[])
{
    string tests[] = {
        "12 + 24",
        "0xFF",
        "0123",
        "0b1010",
        "int a = 34",
        "a += 34",
        "go ()",
        "float result = add(1, 2)",
        "int big = 10_000_000",
        "string quote = \"Mim said \\\"holy shit, it works!\\\", Greg was not amused.\"",
        "string name = \"mim\"",
        "if a >= b return a else return b",
        "test(a(), b)",
        "a %= 5",
        "int* pi = &a",
    };

    for (auto test : tests)
    {
        cout << test << "     \n";
        cout << "-------------\n";
        cout << lex(test) << "\n";
    }
}