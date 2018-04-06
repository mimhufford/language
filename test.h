#pragma once

#include <iostream>
#include <string>

#include "common.h"
#include "types.h"
#include "lex.h"
#include "parse.h"

std::string DebugPrintDecl(Decl* d)
{
    std::ostringstream o;

	if (!d)
	{
		o << "DID NOT PARSE";
		return o.str();
	}

	switch (d->type)
	{
		case VARIABLE: o << "(VAR "  << d->name << " " << DebugPrintDecl(d->expr) << ")"; break;
		case LITERAL:  o << "(LIT "  << d->ival << ")"; break;
		case FUNCTION: o << "(FUNC " << d->name << "(PARAMS) (RET) " << "BLOCK" << ")"; break;
		default:       o << "UNHANDLED"; break;
	}

    return o.str();
}

void TestParse()
{
    std::string tests[] = {
        "var a = 4",
        "func test() { {} var a = 15 }",
    };

    std::cout << "=PARSE==============\n";
    std::cout << "                    \n";

    for (auto test : tests)
    {
        std::cout << test << "            \n";
        std::cout << "--------------------\n";
        std::cout << DebugPrintDecl(Parse(Lex(test))) << "\n\n";
    }
    std::cout << "                    \n";
}

std::string DebugPrintToken(Token t)
{
    std::ostringstream o;
    switch (t.type)
    {
        case DEC:           o << "INT DEC   " << t.ival; return o.str();
        case BIN:           o << "INT BIN   " << t.ival; return o.str();
        case OCT:           o << "INT OCT   " << t.ival; return o.str();
        case HEX:           o << "INT HEX   " << t.ival; return o.str();
        case CHR:           o << "INT CHR   " << t.ival; return o.str();
        case FLOAT:         o << "FLOAT     " << t.fval; return o.str();
        case STRING:        o << "STRING    " << t.sval; return o.str();
        case IDENTIFIER:    o << "IDENTFIER " << t.sval; return o.str();
        case VAR:           o << "VAR       " << " "   ; return o.str();
        case COMMA:         o << "COMMA     " << ","   ; return o.str();
        case IF:            o << "IF        " << " "   ; return o.str();
        case ELSE:          o << "ELSE      " << " "   ; return o.str();
        case RETURN:        o << "RETURN    " << " "   ; return o.str();
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
void TestLex()
{
    std::string tests[] = {
        "12 + 24",
        "0xFF",
        "0123",
        "0b1010",
        "var a = -34",
        "a += 34",
        "print(player.position)",
        "var result = add(1, 2)",
        "var big = 10_000_000",
        "var quote = \"Mim said \\\"holy shit, it works!\\\", Greg was not amused.\"",
        "var name = \"mim\"",
        "if a >= b return a else return b",
        "test(a(), b)",
        "a %= 5",
        "var pi = &a",
        "var pi = 3.141",
        "var angle = 0.01",
        "log <<= 4",
        "log = 1 << 4",
    };
 
    std::cout << "=LEX================\n";
    std::cout << "                    \n";

    for (auto test : tests)
    {
        std::cout << test << "            \n";
        std::cout << "--------------------\n";
        auto tokens = Lex(test);
        while (!tokens.empty())
        {
            std::cout << DebugPrintToken(tokens.front()) << "\n";
            tokens.pop();
        }
        std::cout << "                    \n";
    }
}