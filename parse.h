#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <cassert>

#include "types.h"
#include "common.h"
 
Decl* ParseLiteral(std::queue<Token>& tokens)
{
    Decl* d     = new Decl;
    d->type     = LITERAL;
    d->ival     = tokens.front().ival;     tokens.pop();
    return d;
}

Decl* ParseVar(std::queue<Token>& tokens)
{
    Decl* d     = new Decl;
    d->type     = VARIABLE;                tokens.pop();
    d->name     = tokens.front().sval;     tokens.pop();
    assert(tokens.front().type == ASSIGN); tokens.pop();
    d->expr     = ParseLiteral(tokens);
    return d;
}

std::vector<Decl*> ParseBlock(std::queue<Token>& tokens)
{
	std::vector<Decl*> block;

	assert(tokens.front().type == LBRACE);

	tokens.pop();
	
	while (tokens.front().type != RBRACE)
	{
		if (tokens.front().type == LBRACE)
		{
			Decl* d = new Decl;
			d->block = ParseBlock(tokens);
			block.push_back(d);
		}
		else
		{
			tokens.pop();
		}
	}

	tokens.pop();

	return block;
}

Decl* ParseFunc(std::queue<Token>& tokens)
{
    Decl* d     = new Decl;
    d->type     = FUNCTION;                tokens.pop();
	d->name     = tokens.front().sval;     tokens.pop();
	assert(tokens.front().type == LPAREN); tokens.pop();
	// TODO: any parameters?
	assert(tokens.front().type == RPAREN); tokens.pop();
	// TODO: any return details?
	assert(tokens.front().type == LBRACE);
	d->block    = ParseBlock(tokens);
	return d;
}

Decl* Parse(std::queue<Token> tokens)
{
    if (tokens.empty()) return nullptr;

    if (tokens.front().type <= BEGIN_KEYWORD || tokens.front().type >= END_KEYWORD) return nullptr;

    if (tokens.front().type == VAR)  return ParseVar(tokens);
	if (tokens.front().type == FUNC) return ParseFunc(tokens);

    return nullptr;
} 