set(LLVM_LINK_COMPONENTS support)

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
  )
