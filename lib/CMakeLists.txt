add_library(AliasAnalysis SHARED
    FlowInsensitiveAliasAnalysis.cpp
    FlowSensitiveAliasAnalysis.cpp
)
set_target_properties(AliasAnalysis PROPERTIES
    COMPILE_FLAGS "-std=c++14 -fno-rtti"
)
target_link_libraries(AliasAnalysis AliasToken AliasBench CFGUtils Worklist)
