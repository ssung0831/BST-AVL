This is my code to implement a BST + AVL

Part 1- Worksheet modeling an AVL

Part 2- Recursive function to practice traversing a tree

To run basic testing:

cd hw4
make equal-paths-test
./equal-paths-test

To run in-depth testing:

cd hw4_tests/equalpaths-test
make equalpaths_tests
./equalpaths_tests

Valgrind:

valgrind --tool=memcheck --leak-check=yes ./equal-paths-test


Part- Implementation of a BST w/ iterator

To run basic testing:

cd hw4
make bst-test
./bst-test

To run in-depth testing:

cd hw4_tests/bst_tests
make bst_tests
./bst_tests

Valgrind:

valgrind --tool=memcheck --leak-check=yes ./bst-test
