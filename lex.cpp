#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdint>
 
using namespace std;
 
enum TokenType
{
    COLON,
    LPAREN, RPAREN,
    LBRACE, RBRACE,
    LBRACKET, RBRACKET,
    COMMA,
    DOT,
    KEYWORD,
    INT,
    FLOAT,
    STRING,
    IDENTIFIER,
 
    BEGIN_1ST_PRECENDENCE,
    //-------------------
    MUL, DIV, MOD, AND, LSHIFT, RSHIFT,
    //-------------------
    END_1ST_PRECENDENCE,
 
    BEGIN_2ND_PRECENDENCE,
    //-------------------
    ADD, SUB, XOR, OR, BNT,
    //-------------------
    END_2ND_PRECENDENCE,
 
    BEGIN_3RD_PRECENDENCE,
    //-------------------
    EQ, NEQ, LT, GT, LTEQ, GTEQ,
    //-------------------
    END_3RD_PRECENDENCE,
   
    NOT,
    AND_AND,
    OR_OR,
 
    BEGIN_4TH_PRECENDENCE,
    //-------------------
    ASSIGN, ADD_ASSIGN, SUB_ASSIGN, OR_ASSIGN, AND_ASSIGN, XOR_ASSIGN,
    LSHIFT_ASSIGN, RSHIFT_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
    //-------------------
    END_4TH_PRECENDENCE,
};
 
enum TokenSubType
{
    BIN, OCT, DEC, HEX, CHR,
};
 
struct Token
{
    TokenType    type;
    TokenSubType subtype;
    int64_t      ival;
    double       fval;
    string       sval;
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
 
bool    isop(char c) { return c == '=' || c == '+' || c == '/' || c == '-' || c == '%' || c == '*' || c == '=' || c == '<' || c == '>' || c == '&' || c == '|' || c == '~' || c == '!' || c == '^'; }
bool  isopen(char c) { return c == '(' || c == '{' || c == '['; }
bool isclose(char c) { return c == ')' || c == '}' || c == ']'; }
bool isquote(char c) { return c == '"' || c == '\''; }
bool isnumch(char c) { return isdigit(c) || c == '_' || c == '.' || (tolower(c) >= 'a' && tolower(c) <= 'f'); }
 
Token floatnum(istream& i, ostringstream& o)
{
    o << '.';
 
    Token t;
    t.type = FLOAT;
 
    while (isdigit(i.peek()) || i.peek() == '_')
    {
        char c = i.get();
        if (c == '_') continue;
        int digit = chartodec(c);
        o << c;
    }
 
    t.fval = atof(o.str().c_str());
 
    return t;
}
 
Token num(istream& i)
{
    Token t;
    t.type = INT;
    t.subtype = DEC;
    t.ival = 0;
    int base = 10;
    char first = i.get();
    ostringstream o;
    o << first;
 
    if (first == '0')
    {
        char next = tolower(i.peek());
 
        if      (next == '.')   { i.get();      return floatnum(i, o); }
        else if (next == 'x')   { t.subtype = HEX; base = 16; i.get(); }
        else if (next == 'b')   { t.subtype = BIN; base = 2;  i.get(); }
        else if (isdigit(next)) { t.subtype = OCT; base = 8;           }
    }
    else
    {
        t.ival = chartodec(first);
    }
 
    while (isnumch(i.peek()))
    {
        char c = i.get();
        if (c == '_') continue;
        if (c == '.') return floatnum(i, o);
        int digit = chartodec(c);
        assert(digit < base);
        t.ival = t.ival*base + digit;
        o << c;
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
        "f32", "f64", "char", "string", "void", "sizeof",
        "do", "continue", "break",
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
    t.sval = first;
 
    switch (first)
    {
        case '~': t.type = BNT;    break;
        case '=': t.type = ASSIGN; if (next == '=') { t.type = EQ;         t.sval += (char)i.get(); } break;
        case '!': t.type = NOT;    if (next == '=') { t.type = ADD_ASSIGN; t.sval += (char)i.get(); } break;
        case '+': t.type = ADD;    if (next == '=') { t.type = ADD_ASSIGN; t.sval += (char)i.get(); } break;
        case '-': t.type = SUB;    if (next == '=') { t.type = SUB_ASSIGN; t.sval += (char)i.get(); } break;
        case '/': t.type = DIV;    if (next == '=') { t.type = DIV_ASSIGN; t.sval += (char)i.get(); } break;
        case '*': t.type = MUL;    if (next == '=') { t.type = MUL_ASSIGN; t.sval += (char)i.get(); } break;
        case '%': t.type = MOD;    if (next == '=') { t.type = MOD_ASSIGN; t.sval += (char)i.get(); } break;
        case '^': t.type = XOR;    if (next == '=') { t.type = XOR_ASSIGN; t.sval += (char)i.get(); } break;
        case '>': t.type = GT ;
                  if (next == '=') { t.type = GTEQ; t.sval += (char)i.get(); break; }
                  if (next == '>') { 
                      t.type = RSHIFT; t.sval += (char)i.get();
                      if (i.peek() == '=') { t.type = RSHIFT_ASSIGN; t.sval += (char)i.get(); break; }
                  } break;
        case '<': t.type = LT ;
                  if (next == '=') { t.type = LTEQ; t.sval += (char)i.get(); break; }
                  if (next == '<') { 
                      t.type = LSHIFT; t.sval += (char)i.get();
                      if (i.peek() == '=') { t.type = LSHIFT_ASSIGN; t.sval += (char)i.get(); break; }
                  } break;
        case '&': t.type = AND;
                  if (next == '&') { t.type = AND_AND;    t.sval += (char)i.get(); break; }
                  if (next == '=') { t.type = AND_ASSIGN; t.sval += (char)i.get(); break; }
                  break;
        case '|': t.type = OR ;
                  if (next == '|') { t.type = OR_OR;      t.sval += (char)i.get(); break; }
                  if (next == '=') { t.type = OR_ASSIGN;  t.sval += (char)i.get(); break; }
                  break;
        default : break;
    }
 
    return t;
}
 
Token symbol(istream& i, TokenType type)
{
    Token t;
    t.type = type;
    i.get();
    return t;
}
 
vector<Token> lex(string input)
{
    istringstream i(input);
    vector<Token> o;
 
    while (i.peek() != -1)
    {
        char c = i.peek();
 
        if      (isdigit(c)) { o.push_back(num(i));              }
        else if (isalpha(c)) { o.push_back(ident(i));            }
        else if (isquote(c)) { o.push_back(quote(i));            }
        else if (isop(c))    { o.push_back(op(i));               }
        else if (c == ')')   { o.push_back(symbol(i, RPAREN));   }
        else if (c == '}')   { o.push_back(symbol(i, RBRACE));   }
        else if (c == ']')   { o.push_back(symbol(i, RBRACKET)); }
        else if (c == '(')   { o.push_back(symbol(i, LPAREN));   }
        else if (c == '{')   { o.push_back(symbol(i, LBRACE));   }
        else if (c == '[')   { o.push_back(symbol(i, LBRACKET)); }
        else if (c == ',')   { o.push_back(symbol(i, COMMA));    }
        else if (c == '.')   { o.push_back(symbol(i, DOT));      }
        else if (isspace(c)) { i.get(); /* EAT IT */             }
        else                 { assert(!"UNHANDLED");             }
    }
 
    return o;
}
 
string debugprint(Token t)
{
    ostringstream o;
    switch (t.type)
   {
        case INT:           o << "INT       " << t.ival; return o.str();
        case FLOAT:         o << "FLOAT     " << t.fval; return o.str();
        case STRING:        o << "STRING    " << t.sval; return o.str();
        case IDENTIFIER:    o << "IDENTFIER " << t.sval; return o.str();
        case KEYWORD:       o << "KEYWORD   " << t.sval; return o.str();
        case COMMA:         o << "COMMA     " << ","   ; return o.str();
        case DOT:           o << "DOT       " << "."   ; return o.str();
        case LPAREN:        o << "OPEN      " << "("   ; return o.str();
        case LBRACE:        o << "OPEN      " << "{"   ; return o.str();
        case LBRACKET:      o << "OPEN      " << "["   ; return o.str();
        case RPAREN:        o << "CLOSE     " << ")"   ; return o.str();
        case RBRACE:        o << "CLOSE     " << "}"   ; return o.str();
        case RBRACKET:      o << "CLOSE     " << "]"   ; return o.str();
        case ADD:           o << "OPERATOR  " << "+"   ; return o.str();
        case ASSIGN:        o << "OPERATOR  " << "="   ; return o.str();
        case ADD_ASSIGN:    o << "OPERATOR  " << "+="  ; return o.str();
        case MOD_ASSIGN:    o << "OPERATOR  " << "%="  ; return o.str();
        case GTEQ:          o << "OPERATOR  " << ">="  ; return o.str();
        case LSHIFT:        o << "OPERATOR  " << "<<"  ; return o.str();
        case LSHIFT_ASSIGN: o << "OPERATOR  " << "<<=" ; return o.str();
        case SUB:           o << "OPERATOR  " << "-"   ; return o.str();
        case MUL:           o << "OPERATOR  " << "*"   ; return o.str();
        case AND:           o << "OPERATOR  " << "&"   ; return o.str();
        default:            assert(!"UNHANDLED TOKEN!"); return o.str();
    }
}
 
void testlex()
{
    string tests[] = {
        "12 + 24",
        "0xFF",
        "0123",
        "0b1010",
        "i32 a = -34",
        "a += 34",
        "print(player.position)",
        "f32 result = add(1, 2)",
        "i64 big = 10_000_000",
        "string quote = \"Mim said \\\"holy shit, it works!\\\", Greg was not amused.\"",
        "string name = \"mim\"",
        "if a >= b return a else return b",
        "test(a(), b)",
        "a %= 5",
        "i32* pi = &a",
        "f32 pi = 3.141",
        "f32 angle = 0.01",
        "log <<= 4",
        "log = 1 << 4",
    };
 
    cout << "=LEX================\n";
    cout << "                    \n";

    for (auto test : tests)
    {
        cout << test << "            \n";
        cout << "--------------------\n";
        for (auto token : lex(test))
            cout << debugprint(token) << "\n";
        cout << "                    \n";
    }
}