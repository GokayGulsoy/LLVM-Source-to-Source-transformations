## How to build the tool 
------------------------
1. We need to create a subdirectory under the clang-tools-extra named `CUDAIntegratedTransformerTool` 
2. Then we need to add the following line to CMakeLists.txt file located under the clang-tools-extra directory `add_subdirectory(CUDAIntegratedTransformerTool)` 
3. Then we need to create another CMakeLists.txt file under the CUDAIntegratedTransformerTool directory which we have created in the step 1
4. We need to add the following contents to this CMakeLists.txt file


```shell
set(LLVM_LINK_COMPONENTS support)
add_clang_executable(CUDAIntegratedTransformerTool
	CUDAIntegratedTransformerTool.cpp
  )
target_link_libraries(CUDAIntegratedTransformerTool
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  clangEdit
  )
```

5.After adding the above contents to CMakeLists.txt file, we need to place the source code for the tool under the CUDAIntegratedTransformerTool directory (source code for the file is provided in CUDAIntegratedTransformerTool.cpp)

6.Finally we need to build the tool by changing the current directory to build directory of LLVM and execute the `make` command from terminal

## Commands to be executed successivelly for building tool
1. cd ~/llvm-project
2. mkdir clang-tools-extra/CUDAIntegratedTransformerTool 
3. echo 'add_subdirectory(CUDAIntegratedTransformerTool)' >> clang-tools-extra/CMakeLists.txt
4. vim clang-tools-extra/CUDAIntegratedTransformerTool/CMakeLists.txt
5. Add the following contents to CMakeLists.txt file created in the step 4

```shell
set(LLVM_LINK_COMPONENTS support)
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
)
```

## Command line options of tool

1. `--threads` command line option is used to specify thread number to be assigned after replacement
2. `--reduction-ratio` command line option is used to specify the thread number reduction ratio
3. `convert-double-to-float` command line option is used to convert double types to floats
4. `change-kernel` command line option allows change of kernel launch parameters
5. `KernelParam-num` command line option specifies which parameter of the kernel launch statement to modify
6. `--dim3` command line option allows dim3 declaration parameters to be changed
7. `--num-dim3-changes` command line option defines how many dim3 declarations to be modified
8. `--change-specific` command line option allows specific variable to be changed
9. `--change-var-name` command line option takes the name of the variable whose value is reassigned with given thread number

   
## Example execution commands from command line

```shell
  CUDAIntegratedTransformerTool --threads=128 --change-specific=true --change-var-name="DIM_THREAD_BLOCK_X" gramschmidt.cu -- -I/home/gokay/PolyBench-ACC/common --cuda-gpu-arch=sm_75 >
```
Above example takes a CUDA source code file named gramschmidt.cu and changes the varibles which are named `DIM_THREAD_BLOCK_X` to specified thread number which is `128`, in order to change
variable with specific name `change-specific` and `change-var-name` options are used also.

```shell
  CUDAIntegratedTransformerTool --threads=128 --dim3=true --num-dim3-changes=1 gemm.cu -- -I/home/gokay/PolyBench-ACC/common --cuda-gpu-arch=sm_75 > modified_gemm.cu
```
Above example takes a CUDA source code file named gemm.cu and allows changing the dim3 declaration parameters via `dim3=true` option and also how many dim3 declarations to be modified
is specified by the `num-dim3-changes` option, in this case only 1 dim3 declaration is allowed to be modified and its parameters are replaced with 128 which is indicated by `threads` 
command line option.
