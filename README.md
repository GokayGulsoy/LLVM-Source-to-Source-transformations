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

## CUDAMatrixMultiplicationTransformerIntegratedTool
This is a simple LibTooling based tool to apply source-to-source transformation to matrix multiplication algorithm
implemented in CUDA which enables user to give command line parameters that can be used to provide how many threads
to be used in the CUDA kernel call statement and also provides an option to change double types into float types in
matrix related operations as provided in the above version of the tool, but in this version these transformations are
optional. By default number of threads to be launched within the kernel call in the transformed code is set to 64 and
conversion from double to float type is not enabled unless command line parameter is explicitly provided.
