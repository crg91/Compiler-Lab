#ifndef STACK_H
#define STACK_H

class block;//forward declaration

class Stack
{
	public:
		Stack();
		~Stack();
		void push(char);
		char pop();
		bool ifEmpty();
		void print();
		char getTop();
	private:
		block* top;
};

#endif
