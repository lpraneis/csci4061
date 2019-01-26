// Header file for collatz problems

#ifndef COLLATZ_H
#define COLLZTZ_H 1

// Returns next number in the collatz sequence. If parameter n is less
// than 1, return -1. If n is equal to 1, return 1 as the sequence has
// converged.  If n is bigger than 1, determine if it is even or
// odd. If n is even, return half its value. If n is odd, return three
// times its value plus one. 
int collatz_next(int n);


// Returns the number of steps to converge to 1 for starting point
// n. Use the function collatz_next() in a loop to determine the next
// value at each step.  Return the number of steps to converge, 15 in
// the example below where n=22.  If paramter n is less than or equal
// to 1, return 0.
// 
// If the argument print_output is zero, this function should not
// print anything.  If the argument is non-zero, print each step in
// the sequence starting with the initial value of at Step 0.  For
// example, if n=22 and print_output=1, produce output like the
// following:
// 
// Step 0: 22
// Step 1: 11
// Step 2: 34
// Step 3: 17
// Step 4: 52
// Step 5: 26
// Step 6: 13
// Step 7: 40
// Step 8: 20
// Step 9: 10
// Step 10: 5
// Step 11: 16
// Step 12: 8
// Step 13: 4
// Step 14: 2
// Step 15: 1
int collatz_steps(int n, int print_output);

#endif
