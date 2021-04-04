set(LLVM_LINK_COMPONENTS core support irreader passes transformutils)
add_executable(PTDriver Driver.cpp)
set_target_properties(PTDriver PROPERTIES
    COMPILE_FLAGS "-std=c++14 -fno-rtti"
)

llvm_config (PTDriver ${LLVM_LINK_COMPONENTS})

target_link_libraries(PTDriver PRIVATE PointsToAnalysis)
