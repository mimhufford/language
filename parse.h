#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <cassert>

#include "types.h"
#include "common.h"
 
Decl* ParseLiteral(std::queue<Token> tokens)
{
    Decl* d     = new Decl;
    d->type     = LITERAL;
    d->datatype = I64;
    d->val      = &tokens.front().ival;
    tokens.pop();
    return d;
}

Decl* ParseVar(std::queue<Token> tokens)
{
    Decl* d     = new Decl;
    d->type     = VARIABLE;
    d->datatype = I32;                     tokens.pop();
    d->val      = &tokens.front().sval;    tokens.pop();
    assert(tokens.front().type == ASSIGN); tokens.pop();
    d->expr     = ParseLiteral(tokens);
    return d;
}

Decl* Parse(std::queue<Token> tokens)
{
    if (tokens.empty()) return nullptr;

    if (tokens.front().type <= BEGIN_KEYWORD || tokens.front().type >= END_KEYWORD) return nullptr;

    if (tokens.front().sval == "i32") return ParseVar(tokens);

    return nullptr;
} 