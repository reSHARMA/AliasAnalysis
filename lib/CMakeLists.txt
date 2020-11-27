add_library(AliasAnalysis SHARED
    AliasUtils.cpp
    AliasAnalysis.cpp
)
set_target_properties(AliasAnalysis PROPERTIES
    COMPILE_FLAGS "-g -std=c++14 -fno-rtti"
)
target_link_libraries(AliasAnalysis AliasToken AliasBench)
