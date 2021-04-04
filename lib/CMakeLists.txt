add_library(PointsToAnalysis SHARED
    FlowInsensitivePointsToAnalysis.cpp
    FlowSensitivePointsToAnalysis.cpp
    ContextSensitivePointsToAnalysis.cpp
)
set_target_properties(PointsToAnalysis PROPERTIES
    COMPILE_FLAGS "-g -std=c++14 -fno-rtti"
)
target_link_libraries(PointsToAnalysis Spatial)
