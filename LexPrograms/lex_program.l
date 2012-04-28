/*
Program to recognize keywords, identifiers, numbers and relational operators
By- 
Chirag Varde
chirag.varde.crg@gmail.com
*/

%{
#include<stdlib.h>
#include<string.h>
#define IF		1
#define THEN		2
#define ELSE		3
#define ID		4
#define NUMBER		5
#define RELOP		6
#define LT		7
#define LE		8
#define EQ		9
#define NE		10
#define GT		11
#define GE		12
#define MAXINDEX	10
#define MAXLEN 		10
int numArray[MAXINDEX];
char idArray[MAXINDEX][MAXLEN];
int num_index=	0;
int id_index= 	0;
int relop;
void installID();
void installNum();
%}

delim	[ \t\n]
ws	{delim}+
letter	[A-Za-z]
digit	[0-9]
id	{letter}({letter}|{digit})*
number	{digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%

{ws}		{/*no action and no return*/}
if		{return (IF);}
then		{return (THEN);}
else		{return (ELSE);}
{id}		{installID(); return (ID);}
{number}	{installNum(); return (NUMBER);}
"<"		{relop= LT; return (RELOP);}
"<="		{relop= LE; return (RELOP);}
"="		{relop= EQ; return (RELOP);}
"<>"		{relop= NE; return (RELOP);}
">"		{relop= GT; return (RELOP);}
">="		{relop= GE; return (RELOP);}

%%

void installID()
{
	printf("Token= ID\n");
	//check for existing id
	int i=0;
	while(i< id_index)
	//for(int i=0; i<id_index; i++)
	{
		if(!strcmp(yytext, &idArray[i][0]))
		{
			printf("identifier already exists in id table at index %d\n", i);
			return;
		}
		i++;
	}

	if(id_index== MAXINDEX)
	{
		printf("No space in id table\n");
		return;
	}

	strcpy(idArray[id_index], yytext);
	printf("New id added in id table at %d\n", id_index);
	id_index++; 
}

void installNum()
{
	printf("Token= NUMBER\n");
	int num= atoi(yytext);
	//check for existing number
	int i=0;
	while(i<num_index)
	//for(int i=0; i<num_index; i++)
	{
		if(numArray[i]== num)
		{
			printf("Num already in num table, index= %d\n", i);
			return;
		}
		i++;
	}

	if(num_index== MAXINDEX)
	{
		printf("Error! no space in num table\n");
		return;
	}

	//entering the number in num table
	numArray[num_index]= num;
	printf("new num added in num table, index= %d\n", num_index);
	num_index++;
}

int main()
{
	int token;	
	while(token= yylex())
	{
		switch(token)
		{
			case IF:
			printf("Token= IF\n");
			break;
			case THEN:
			printf("Token= THEN\n");
			break;
			case ELSE:
			printf("Token= ELSE\n");
			break;
			case ID:
			break;
			case NUMBER:
			break;
			case RELOP:
				printf("Token= RELOP  ");
				switch(relop)
				{
					case LT:printf("LT\n");break;
					case LE:printf("LE\n");break;
					case EQ:printf("EQ\n");break;
					case NE:printf("NE\n");break;
					case GT:printf("GT\n");break;
					case GE:printf("GE\n");break;
					default:break;
				}
				break;
			default:break;
		}
	}

	return 0;
}




