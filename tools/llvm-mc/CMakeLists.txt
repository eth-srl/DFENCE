set(LLVM_LINK_COMPONENTS ${LLVM_TARGETS_TO_BUILD} support MC MCParser)

add_llvm_tool(llvm-mc
  llvm-mc.cpp
  Disassembler.cpp
  )
