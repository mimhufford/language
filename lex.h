#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <cassert>

#include "types.h"
#include "common.h"

Token Float(std::istream& i, std::ostringstream& o)
{
    o << '.';
 
    Token t;
    t.type = FLOAT;
 
    while (isdigit(i.peek()) || i.peek() == '_')
    {
        char c = i.get();
        if (c == '_') continue;
        int digit = CharToDec(c);
        o << c;
    }
 
    t.fval = atof(o.str().c_str());
 
    return t;
}
 
Token Num(std::istream& i)
{
    Token t;
    t.type = INT;
    t.subtype = DEC;
    t.ival = 0;
    int base = 10;
    char first = i.get();
    std::ostringstream o;
    o << first;
 
    if (first == '0')
    {
        char next = tolower(i.peek());
 
        if      (next == '.')   { i.get(); return Float(i, o); }
        else if (next == 'x')   { t.subtype = HEX; base = 16; i.get(); }
        else if (next == 'b')   { t.subtype = BIN; base = 2;  i.get(); }
        else if (isdigit(next)) { t.subtype = OCT; base = 8;           }
    }
    else
    {
        t.ival = CharToDec(first);
    }
 
    while (isnumch(i.peek()))
    {
        char c = i.get();
        if (c == '_') continue;
        if (c == '.') return Float(i, o);
        int digit = CharToDec(c);
        assert(digit < base);
        t.ival = t.ival*base + digit;
        o << c;
    }
 
    return t;
}
 
Token Ident(std::istream& i)
{
    Token t;
    t.type = IDENTIFIER;
    t.sval = "";
 
    while (isalnum(i.peek()) || i.peek() == '_')
    {
        t.sval += (char)i.get();
    }
 
    std::vector<std::string> keywords = {
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
 
Token Quote(std::istream& i)
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
 
Token Op(std::istream& i)
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
 
Token Symbol(std::istream& i, TokenType type)
{
    Token t;
    t.type = type;
    i.get();
    return t;
}
 
std::queue<Token> Lex(std::string input)
{
    std::istringstream i(input);
    std::queue<Token> o;
 
    while (i.peek() != -1)
    {
        char c = i.peek();
 
        if      (isdigit(c)) { o.push(Num(i));              }
        else if (isalpha(c)) { o.push(Ident(i));            }
        else if (isquote(c)) { o.push(Quote(i));            }
        else if (isop(c))    { o.push(Op(i));               }
        else if (c == ')')   { o.push(Symbol(i, RPAREN));   }
        else if (c == '}')   { o.push(Symbol(i, RBRACE));   }
        else if (c == ']')   { o.push(Symbol(i, RBRACKET)); }
        else if (c == '(')   { o.push(Symbol(i, LPAREN));   }
        else if (c == '{')   { o.push(Symbol(i, LBRACE));   }
        else if (c == '[')   { o.push(Symbol(i, LBRACKET)); }
        else if (c == ',')   { o.push(Symbol(i, COMMA));    }
        else if (c == '.')   { o.push(Symbol(i, DOT));      }
        else if (isspace(c)) { i.get(); /* EAT IT */        }
        else                 { assert(!"UNHANDLED");        }
    }
 
    return o;
}