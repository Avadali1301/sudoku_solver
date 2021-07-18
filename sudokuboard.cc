#include <iostream>
#include <cstdlib>
#include <cmath>
#include "sudokuboard.h"
using namespace std;

// NOTE: One place I could make this a bit faster is if I created toroidal_ll in 1 go
// rather than creating and then trimming it.
sudokuboard::sudokuboard(string config) {
	// I'm just doing this for the 9 x 9 case
	_size = 9;
	columns = 9*9*4;
	rows = 9*9*9;
	// gotta initialize the nodes in my ll and head
	head = new node;
	for(int i = 0; i < rows + 1; i++) {
		for(int j = 0; j < columns; j++) {
			// if(i < rows) {
			// 	coverMatrix[i][j] = 1;
			// }
			// if(i < rows && i % 2 == 0)  {
			// 	coverMatrix[i][0] = 0;
			// }
			// if(i < rows && i % 2 == 1) {
			// 	coverMatrix[i][1] = 0;
			// }
			toroidal_ll[i][j] = new node;
			toroidal_ll[i][j] -> rNum = i;
			toroidal_ll[i][j] -> cNum = j;
		}
	}
	initSudoku = config;
	// for(int i = 0; i < columns; i++) {
	// 	if(i % 3 == 0) {
	// 		coverMatrix[25][i] = 1;
	// 	}
	// 	else {
	// 		coverMatrix[25][i] = 0;
	// 	}
	// }
	// for(int i = 0; i < columns; i++) {
	// 	if(i % 3 == 1) {
	// 		coverMatrix[69][i] = 1;
	// 	}
	// 	else {
	// 		coverMatrix[69][i] = 0;
	// 	}
	// }
	// for(int i = 0; i < columns; i++) {
	// 	if(i % 3 == 2) {
	// 		coverMatrix[93][i] = 1;
	// 	}
	// 	else {
	// 		coverMatrix[93][i] = 0;
	// 	}
	// }
	
	// coverMatrix[0][0] = 1;
	// //coverMatrix[0][2] = 1;
	// coverMatrix[0][3] = 1;
	// coverMatrix[1][0] = 1;
	// coverMatrix[1][1] = 1;
	// coverMatrix[1][2] = 1;
	// coverMatrix[2][0] = 1;
	// coverMatrix[3][1] = 1;
	// coverMatrix[3][2] = 1;
	// coverMatrix[3][3] = 1;
	// cout << initSudoku;
}
sudokuboard::~sudokuboard() {

}
// gets index above
int sudokuboard::getUp(size_t index) const{
	if(index == 0) {
		return rows;
	}
	else {
		return index - 1;
	}
}
int sudokuboard::getDown(size_t index) const{
	// return next index below, or just looping back to top
	// skip if here because you won't get negative index
	return (index + 1) % (rows + 1);
}
int sudokuboard::getLeft(size_t index) const{
	// you need to check if you're at the edge of the list
	if (index == 0) {
		// if you are, loop back around
		return columns - 1;
	}
	else {
		// else, just return 1 index to the left
		return index - 1;
	}
}
int sudokuboard::getRight(size_t index) const{
	// you're returning either the next right index or, since the ll is toroidal,
	// you're looping around to 0.
	// you can skip the if statement here because you won't end up with a negative
	// index
	return (index + 1) % columns;
}
// this method creates the cover matrix
// we'll be working with a sparse binary matrix
void sudokuboard::makeCover() {
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < columns; j++) {
			// this right here checks if the current row has a hint given to us
			// if it does, we can just make it 0 unless it's the exact row with the hint
			// this forces the program to choose the hint for that space
			if(initSudoku[i/_size] != '_' && (initSudoku[i/_size] -'0' != i % _size + 1)) {
				coverMatrix[i][j] = 0;
			}
			else {
				// for each of these conditions we only have to set upper bounds
				// because we use else-if rather than just repeated if-statements

				// it will be in the cell section of columns
				if(j < (_size)*(_size)) {
					// this checks if the current cell should be a 1
					if(j == (i / _size)) {
						coverMatrix[i][j] = 1;
					}
					// otherwise I just set it equal to 0
					else {
						coverMatrix[i][j] = 0;
					}
				}
				// it will be in the row section of column
				else if(j < 2*(_size)*(_size)) {
					// checks if current cell should be 1
					if(j == ((_size)*(_size) + (i % _size) + _size*(i / (_size*_size)))){
						coverMatrix[i][j] = 1;
					}
					// otherwise set it to 0
					else {
						coverMatrix[i][j] = 0;
					}
				}
				// it will be in the column section of columns
				else if(j < 3*(_size)*(_size)) {
					// checks if current cell should be 1
					if(j == (2*(_size)*(_size) + i % (_size*_size))) {
						coverMatrix[i][j] = 1;
					}
					// otherwise set to 0
					else {
						coverMatrix[i][j] = 0;
					}
				}
				// it will be in the box section of columns
				else if(j < 4*(_size)*(_size)) {
					// checks if current cell should be 1
					if(j == (3*(_size)*(_size) + i % _size + _size*(i/27 % 3) + 3*_size*(i/(3*81)))) {
						coverMatrix[i][j] = 1;
					}
					// otherwise 0
					else {
						coverMatrix[i][j] = 0;
					}
				}
			}
		}
	}
}
// this method creates a toroidal_ll but includes 0's as nodes as well.
void sudokuboard::makeToroidal() {
	for(int i = 0; i < rows + 1; i++) {
		for(int j = 0; j < columns; j++) {
			// Setting pointers of nodes to next nodes
			toroidal_ll[i][j] -> left = toroidal_ll[i][getLeft(j)];
			toroidal_ll[i][j] -> right = toroidal_ll[i][getRight(j)];
			toroidal_ll[i][j] -> up = toroidal_ll[getUp(i)][j];
			toroidal_ll[i][j] -> down = toroidal_ll[getDown(i)][j];
			// keeping track of column node is in
			toroidal_ll[i][j] -> column = toroidal_ll[0][j];
			// since toroidal_ll has 1 more row than coverMatrix due to 
			// containing list heads in columns, need to account for this
			if (i > 0) {
				// storing value of each node
				toroidal_ll[i][j] -> value = coverMatrix[i - 1][j];
				// I want to track how many nodes are in each column 
				toroidal_ll[0][j] -> nCount += 1;
			}
		}
	}
	// gotta setup the head node.
	head -> right = toroidal_ll[0][0];
	toroidal_ll[0][0] -> left = head;
	head -> left = toroidal_ll[0][columns - 1];
	toroidal_ll[0][columns - 1] -> right = head;
}
// this method trims the 0's from the toroidal_ll as we don't need them
// thus we can save time and space
void sudokuboard::trimToroidal() {
	// starting at i = 1 because I'm not going to remove any list heads
	for(int i = 1; i < rows + 1; i++) {
		for(int j = 0; j < columns; j++) {
			// if the value is 0, we don't need it in toroidal_ll
			if(toroidal_ll[i][j] -> value == 0) {
				// whatever node's left pointer was pointing to toroidal_ll[i][j]
				// is now pointing to the left of toroidal[i][j]
				toroidal_ll[i][j] -> right -> left = toroidal_ll[i][j] -> left;
				// same deal as before, now just for the right pointer
				toroidal_ll[i][j] -> left -> right = toroidal_ll[i][j] -> right;
				// same deal as before, now just for up pointer
				toroidal_ll[i][j] -> down -> up = toroidal_ll[i][j] -> up;
				// same deal as before, now just for down pointer
				toroidal_ll[i][j] -> up -> down = toroidal_ll[i][j] -> down;
				// there's 1 less node in the column
				toroidal_ll[0][j] -> nCount -= 1;
			}
		}
	}
}
// When I'm calling this method in Algorithm X, I'm going to cover every node in the row
// individually, so I will implement it such that it only eliminates the columns and rows
// for 1 node in the row
void sudokuboard::cover(node * n) {
	// I'm destroying the right and left links of the column
	n -> column -> left -> right = n -> column -> right;
	n -> column -> right -> left = n -> column -> left;
	// will use later
	node * r;
	node * rn;
	// now I'm iterating through the column and I a destroying every row that intersects
	// this column
	for(r = n -> column -> down; r != n -> column; r = r -> down) {
		for(rn = r -> right; rn != r; rn = rn -> right) {
			// destroying above and below links
			rn -> up -> down = rn -> down;
			rn -> down -> up = rn -> up;
			// I now need to update the number of nodes in the column
			rn -> column -> nCount -= 1;
		}
	}
}
// right here is how I backtrack. 
// I restore the nodes that I covered when I encounter the end of the branch 
// of a non-solution. Then I just go and try again.
// IMPORTANT NOTE: This method abuses a neat piece of knowledge Donald Knuth popularized:
// when we remove an item from a linked list via our usual method, we can restore the item
// by just reversing the actions because the item's pointers are still intact, rather the 
// other elements are just no longer pointing to it.
void sudokuboard::uncover(node * n) {
	node * rn;
	node * ln;
	// I now have to reverse everything I did in cover()
	for(rn = n -> column -> up; rn != n -> column; rn = rn -> up) {
		for(ln = rn -> left; ln != rn; ln = ln -> left) {
			// we're re-linking this node vertically
			ln -> up -> down = ln;
			ln -> down -> up = ln;
			// since we've added a node back into the column, we increase nCount
			ln -> column -> nCount += 1;
		}
	}
	// now we need to re-attach the column head to the left and right
	n -> column -> left -> right = n -> column;
	n -> column -> right -> left = n -> column;
}
// for Algorithm X we need to deterministically select the column to cover
sudokuboard::node * sudokuboard::smallestColumn() {
	node * h = head;
	node * col = h -> right;
	h = col -> right;
	while(h != head) {
		if(h -> nCount < col -> nCount) {
			col = h;
		}
		h = h -> right;
	}
	return col;
}
// NOTE: All pseudocode online does not use tail-recrusion, so this is my own original
// implementation (as far as I am aware).
void sudokuboard::aX(node * k) {
	// there are no more columns
	// once I find a solution
	// base case
	// because if this is true, there are no more columns in the toroidal_ll
	if (head -> right == head) {
		return;
	}
	node * rn;
	node * r = k;
	node * ln;
	// I only start backtracking if I've reached a column header, meaning there are no 
	// nodes in the smallest column. This would imply that there's no solution in the 
	// current branch
	if(k -> rNum == 0) {
		// now I'm essentially backtracking until I reach a point where I can travel down
		// to the next node in the column
		while(r -> down == r -> column) {
			// getting the x and y of the node to pop
			int y = history.top();
			history.pop();
			// x-coord
			int x = history.top();
			history.pop();
			// gotta pop it off the solutions
			solutions.pop();
			r = toroidal_ll[x][y];
			// uncover the column that was previously covered
			uncover(r -> column);
			// Now I'm just reversing everything I did when I was covering
			for(ln = r -> left; ln != r; ln = ln -> left) {
				uncover(ln);
			}
		}
		// after completely backtracking, I just move down to the next node.
		// It's like going up my branch until I reach a new branch,
		// then taking that new branch
		k = r -> down;
	}
	// this is when I'm travelling forward down current branch
	else {
		// covering the column then going to start covering the rows
		cover(k -> column);
		// because I'm keeping k's pointers
		// pushing my solution down
		solutions.push(k -> rNum);
		// saving x & y of this node for backtracking
		history.push(k -> rNum);
		history.push(k -> cNum);
		// I'm covering anything that intersects with the row.
		// This means I eliminate any row that is not a potential element of the
		// solution set
		for(rn = k -> right; rn != k; rn = rn -> right) {
			cover(rn);
		}
		// gotta reset the k to pass it on to the recursion
		// choosing the smallest column is analagous to 
		// selecting the most constrained square
		k = smallestColumn() -> down;
	}
	// this is the recursive call.
	// As you know, this originally didn't use tail-recursion, so the run-time
	// was dogshit.
	// But now that I created my own Algorithm X implementation with tail-recursion,
	// it's super fast.
	aX(k);
}
void sudokuboard::knuthAx() {
	node * rowNode;
	node * rightNode;
	node * leftNode;
	node * column;
	if(head->right == head)
    {
        printStack();
        return;
    }
  
    // choose column deterministically
    column = smallestColumn();
  
    // cover chosen column
    cover(column);
  
    for(rowNode = column->down; rowNode != column; 
        rowNode = rowNode->down )
    {
        solutions.push(rowNode -> rNum);
  
        for(rightNode = rowNode->right; rightNode != rowNode; rightNode = rightNode->right)
            cover(rightNode);
        knuthAx();
        solutions.pop();
  
        column = rowNode->column;
        for(leftNode = rowNode->left; leftNode != rowNode;
            leftNode = leftNode->left)
            uncover(leftNode);
    }
  
    uncover(column);
}
void sudokuboard::printStack() {
	solutions.print();
}
int sudokuboard::getMin() {
	// the largest row # possible is 729
	int min = 730;
	int index = 0;
	for(int i = 0; i < 81; i++) {
		if(answerArray[i] < min) {
			min = answerArray[i];
			index = i;
		}
	}
	answerArray[index] = 730;
	return min;
}
// just get the minimum element and then print it
void sudokuboard::printBoard() {
	for(int i = 0; i < 81; i++) {
		answerArray[i] = solutions.top();
		solutions.pop();
	}
	int count = 0;
	for(int i = 0; i < 81; i++) {
		int value = getMin() % 9;
		if(value == 0) {
			value = 9;
		}
		cout << value;
		count++;
		if(count == 9) {
			count = 0;
			cout << endl;
		}
	}

}