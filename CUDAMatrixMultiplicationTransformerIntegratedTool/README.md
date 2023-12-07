# CUDAMatrixMultiplicationModifier
This tool is used to transform all `double` type vector declarations,
and double type variables which are involved in matrix multiplication 
related operations into `float` type

## How to build the tool 
1. We need to create a subdirectory under the clang-tools-extra named `CUDAMatrixMultiplicationModifier` 
2. Then we need to add the following line to CMakeLists.txt file located under the clang-tools-extra directory `add_subdirectory(CUDAMatrixMultiplicationModifier)` 
3. Then we need to create another CMakeLists.txt file under the CUDAMatrixMultiplicationModifier directory which we have created in the step 1
4. We need to add the following contents to this CMakeLists.txt file


`set(LLVM_LINK_COMPONENTS support)
add_clang_executable(CUDAMatrixMultiplicationModifier
	CUDAmatrixMultiplicationModifier.cpp
  )
target_link_libraries(CUDAMatrixMultiplicationModifier
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  clangEdit
  )`

5.After adding the above contents to CMakeLists.txt file, we need to place the source code for the tool under the CUDAMatrixMultiplicationModifier directory (source code for the file is provided in CUDAMatrixMultiplicationModifier.cpp)

6.Finally we need to build the tool by changing the current directory to build directory of LLVM and execute the `make` command from terminal

## Commands to be executed for building tool successivelly
1. cd ~/llvm-project
2. mkdir clang-tools-extra/CUDAMatrixMultiplicationModifier 
3. echo 'add_subdirectory(CUDAMatrixMultiplicationModifier)' >> clang-tools-extra/CMakeLists.txt
4. vim clang-tools-extra/CUDAMatrixMultiplicationModifier/CMakeLists.txt
5. Add the following contents to CMakeLists.txt file created in the step 4

`set(LLVM_LINK_COMPONENTS support)
add_clang_executable(CUDAMatrixMultiplicationModifier
	CUDAMatrixMultiplicationModifier.cpp
  )
target_link_libraries(CUDAMatrixMultiplicationModifier
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  clangEdit
  )`

5. cd ~/llvm-project/build
6. make

## Command to be exectued from terminal to run the tool on CUDA source code file

### Following version uses --threads=<value> command line argument to set the value of threads to be launched
```shell
CUDAMatrixMultiplicationModifier --threads=128 parallelMatrixMultiplication.cu -- --cuda-gpu-arch=sm_75
```

### Following version uses --reduction-ration=<value> command line argument to set reduction ratio for threads
```shell
CUDAMatrixMultiplicationModifier --threads=128 --reduction-ratio=25 parallelMatrixMultiplication.cu -- --cuda-gpu-arch=sm_75
```

### Following version uses --convert-double-to-float to enable data type transformation from double to float type
```shell
CUDAMatrixMultiplicationModifier --threads=256 --reduction-ratio=25 --convert-double-to-float parallelMatrixMultiplication.cu -- --cuda-gpu-arch=sm_75
```
