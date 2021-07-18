#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>
#include "sudokuboard.h"
#include <fstream>
using namespace std;

int main(int argc, char * argv[]) {
	// takes file as command line input
		// sudokuboard s("");
		// // make cover matrix
		// //s.makeCover();
		// // make toroidal linked list
		// s.makeToroidal();
		// // cut out 0's
		// s.trimToroidal();
		// // call algorithm X to solve exact cover
		// time_t start, end;
		// time(&start);
		// ios_base::sync_with_stdio(false);
		// s.knuthAx();
		// //s.aX(s.smallestColumn() -> down);
		// time(&end);
		// double time_taken = double(end - start);
  //   cout << endl << "Time taken by program is : " << fixed
  //        << time_taken << setprecision(45);
  //   cout << " sec " << endl;
		// s.printStack();
	if(argc != 2) {
		cerr << "ERROR: Program needs file as command-line input.";
		return 0;
	}
	// reading in the sudoku board
	string fileName = argv[1];
	ifstream board(fileName);
	string config;
	string temp;
	// checks if file exists
	if(!board) {
		cerr << "ERROR: File does not exist.";
		return 0;
	}
	while(getline(board, temp)) {
		//cout << temp << endl;
		config += temp;
	}
	// cout << config << endl;
	// cout << config.length() << endl;
	board.close();
	int hCount = 0;
	for(int i = 0; i < config.length(); i++) {
		if(config[i] != '_') {
			hCount++;
		}
	}
	// gotta have at least 17 hints
	if(hCount < 17) {
		cerr << "ERROR: Not enough hints to analytically generate unique solution.";
	}
	else {
		// creating the sudoku board
		sudokuboard s(config);
		// make cover matrix
		s.makeCover();
		// make toroidal linked list
		s.makeToroidal();
		// cut out 0's
		s.trimToroidal();
		// call algorithm X to solve exact cover
		time_t start, end;
		time(&start);
		ios_base::sync_with_stdio(false);
		s.knuthAx();
		// s.aX(s.smallestColumn() -> down);
		time(&end);
		double time_taken = double(end - start);
    cout << endl << "Time taken by program is : " << fixed
         << time_taken << setprecision(45);
    cout << " sec " << endl;
    s.printStack();
		// output board
		// s.printBoard();
	}
}