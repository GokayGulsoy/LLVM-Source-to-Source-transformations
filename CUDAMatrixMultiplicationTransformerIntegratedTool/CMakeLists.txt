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
