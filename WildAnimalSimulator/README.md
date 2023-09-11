# MatrixMultiplicationTypeModifier
This tool is used to transform member function declaration `walk()` into `run()` and member function call `walk()` which is called via Cat subclass of Animal class into `run()`

## How to build the tool 
1. We need to create a subdirectory under the clang-tools-extra named `izzyrefactor` 
2. Then we need to add the following line to CMakeLists.txt file located under the clang-tools-extra directory `add_subdirectory(izzyrefactor)` 
3. Then we need to create another CMakeLists.txt file under the izzyrefactor directory which we have created in the step 1
4. We need to add the following contents to this CMakeLists.txt file


`set(LLVM_LINK_COMPONENTS support)
add_clang_executable(izzyrefactor
	izzyrefactor.cpp
  )
target_link_libraries(izzyrefactor
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  clangEdit
  )`

5.After adding the above contents to CMakeLists.txt file, we need to place the source code for the tool under the izzyrefactor directory (source code for the file is provided in izzyrefactor.cpp)

6.Finally we need to build the tool by changing the current directory to build directory of LLVM and execute the `make` command from terminal

## Commands to be executed for building tool successivelly
1. cd ~/clang-llvm
2. mkdir clang-tools-extra/izzyrefactor echo 'add_subdirectory(izzyrefactor)' >> clang-tools-extra/CMakeLists.txt
3. vim clang-tools-extra/izzyrefactor/CMakeLists.txt
4. Add the following contents to CMakeLists.txt file created in the step 3

`set(LLVM_LINK_COMPONENTS support)
add_clang_executable(izzyrefactor
	izzyrefactor.cpp
  )
target_link_libraries(izzyrefactor
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  clangEdit
  )`

5. cd ~/clang-llvm-project/build
6. make

## Command to be exectued from terminal to run the tool on C++ source code file
`izzyrefactor wildanimal-sim.cpp --`
