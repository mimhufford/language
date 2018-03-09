#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void num(istream& i, ostream& o)
{
	while (isdigit(i.peek()) || i.peek() == '_')
	{
		char c = i.get();
		if (isdigit(c)) o << c;
	}
}

void ident(istream& i, ostream& o)
{
	while (isalnum(i.peek()) || i.peek() == '_')
	{
		char c = i.get();
		o << c;
	}
}

void quote(istream& i, ostream& o, char opener)
{
	while (i.peek() != opener)
	{
		char c = i.get();
		if (c == '\\' && i.peek() == opener) i.get(c); // dump the escape char
		o << c;
	}

	i.get(); // dump the end quote
}

bool    isop(char c) { return c == '+' || c == '/' || c == '*' || c == '-' || c == '=' || c == '<' || c == '>'; }
bool  isopen(char c) { return c == '(' || c == '{' || c == '['; }
bool isclose(char c) { return c == ')' || c == '}' || c == ']'; }
bool isquote(char c) { return c == '"' || c == '\''; }

string lex(string input)
{
	istringstream i(input);
	ostringstream o;
	char c;

	while (i.get(c))
	{
		if      (isdigit(c)) { o << "NUMBER " << c;   num(i, o)   ; o << "\n"; }
		else if (isalpha(c)) { o << "IDENT  " << c; ident(i, o)   ; o << "\n"; }
		else if (isquote(c)) { o << "STRING "     ; quote(i, o, c); o << "\n"; }
		else if (isop(c))    { o << "OP     " << c;                 o << "\n"; }
		else if (isopen(c))  { o << "OPEN   " << c;                 o << "\n"; }
		else if (isclose(c)) { o << "CLOSE  " << c;                 o << "\n"; }
		else if (c == ',')   { o << "COMMA  " << c;                 o << "\n"; }
		else if (isspace(c)) {                                                 }
		else                 { o << "?????? " << c;                 o << "\n"; }
	}

	return o.str();
}

int main(int argc, char* argv[])
{
	string tests[] = {
		   "12 + 24",
		   "int a = 34",
		   "go ()",
		   "float result = add(1, 2)",
		   "int big = 10_000_000",
		   "string quote = \"Mim said \\\"holy shit, it works!\\\", Greg was not amused.\"",
		   "string name = \"mim\"",
		   "if a >= b return a else return b",
	};

	for (auto test : tests)
	{
		cout << test << "     \n";
		cout << "-------------\n";
		cout << lex(test) << "\n";
	}

	char c; cin >> c;
}