cmake_minimum_required(VERSION 3.10)

add_library(HttpFramework
            src/Server/Server.cpp
            src/Server/EpollManager.cpp
            src/Connection.cpp
        src/utils/utils.cpp
            src/Connection.cpp
            src/Errors.cpp
            src/http_structures/HttpRequest.cpp
            src/http_structures/HttpResponse.cpp)

target_include_directories(HttpFramework PUBLIC include include/http_structures)
target_link_libraries(HttpFramework PUBLIC pthread coroutine Descriptor Logger)
target_compile_features(HttpFramework PUBLIC cxx_std_17)
#target_compile_options(HttpFramework PUBLIC -O0 -ggdb3)

add_subdirectory(example)