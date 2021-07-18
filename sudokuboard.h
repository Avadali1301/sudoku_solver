#include <iostream>
#include <cstdlib>
#include "stack.h"
using namespace std;

class sudokuboard {
	public:
		struct node {
			// these are the pointers that every node has
			// points to list head node that it belongs to
			node * column;
			// points to node left of current node
			node * left;
			// points to node right of current node
			node * right;
			// points to node above current node
			node * up;
			// points to node below current node
			node * down;

			// this is for header nodes.
			// keeps track of nodes in column
			int nCount;
			// keep track of column number
			int cNum;
			// keep track of row numbers
			int rNum;
			int value;
		};
		sudokuboard(string config);
		~sudokuboard();

		// Don't use any of this shit
		// void place(size_t r, size_t c, char n); // places a digit at coords
		// void remove(size_t r, size_t c); // removes digit from coords


		// void print() const; // prints the board
		// bool canPlace(size_t r, size_t c, char n) const; // sees if can place digit at coords
		// bool solved() const; // sees if board has any empty spaces
		// char get(size_t r, size_t c) const; // returns digit at coords

		// these are methods to get the next indices of the toroidal list
		int getUp(size_t index) const;

		int getDown(size_t index) const;

		int getLeft(size_t index) const;

		int getRight(size_t index) const;


		// this baby's going to create the toroidal ll 
		void makeToroidal();
		// this will remove 0's from the toroidall_ll
		// we are going to abuse the fact that L[R[x]] <- L[x], R[L[x]] <- R[x]
		// removes a node from a linked list. We can repeat this logic with 
		// the up and down pointers as well.
		void trimToroidal();
		// this will be used in Algorithm X to cover a node completely
		// that means to essentially unlink a node and its column and the rows
		// of the elements in the columns from the list
		void cover(node * n);
		// this will be used in Algorithm X to uncover a node completely
		// that means to relink a node's column that was previously covered
		void uncover(node * n);
		// this method is for deterministically selecting a column to cover
		node * smallestColumn();
		// this method will be the implementation of algorithm X
		// we're going to do this tail-recursively
		void aX(node * k);
		// this will create the sparse binary coverMatrix 
		void makeCover();
		// this will print out the solution board
		void printBoard();
		void knuthAx();
		void printStack();
		// gets the minimum element of the array
		int getMin();
	private:
		// I'm just going set _size = 9.
		// Using this struct to build toroidal linked list
		int _size;
		// string sboard[9];
		// int space_count;

		// number columns in coverMatrix
 		int columns;
 		// number rows in coverMatrix
 		int rows;
 		// head node.
 		// has pointer to list head node of first column
 		node * head;

 		// I'm going to be pushing the solution to the exact cover problem
 		// onto this stack
 		stack solutions;
 		// this holds the nodes I've already covered so that I can backtrack
 		stack history;
 		// holds toroidal linked list
 		// our exact cover matrix will have 9^3 rows because our sudoku is 9 x 9
 		// this means that we will have a row for every possible value for each cell
 		// we are going to have 9^2*4 columns because there are 4 constraints on sudoku
 		// Cell constraint: each cell can only hold 1 value [1, 9]
 		// Row constraint: each row can only have 1 occurence of [1, 9]
 		// Column constraint:each column can only have 1 occurence of [1, 9]
 		// Box constraint: each 3x3 box can only have 1 occurence of [1, 9]
 		node * toroidal_ll[9*9*9 + 1][9*9*4];
 		// We are going to transform our sudoku board into an exact cover problem
 		// we are going to store the exact cover board in this matrix.
 		// that's why it has the same dimensions as toroidal_ll.
 		// This cover matrix will be a sparse binary matrix
 		int coverMatrix[9*9*9][9*9*4];

 		// this holds the initial config of the board
 		// when I parse the input in sudoku.cc, I'll change underscores to 0's
 		string initSudoku;
 		// this is just going to hold the solutions while I sort them and then print them.
 		int answerArray[81];

};