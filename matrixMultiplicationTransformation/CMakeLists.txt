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
