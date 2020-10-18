set(LLVM_LINK_COMPONENTS core support irreader)

add_executable(AADriver Driver.cpp)
set_target_properties(AADriver PROPERTIES
    COMPILE_FLAGS "-std=c++14 -fno-rtti"
)

llvm_config (AADriver ${LLVM_LINK_COMPONENTS})

target_link_libraries(AADriver PRIVATE AliasAnalysis)
