#ifndef GRAMMAR_H
#define GRAMMAR_H
#include<iostream>
using namespace std;
class node;//declaration..defintion later
class Grammar
{
	public:
		Grammar();
		void setCountTerminals(int);
		void setCountNonTerminals(int);
		void setProduction(int, string);
		bool takeInputFromFile();
		void printGrammar();
		char* getFirst(int);
		char* getFollow(int);
		char* getFirstFromString(string);
		bool ifEpsilon(int);
		void constructParsingTable();
		void printParsingTable();
		void parse(string);
	private:
		node* array;
		int countTerminals;
		int countNonTerminals;
		int charToInt[256];//same array for both terminals and non terminals
		char* intToCharTerminals;//size would be determined at run time
		char* intToCharNonTerminals;
		char* parsingTable[10][10];//maximum 10X10 size
};
#endif
