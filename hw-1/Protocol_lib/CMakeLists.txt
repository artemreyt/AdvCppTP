add_library(Protocol src/Protocol.cpp)
target_include_directories(Protocol PUBLIC include)
target_compile_features(Protocol PUBLIC cxx_std_17)