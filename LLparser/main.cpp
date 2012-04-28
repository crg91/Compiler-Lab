#include "Grammar.h"
int main()
{
	Grammar g;
	g.takeInputFromFile();
	g.printGrammar();
	g.constructParsingTable();
	g.printParsingTable();
	g.parse("eaceaebed$");
	return 0;
}
