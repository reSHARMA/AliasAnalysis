set(LLVM_LINK_COMPONENTS core support irreader passes transformutils)
# set(LLVM_LINK_COMPONENTS aggressiveinstcombine all analysis asmparser asmprinter binaryformat bitreader bitstreamreader bitwriter cfguard codegen core coroutines coverage debuginfocodeview debuginfodwarf debuginfogsym debuginfomsf debuginfopdb demangle dlltooldriver dwarflinker engine executionengine extensions frontendopenmp fuzzmutate globalisel instcombine instrumentation interpreter ipo irreader libdriver lineeditor linker lto mc mca mcdisassembler mcparser mirparser native nativecodegen objcarcopts object objectyaml option orcerror passes profiledata remarks runtimedyld scalaropts selectiondag support symbolize tablegen target textapi transformutils vectorize windowsmanifest xray)
add_executable(AADriver Driver.cpp)
set_target_properties(AADriver PROPERTIES
    COMPILE_FLAGS "-std=c++14 -fno-rtti"
)

llvm_config (AADriver ${LLVM_LINK_COMPONENTS})

target_link_libraries(AADriver PRIVATE AliasAnalysis)
