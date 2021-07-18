/*
Leah Wolf
stack.h
For use in all stack-related programs
September 2019
*/

#include <iostream>
#include <cstdlib>
using namespace std;

typedef int stackData;  //for easy switching of the type stored in our stack

class stack{
public:
	stack();				//Constructs an empty stack.
	~stack();				//Removes & deletes all remaining elements of the stack.

	void push(stackData d);  //Creates a node in the stack and places d as the data in it.
	void pop();				//Removes and deletes the top node from the stack.
	
	stackData top() const;	//Returns the data stored in the top node of the stack.
	size_t size() const;	//Returns the number of nodes in the stack.
	bool empty() const;		//Returns whether the stack is empty.
	void print() const;	//Prints out a visual representation of the stack.

private:
	struct node{			//Node structure with which to build the stack
		node * next;
		stackData data;
	};

	size_t _size;			//Holds number of nodes in the stack.
	node * _top;			//Pointer to the top node in the stack.

};