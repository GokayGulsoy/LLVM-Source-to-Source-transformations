# LLVM-Source-to-Source-transformations
This Repository contains some examples of source-to-source transformations applied with LLVM's LibTooling and RecursiveASTVisitor

## MatrixMultiplicationTypeModifier
This is a simple LibTooling based tool to apply source-to-source transformation to matrix multiplication algorithm which 
detects double type matrix declarations and double type variables which are involved in the matrix related operations
and converts their type to float.

## izzyrefactor
This is a simple LibTooling based tool to apply source-to-source transformation to class named Animal which detects the 
member function named `walk()` and changes its declaration into `run()`, and it also finds the member function calls and 
changes them from `walk()` to `run()`.

## CUDAMatrixMultiplicationModifier
This is a simple LibTooling based tool to apply source-to-source transformation to matrix multiplication algorithm
implemented in CUDA which detects double type vector declarations and double type variable declarations which are 
involved in matrix multiplication related operations and convert their data type to float.
