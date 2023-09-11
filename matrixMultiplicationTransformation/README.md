# MatrixMultiplicationTypeModifier
This tool is used to transform all `double` type matrix declarations
and double type variables which are involved in matrix related operations
into `float` type

## How to build the tool 
1.We need to create a subdirectory under the clang-tools-extra named
`matrixMultiplicationTypeModifier` 
2.Then we need to add the following line to CMakeLists.txt file located
under the clang-tools-extra directory `add_subdirectory(matrixMultiplicationTypeModifier)` 
3.Then we need to create another CMakeLists.txt file under the matrixMultiplicationTypeModifier directory which we have created in the step 1
4.We need to add the following contents to this CMakeLists.txt file

set(LLVM_LINK_COMPONENTS support)

add_clang_executable(matrixMultiplicationTypeModifier
	matrixMultiplicationTypeModifier.cpp
  )
target_link_libraries(matrixMultiplicationTypeModifier
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  clangEdit
  )

5.After adding the above contents to CMakeLists.txt file, we need to place
the source code for the tool under the matrixMultiplicationTypeModifier directory (source code for the file is provided in matrixMultiplicationTypeModifier.cpp)

6.Finally we need to build the project by changing directory to build folder and executing the set(LLVM_LINK_COMPONENTS support)

add_clang_executable(matrixMultiplicationTypeModifier
	matrixMultiplicationTypeModifier.cpp
  )
target_link_libraries(matrixMultiplicationTypeModifier
  PRIVATE
  clangAST
  clangASTMatchers5.After adding the above contents to CMakeLists.txt file, we need to place
the source code for the tool under the matrixMultiplicationTypeModifier directory (source code for the file is provided in matrixMultiplicationTypeModifier.cpp)

6.Finally we need to build the tool by changing the current directory to 
build directory of LLVM and execute the `make` command from terminal

## Commands to be executed for building tool successivelly
1.cd ~/clang-llvm
2.mkdir clang-tools-extra/matrixMultiplicationTypeModifier
echo 'add_subdirectory(matrixMultiplicationTypeModifier)' >> clang-tools-extra/CMakeLists.txt
3.vim clang-tools-extra/matrixMultiplicationTypeModifier/CMakeLists.txt
4.Add the following contents to CMakeLists.txt file created in the step 3

set(LLVM_LINK_COMPONENTS support)

add_clang_executable(matrixMultiplicationTypeModifier
	matrixMultiplicationTypeModifier.cpp
  )
target_link_libraries(matrixMultiplicationTypeModifier
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  clangEdit
  )

5.cd ~/clang-llvm-project/build
6.make

## Command to be exectued from terminal to run the tool on C++ source codefile
`matrixMultiplicationTypeModifier matrixMultiplication.cpp --`
