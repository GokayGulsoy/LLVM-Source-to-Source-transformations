# LLVM-Source-to-Source-transformations
This Repository contains some examples of source-to-source transformations applied with LLVM's LibTooling and RecursiveASTVisitor
## MatrixMultiplicationTypeModifier
This is a simple LibTooling based tool to apply source-to-source transformation to matrix multiplication algorithm which 
detects double type matrix declarations and double type variables which are involved in the matrix related operations
and converts their type to float.
