cmake_minimum_required(VERSION 3.10)

add_library(HttpFramework
            src/Server.cpp
            src/EpollManager.cpp
            src/Connection.cpp
            src/utils.cpp
            src/Connection.cpp
            src/Errors.cpp
            src/HttpRequest.cpp)
target_include_directories(HttpFramework PUBLIC include)
target_link_libraries(HttpFramework PRIVATE pthread coroutine Descriptor Logger)
target_compile_features(HttpFramework PRIVATE cxx_std_17)