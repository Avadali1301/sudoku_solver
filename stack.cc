#include <iostream>
#include <cstdlib>
#include "stack.h"

using namespace std;
stack::stack() {
	// starting with an empty stack
	_size = 0;
	_top = NULL;
}
stack::~stack() {
}
// pushes an element to the stop of the stack
// no capacity, meaning we can push any non-negative integral number of elements to the stack
void stack::push(stackData d) {
	// create the new node that will be the top
	node * n = new node;
	// set the data to d
	n -> data = d;
	// I make n point to the previous top so that we don't lose the stack.
	n -> next = _top;
	// I then make the pointer top point to the new top (n).
	_top = n;
	// gotta increase size by 1
	_size += 1;
}
// removes an element from the top of the stack
void stack::pop() {
	if(!empty()) {
		// I'm shifting the top down by one
		node * temp = _top;
		// gotta use the temp do get rid of the excess node
		delete temp;
		_top = _top -> next;
		// decrement size
		_size -= 1;
	}
}
// returns the data of the top element of the stack
stackData stack::top() const {
	return _top -> data; // top is a pointer so just getting the data top points to
}
// returns the size of the stack
size_t stack::size() const {
	return _size;
}
// returns true if the stack is empty, false otherwise
bool stack::empty() const {
	return _size == 0;
}
// prints out the stack
void stack::print() const {
	node * n = _top;
	cout << "[";
	while(n != NULL) {
		// this is going to be from the top down
		cout << n -> data;
		if(!(n -> next == NULL)) {
			cout << ", ";
		}
		// I'm moving n down the stack without actually losing any of the stack
		n = n -> next;
	}
	cout << "]";
}