#pragma once

#include <sstream>
#include <string>
#include <iostream>

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
    long long    ival;
    double       fval;
    std::string  sval;
};

enum DeclType
{
    VAR, LITERAL
};

enum Type
{
    I8, I16, I32, I64, U8, U16, U32, U64, F32, F64,
};

struct Decl
{
    DeclType    type;
    Type        datatype;
    void*       val;
    Decl*       expr;
    Decl*       lhs;
    Decl*       rhs;
};