#include "Stack.h"
#include<iostream>
using namespace std;
class block
{
	public:
		char val;
		block* next;
};

Stack::Stack()
{
	top= NULL;
}

Stack::~Stack()
{
}

void Stack::push(char c)
{
	block* temp= new block;
	temp->val= c;
	temp->next= NULL;

	if(top== NULL)
	{
		top= temp;
	}
	else
	{
		temp->next= top;
		top= temp;
	}
}

char Stack::pop()
{
	if(top== NULL)
	{
		cout<<"no elements to pop\n";
		return 0;
	}

	else
	{
		char c= top->val;
		top= top->next;
		return c;
	}
}

bool Stack::ifEmpty()
{
	if(top== NULL)
		return true;
	return false;
}

void Stack::print()
{
	cout<<"STACK:\n";
	block* temp= top;
	while(temp!= NULL)
	{
		cout<<temp->val<<endl;
		temp= temp->next;
	}
}

char Stack::getTop()
{
	if(top!= NULL)
		return top->val;
	else
		return 0;
}
