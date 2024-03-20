set(LLVM_LINK_COMPONENTS_)

add_clang_executable(CUDAIntegratedTransformerTool CUDAIntegratedTransformerTool.cpp)

target_link_libraries(CUDAIntegratedTransformerTool
	PRIVATE
	clangAST
	clangASTMatchers
	clangFrontend
	clangSerialization
	clangTooling
	clangEdit)
