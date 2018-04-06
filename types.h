#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <iostream>

enum TokenType
{
    COLON,
    LPAREN, RPAREN,
    LBRACE, RBRACE,
    LBRACKET, RBRACKET,
    COMMA,
    DOT,
    IDENTIFIER,
    FLOAT,
    STRING,

    BEGIN_INT,
    //-------------------
    BIN, OCT, DEC, HEX, CHR,
    //-------------------
    END_INT,

    BEGIN_KEYWORD,
    //-------------------
    TYPE, FUNC, CONST, IF, ELSE, VAR, LOOP, CONTINUE, BREAK, RETURN,
    //-------------------
    END_KEYWORD,    
 
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
 
struct Token
{
    TokenType    type;
    long long    ival;
    double       fval;
    std::string  sval;
};

enum DeclType
{
    VARIABLE, LITERAL, FUNCTION
};

enum Type
{
    I8, I16, I32, I64, U8, U16, U32, U64, F32, F64,
};

struct Decl
{
    DeclType           type;
    Type               datatype;
	std::string        name;
    Decl*              expr;
	long long          ival;
	std::vector<Decl*> block;
};