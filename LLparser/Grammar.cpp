#include "Grammar.h"
#include<fstream>
#include<iomanip>
#include "Stack.h"
enum Type{TERMINAL, NONTERMINAL, EPSILON, SEPARATOR};

class node
{
	public:
		node* next;
		char value;
		Type type;
};
	
Grammar::Grammar()
{
	array= NULL;
}

void Grammar::setCountTerminals(int count)
{
	countTerminals= count;
	intToCharTerminals= new char[countTerminals];
}

void Grammar::setCountNonTerminals(int count)
{
	countNonTerminals= count;
	intToCharNonTerminals= new char[countNonTerminals];
	array= new node[countNonTerminals];
}

void Grammar::setProduction(int nonTerminal, string str)
{
	node* currentIndex= array+ nonTerminal;//to get to the right position in array
	node* temp;
	node* ptr= currentIndex;//will store pointer to last entry in production
	ptr->type= NONTERMINAL;
	ptr->value= intToCharNonTerminals[nonTerminal];
	for(int i=0; i<str.length(); i++)
	{
		temp= new node;
		if(str[i]>= 'a' && str[i]<= 'z')
			temp->type= TERMINAL;
		else if(str[i]>= 'A' && str[i]<= 'Z')
			temp->type= NONTERMINAL;
		else if(str[i]== '~')
			temp->type= EPSILON;
		else//when str[i]== '|'
			temp->type= SEPARATOR;//it will also include absilon and or production separator

		temp->value= str[i];
		temp->next= NULL;
		ptr->next= temp;
		ptr= ptr->next;
	}
}

bool Grammar::takeInputFromFile()
{
	int temp;
	string s;	
	ifstream file("inputs.txt");
	file>>temp;
	setCountNonTerminals(temp);
	file>>temp;
	setCountTerminals(temp);
	file>>s;
	for( int i=0; i<countNonTerminals; i++)
	{
		charToInt[(int)s[i]]= i;
		intToCharNonTerminals[i]= s[i];
	}

	file>>s;
	for( int i=0; i<countTerminals; i++)
	{
		charToInt[(int)s[i]]= i;
		intToCharTerminals[i]= s[i];
	}

	for(int i=0; i<countNonTerminals; i++)
	{
		file>>s;
		setProduction(i, s);
	}
	file.close();
	return true;
}

bool Grammar::ifEpsilon(int nonTerminal)
{
	char* buffer= getFirst(nonTerminal);
	while(*buffer!= 0)
	{
		if(*buffer== '~')
			return true;
		buffer++;
	}
	
	return false;
}

char* Grammar::getFirst(int nonTerminal)
{

	node* ptr= array+ nonTerminal;//to traverse the production

	char* output= new char[3*countTerminals];//maximum sized array
	for(int i=0; i<3*countTerminals; i++)//for the sake of proper initialization
		output[i]= 0;//it is little wrong since character can't be assigned null value but it 0 i guess
	int count= 0;//will keep the index where next terminal is to be added

	//first of all we have to skip the non terminal itself
	ptr= ptr->next;
	//now our pointer is at the first element in right hand side of production
	while(ptr!= NULL)
	{
		if(ptr->type== TERMINAL)//we have found one element of first set
		{
			output[count]=  ptr->value;
			count++;
			ptr= ptr->next;
			//now skip till we get a separator
			while(true)
			{
				if(ptr== NULL)
				{
					return output;
				}

				else if(ptr->type== SEPARATOR)
				{
					break;
				}
				else//it cannot be epsilon because epsilon can only be found after separator
					ptr= ptr->next;
			}
		}

		else if(ptr->type== NONTERMINAL)
		{
			while(ptr->type!= SEPARATOR)//for the complete production
			{
				//copying first set of this non terminal into our output
				char* temp= getFirst(charToInt[(int)ptr->value]);
				while(*temp!= 0)
				{
					if(*temp== '~')
					{
						temp++;
						continue;
					}
					output[count]= *temp;
					count++;
					temp++;
				}

				if(!ifEpsilon(charToInt[(int)ptr->value]))//ie first set of non terminal does not have epsilon
				{
					ptr= ptr->next;
					//we just have to go to sperator
					while(true)
					{
						if(ptr== NULL)
							return output;
						if(ptr->type== SEPARATOR)
							break;//it will automatically teminate from other while loop because
								//ptr->type== SEPARATOR would be false
						else
							ptr= ptr->next;//iterate
					}
				}
				
				else
				{	//two cases here: either there is a terminal or a non terminal
					ptr= ptr->next; //next symbol of producion after non terminal having epsilon in first set
					if(ptr== NULL)
					{
						output[count]='~';
						count++;
						return output;
					}
					if(ptr->type== TERMINAL)
					{
						output[count]= ptr->value;
						count++;
						while(true)//go to separator
						{
							if(ptr== NULL)
								return output;
							if(ptr->type== SEPARATOR)
								break;
							else
								ptr= ptr->next;
						}
					}
				}
			}
		}
		else if(ptr->type== SEPARATOR)
		{
			ptr= ptr->next;
		}

		else //when ptr->type== EPSILON
		{
			output[count]= ptr->value;
			count++;
			ptr= ptr->next;
		}
	}
	
	return output;
}

char* Grammar::getFollow(int nonTerminal)
{
	int count= 0;
	char* output= new char[2*countTerminals];

	for(int i=0; i<2*countTerminals; i++)
		output[i]= 0;
	if(nonTerminal== 0)//ie it is start symbol
	{
		output[count]= '$';
		count++;
	}
	
	for(int i=0; i<countNonTerminals; i++)
	{
		node* temp= array+ i;
		temp= temp->next;//we have to start with right hand side of production
		
		while(true)
		{
			if(temp== NULL)
				break;
			else if(temp->value== intToCharNonTerminals[nonTerminal])
			{
				if(temp->next== NULL)//ie it is last
				{
					if(i== nonTerminal)
						break;
					char* ptr= getFollow(i);
					bool flag1= 0;
					while(*ptr!= 0)
					{
						flag1= 0;
						for(int j=0; j<count; j++)
						{
							if(output[j]== *ptr)
							{
								//do not make entry
								ptr++;
								flag1= 1;
								break;
							}
						}
						if(flag1)
							continue;		
						output[count]= *ptr;
						count++;
						ptr++;
					}
					break;//go check for some other productions
				}//short and simple

				else if(temp->next->type== TERMINAL)//simply add that terminal to follow
				{
					bool flag2= 0;
					for(int j=0; j<count; j++)
					{
						flag2= 0;
						if(output[j]== temp->next->value)
						{
							flag2= 1;
							break;
						}
					}
					if(!flag2)
					{
						output[count]= temp->next->value;
						count++;
					}
					temp= temp->next;//ie at the terminal next to non terminal
					temp= temp->next;//next unexplored node
				}

				else if(temp->next->type== SEPARATOR)
				{
					if(i== nonTerminal)
					{
						temp= temp->next;
						temp= temp->next;
					}
					else
					{
					//same as case of temp->next==NULL
					char* ptr= getFollow(i);
					bool flag3= 0;
					while(*ptr!= 0)
					{
						flag3= 0;
						for(int j=0; j<count; j++)
						{
							if(output[j]== *ptr)
							{
								flag3= 1;
								ptr++;
								break;
							}
						}
						if(flag3)
							continue;
						output[count]= *ptr;
						count++;
						ptr++;
					}
					
					temp= temp->next;//at separator
					temp= temp->next;//at next symbol
					}
				}

				//now only case remaining is temp->next->type== NONTERMINAL
				else
				{
					temp= temp->next;//getting to next non terminal
					bool flag= 0;//will keep record when first of a non terminal contains absilon so we have to add next non terminals first set also
					while(temp->type== NONTERMINAL)
					{
						flag= 0;
						while(temp!= NULL)
						{
							if(temp->value!= intToCharNonTerminals[nonTerminal])//consecutive non terminals
								break;
							temp= temp->next;
						}
						
						if(temp== NULL)
							break;
						//we have to add that non terminal's first set
						char* ptr= getFirst(charToInt[(int)temp->value]);
						bool flag4= 0;
						while(*ptr!= 0)
						{
							flag4= 0;
							if(*ptr== '~')
							{
								flag= 1;
								ptr++;
								continue;
							}
							for(int j=0; j<count; j++)
							{
								if(output[j]==*ptr)
								{
									flag4= 1;
									ptr++;
									break;
								}
							}
							if(flag4)
								continue;
							output[count]= *ptr;
							count++;
							ptr++;
						}

						if(flag== 0)
							break;
				
						temp= temp->next;
						if(temp== NULL)//doubt on second condition
						{
							if(i== nonTerminal)
								break;
							//now we can add follow of i to it
							char* ptr= getFollow(i);
							bool flag5= 0;
							while(*ptr!= 0)
							{
								flag5= 0;
								//cout<<"5= "<<*ptr<<endl;
								for(int j=0; j<count; j++)
								{
									if(output[j]== *ptr)
									{
										flag5= 1;
										ptr++;
										break;
									}
								}
								if(flag5)
									continue;
								output[count]= *ptr;
								count++;
								ptr++;
							}
							
							break;
						}
						else if(temp->type== SEPARATOR)
						{
							if(i== nonTerminal)
								break;
							char* ptr= getFollow(i);
							bool flag6= 0;
							while(*ptr!= 0)
							{
								flag6= 0;
								for(int j=0; j<count; j++)
								{
									if(output[j]== *ptr)
									{
										flag6= 1;
										ptr++;
										break;
									}
								}
								if(flag6)
									continue;
								output[count]= *ptr;
								count++;
								ptr++;
							}
							break;
						}
					}//this loop will continue while flag== 1 and non terminals are found
				}
			}
			else//when the next non symbol is not equal to the non terminal we are looking for
				temp= temp->next;
		}//while loop ends
	}

	return output;
}

char* Grammar::getFirstFromString(string str)
{
	if(str[0]== '~')
	{	char out= '~';
		char* ptr= &out;
		return ptr;
	}

	char* output= new char[10];//assuming maximum 10 elements in first set
	int count= 0;
	for(int i=0; i<10; i++)
		output[i]= 0;
	
	bool flag= 0;
	bool flag2= 0;
	for(int i=0; i<str.length(); i++)
	{
		if(str[i]>= 'a' && str[i]<= 'z')//ie its a terminal
		{
			flag= 0;
			for(int j=0; j<count; j++)
			{
				if(output[j]== str[i])
				{
					flag= 1;
					break;
				}
			}
			if(!flag)
			{
				output[count]= str[i];
				count++;
			}
			return output;
		}

		else if(str[i]>= 'A' && str[i]<= 'Z')
		{
			flag= 0;
			flag2= 0;
			char* temp= getFirst(charToInt[(int)str[i]]);
			while(*temp!= 0)
			{
				flag= 0;
				if(*temp== '~')
				{
					flag2= 1;
					temp++;
				}
				for(int j=0; j<count; j++)
				{
					if(output[j]== *temp)
					{
						flag= 1;
						temp++;
						break;
					}
				}
				if(flag|| flag2)
					continue;
				output[count]= *temp;
				count++;
				temp++;
			}

			if(!flag2)
				return output;
		}
	}
	return output;
}

void Grammar::constructParsingTable()
{
	for(int i=0; i<countNonTerminals; i++)
	{
		for(int j=0; j<countTerminals+ 1; j++)
		{
			parsingTable[i][j]= NULL;//initialization
		}
	}

	for(int i=0; i<countNonTerminals; i++)
	{
		char* str= new char[10];//maximum 10 sized production
		int count= 0;
		for(int j=0; j<10; j++)
			str[j]= 0;//proper initialization

		//now we have to fill str array
		node* ptr= array+ i;
		ptr= ptr->next;
		while(true)//this is for traversal of complete right hand side
		{
			if(ptr== NULL || (ptr!= NULL && ptr->type== SEPARATOR))
			{
				//we have make entry for the given string
				char* first= getFirstFromString(str);
				bool flag= 0;
				while(*first!= 0)
				{
					if(*first== '~')
					{
						flag= 1;
						first++;
						continue;
					}

					parsingTable[i][charToInt[(int)*first]]= str;
					first++;
				}
				if(flag)//it means there is absilon in first(alpha) there we have add the production to follow of A
				{
					char* follow= getFollow(i);
					while(*follow!= 0)
					{
						if(*follow== '$')
						{
							parsingTable[i][countTerminals]= str;
							follow++;
							continue;
						}
						parsingTable[i][charToInt[(int)*follow]]= str;
						follow++;
					}
				}
				if(ptr== NULL)
					break;//as we have to now go for next non terminal

				//execution comes here if ptr->type== SEPARATOR
				str= new char[10];
				count= 0;
				for(int j=0; j<10; j++)
					str[j]= 0;
				ptr= ptr->next;
			}
			
			else
			{
				str[count]= ptr->value;
				count++;
				ptr= ptr->next;
			}
		}
	}
}

void Grammar::printGrammar()
{
	cout<<"Total no. of non terminals= "<<countNonTerminals<<endl;
	cout<<"Total no. of terminals= "<<countTerminals<<endl;
	cout<<"Non terminals are: ";
	for(int i=0; i<countNonTerminals; i++)
		cout<<intToCharNonTerminals[i]<<"  ";
	cout<<endl;
	cout<<"Terminals are: ";
	for( int i=0; i<countTerminals; i++)
		cout<<intToCharTerminals[i]<<"  ";
	cout<<endl;

	cout<<"GRAMMAR\n";
	for(int i=0; i<countNonTerminals; i++)
	{
		node* ptr= array+ i;
		cout<<ptr->value<<"-->";
		ptr= ptr->next;
		while(ptr!= NULL)
		{
			cout<<ptr->value;
			ptr= ptr->next;
		}
		cout<<endl;
	}
	for(int i=0; i<countNonTerminals; i++)
	{
		char* temp= getFirst(i);
		cout<<"FIRST("<<intToCharNonTerminals[i]<<")= {";
		while(*temp!= 0)
		{
			cout<<*temp<<" ";
			temp++;
		}
		cout<<"}\n";
	}

	for(int i=0; i<countNonTerminals; i++)
	{
		char* temp= getFollow(i);
		cout<<"FOLLOW("<<intToCharNonTerminals[i]<<")= {";
		while(*temp!= 0)
		{
			cout<<*temp<<" ";
			temp++;
		}
		cout<<"}\n";
	}
}

void Grammar::printParsingTable()
{
	char* ptr;
	cout<<setw(8);
	for(int i=0; i<countTerminals; i++)
	cout<<setw(8)<<intToCharTerminals[i];
	cout<<setw(8)<<"$\n";
	for(int i=0; i<countNonTerminals; i++)
	{
		cout<<intToCharNonTerminals[i];
		for(int j=0; j<countTerminals+ 1; j++)
		{
			if(parsingTable[i][j]!= NULL)
			{
				ptr= parsingTable[i][j];
				cout<<setw(8);
				while(*ptr!= 0)
				{
					cout<<*ptr;
					ptr++;
				}
			}
			else
			{
				cout<<setw(8)<<"error";
			}
		}
		cout<<endl;
	}
}

void Grammar::parse(string str)
{
	Stack s;
	s.push('$');
	s.push(intToCharNonTerminals[0]);//start symbol
	for(int i=0; i<str.length(); i++)
	{
		cout<<"input string i= "<<i<<endl;
		s.print();
		if(s.ifEmpty())
		{
			cout<<"error\n";
			return;
		}
		else if(s.getTop()>= 'a' && s.getTop()<= 'z')//ie its a terminal
		{
			cout<<"top= "<<s.getTop()<<" symbol= "<<str[i]<<endl;
			if(s.getTop()== str[i])
			{
				cout<<"popping1= "<<s.pop()<<endl;
			}
			else
			{
				cout<<"error\n";
				return;
			}
		}
		else if(str[i]== '$' && s.getTop()== '$')
		{
			cout<<"parsed\n";
			return;
		}
		else if(str[i]== '$')
		{
			if(parsingTable[charToInt[s.getTop()]][countTerminals]== NULL)
			{
				cout<<"error\n";
				return;
			}
			else
			{
				char* temp= parsingTable[charToInt[s.getTop()]][countTerminals];
				if(*temp== '~')
				{
					cout<<"popping2 =";
					cout<<s.pop()<<endl;
				}
				else
				{
					Stack stackTemp;
					while(*temp!= 0)
					{
						stackTemp.push(*temp);
					}
					while(!stackTemp.ifEmpty())
					{
						s.push(stackTemp.pop());
					}
				}
			}
			i--;
		}
		else if(s.getTop()== '$' && str[i]!= '$')
		{
			cout<<"error1\n";
			return;
		}
		else if(parsingTable[charToInt[(int)s.getTop()]][charToInt[(int)str[i]]]== NULL)
		{
			cout<<"error2\n";
			return;
		}
		else
		{
			char* temp= parsingTable[charToInt[(int)s.pop()]][charToInt[(int)str[i]]];
			if(*temp== '~')
			{cout<<"Popping3= ";
			}
			else
			{
			Stack tempStack;
			while(*temp!= 0)
			{
				tempStack.push(*temp);
				temp++;
			}
			while(!tempStack.ifEmpty())
			{
				s.push(tempStack.pop());
			}
			}
			i--;
		}
	}
}
