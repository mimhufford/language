#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum TokenType
{
    INT, FLOAT, CHAR, STRING, IDENTIFIER, OPERATOR, KEYWORD
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

Token num(istream& i)
{
    Token t;
    t.type = INT;
    t.ival = 0;

    int base = 10;
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
        t.ival = chartodec(first);
    }

    while (isdigit(i.peek()) || i.peek() == '_' || (tolower(i.peek()) >= 'a' && tolower(i.peek()) <= 'f'))
    {
        char c = i.get();
        if (c == '_') continue;
        int digit = chartodec(c);
        t.ival = t.ival*base + digit;
    }

    return t;
}

Token ident(istream& i)
{
    Token t;
    t.type = IDENTIFIER;
    t.sval = "";

    while (isalnum(i.peek()) || i.peek() == '_')
    {
        t.sval += (char)i.get();
    }

    vector<string> keywords = {
        "return", "while", "for", "if", "else", "struct",
        "i8", "i16", "i32", "i64", "u8", "u16", "u32", "u64",
        "f32", "f64", "char", "string", "void",
    }; 

    for (auto keyword : keywords)
    {
        if (t.sval == keyword)
        {
            t.type = KEYWORD;
            break;
        }
    }

    return t;
}

Token quote(istream& i)
{
    Token t;
    t.type = STRING;
    t.sval = "";

    char opener = i.get();

    while (i.peek() != opener)
    {
        char c = i.get();
        if (c == '\\' && i.peek() == opener) i.get(c); // dump the escape char
        t.sval += c;
    }

    i.get(); // dump the end quote

    return t;
}

Token op(istream& i)
{
    char first = i.get();
    char next  = i.peek();

    Token t;
    t.type = OPERATOR;
    t.sval = first;

    switch (first)
    {
        case '+': if (next == '=') t.sval += (char)i.get(); break;
        case '-': if (next == '=') t.sval += (char)i.get(); break;
        case '/': if (next == '=') t.sval += (char)i.get(); break;
        case '*': if (next == '=') t.sval += (char)i.get(); break;
        case '>': if (next == '=') t.sval += (char)i.get(); break;
        case '<': if (next == '=') t.sval += (char)i.get(); break;
        case '%': if (next == '=') t.sval += (char)i.get(); break;
        case '~': if (next == '=') t.sval += (char)i.get(); break;
        case '&': if (next == '&') t.sval += (char)i.get(); 
                  if (next == '=') t.sval += (char)i.get(); break;
        case '|': if (next == '|') t.sval += (char)i.get(); 
                  if (next == '=') t.sval += (char)i.get(); break;
        default : break;
    }

    return t;
}

bool    isop(char c) { return c == '+' || c == '/' || c == '*' || c == '-' || c == '%' || c == '*' || c == '=' || c == '<' || c == '>' || c == '&' || c == '|' || c == '~'; }
bool  isopen(char c) { return c == '(' || c == '{' || c == '['; }
bool isclose(char c) { return c == ')' || c == '}' || c == ']'; }
bool isquote(char c) { return c == '"' || c == '\''; }

vector<Token> lex(string input)
{
    istringstream i(input);
    vector<Token> o;

    while (i.peek() != -1)
    {
        char c = i.peek();

        if      (isdigit(c)) { o.push_back(num(i));   }
        else if (isalpha(c)) { o.push_back(ident(i)); }
        else if (isquote(c)) { o.push_back(quote(i)); }
        else if (isop(c))    { o.push_back(op(i));    }
        else if (isopen(c))  { i.get();  }
        else if (isclose(c)) { i.get();  }
        else if (c == ',')   { i.get();  }
        else if (isspace(c)) { i.get();  }
        else                 { i.get();  }
    }

    return o;
}

string print(Token t)
{
    ostringstream o;
    switch (t.type)
    {
        case INT:        o << "INT       " << t.ival; return o.str();
        case FLOAT:      o << "FLOAT     " << t.ival; return o.str();
        case CHAR:       o << "CHAR      " << t.ival; return o.str();
        case STRING:     o << "STRING    " << t.sval; return o.str();
        case IDENTIFIER: o << "IDENTFIER " << t.sval; return o.str();
        case OPERATOR:   o << "OPERATOR  " << t.sval; return o.str();
        case KEYWORD:    o << "KEYWORD   " << t.sval; return o.str();
		default:         o << "UNKNOWN!!!";           return o.str();
    }
}

int main(int argc, char* argv[])
{
    string tests[] = {
        "12 + 24",
        "0xFF",
        "0123",
        "0b1010",
        "i32 a = 34",
        "a += 34",
        "go ()",
        "f32 result = add(1, 2)",
        "i64 big = 10_000_000",
        "string quote = \"Mim said \\\"holy shit, it works!\\\", Greg was not amused.\"",
        "string name = \"mim\"",
        "if a >= b return a else return b",
        "test(a(), b)",
        "a %= 5",
        "i32* pi = &a",
    };

    for (auto test : tests)
    {
        cout << test << "            \n";
        cout << "--------------------\n";
        for (auto token : lex(test))
            cout << print(token) << "\n";
        cout << "                    \n";
    }
}