For this project, I'm approaching it in a different manner.
The comments in the project itself should help you, so I'll be brief here.

Essentially what I'm doing is I'm exploiting the NP-completeness of Sudoku.
Since Sudoku and Exact Cover are 2 NP-complete problems, I can transform them into one another.

In this case, I turned Sudoku into a 9*9*9 x 9*9*4 exact cover matrix.
We hace 9^3 rows because we need 1 row for each possible value of each cell in the grid.
We have 4*9^2 columns because Sudoku has 4 constraints (cell, row, column, box), and we need to ensure that each element in our solutions abides by those constraints.

So the exact cover problem is essentially as follows: suppose that 
you have a set S and you have some set Q of subsets of S. Then the goal is to find
a set P of these subsets in Q such that the intersection of any 2 elements of P
will be Null and the union of all elements in P should be S.

We can solve this problem by representing each subset as a row in a matrix where each column is an element of S. 
Then we just need to find a set of rows that abides by our constraints.

Donald Knuth figured out a method of solving this that was much faster than traditional approaches; it's called Dancing Links, and it's what I implemented.

Dancing Links turns the matrix into a toroidal linked list (every column and every row is a circular doubly linked list). With this, backtracking becomes much easier because when you get rid of a node in the linked list, although you get rid of the other nodes' pointers to the deleted node, the deleted node still has its pointers. This means you can easily add it back by just reversing the deletion using the deleted node's pointers.

Oh yeah, creating and trimming down the toroidal linked list isn't difficult, but creating the cover matrix the toroidal_ll is based off of is pretty tricky.
What I did is I found a paper that provided a diagram of how the various constraints could be visualized inside of the cover matrix. So I then just figured out the various patterns then coded them inside of the makeCover() method (the box constraint was the only hard one).

Anyway, the algorithm to solve this problem is called Algorithm X, and below is an explanation.

- Algorithm X is an algorithm devised by Donald Knuth which exploits the fact that
if we define L[x] to be the node to the left of x and R[x] to be the node to the right
of x, then L[x] -> L[R[x]] and R[x] -> R[L[x]] removes x from the linked list.
But then we can just do L[R[x]] -> x and R[L[x]] -> x to restore the node. This 
is because to remove the node all we did was change the pointers of the nodes adjacent
to x, meaning that we left x's pointers intact. So we can just reverse the process 
to restore the node. This makes solving the exact cover problem much easier 
when using a linked list implementation, as we have just shown backtracking 
can easily be done.
- Algorithm X uses DFS, meaning that it will go through a potential solution until 
that solution is shown to be wrong. It will then backtrack to the next highest branch,
then it will just repeat. DFS is useful for this problem because our branches are
guaranteed to have a finite length, meaning that we will never get stuck down an
infinite branch.

- Algorithm X: What we first do is deterministically select a column in our 
toroidal linked list. You can do this in a multitude of ways: select the column
with the most 1's, select the column with the least 1's, the leftmost column, 
the rightmost column, etc. We must consider the columns to be the elements
of the set S, and the rows are subsets in Q. Our job is now to eliminate any rows 
with overlap and find a solution. Now we randomly choose a row that contains a 1 
in our chosen column. Since we can't choose truly randomly, we just select 
the first row (so we can backtrack). Then what we do is we add this row 
to our working solution set. Since this row is now part of the solution set, 
we now eliminate any other rows with 1's in the same columns as our row we just added.
In addition, we eliminate all columns that contain a 1 in our chosen row.
We continue this process until we either reach a point where we have 0 columns left 
(we've solved the problem), or we reach a point where we have no rows left but still
have some non-zero number of columns left. If we reach the latter ourcome, we backtrack
until we reach a point where we can continue this process. Using Donald Knuth's idea,
we can easily backtrack by popping our solution from the solution stack and restoring
the removed rows and columns using the idea we mentioned earlier. So all in all,
it's actually a fairly straightforward and simple algorithm. We implement it below. 

I have only seen non-tail recursive implementations of Algorithm X, so in order to speed up my program I had to create a new tail-recursive implementation of the Algorithm. You can see the implementation and explanation in the comments.

I use a few methods inside my Algorithm X implementation, mainly cover() and uncover(). These methods are explained in the comments.

Anyway, what I end up doing is getting a stack of the row numbers of every row that is part of the solution set. This is nice because the sudoku cell's value and coordinates are encoded into that 1 row number, so I just get all the information from there, put them into an array, then print out the board.

So yeah, I guess I hope you have a good time checking out this code. It's pretty cool.

CITATIONS: I had help from Wikipedia and this paper:
https://www.kth.se/social/files/58861771f276547fe1dbf8d1/HLaestanderMHarrysson_dkand14.pdf
Essentially I just looked at a shit-load of pseudocode and linked list diagrams
until I completely understood the algorithm and the toroidal linked list.
I also had to create my own new tail-recursive variation on Algorithm X
 
FUN FACT: The toroidal linked list is called toroidal because the way that each 
row and each column wraps around itself is analagous to how an ant travelling on a 
straight path along a torus would repeatedly traverse the same path.





