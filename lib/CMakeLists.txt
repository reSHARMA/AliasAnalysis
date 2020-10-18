add_library(AliasAnalysis SHARED
    AliasAnalysis.cpp
)
set_target_properties(AliasAnalysis PROPERTIES
    COMPILE_FLAGS "-std=c++14 -fno-rtti"
)
target_link_libraries(AliasAnalysis AliasToken)
