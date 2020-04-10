set(SOURCE_LIB src/Process.cpp src/utils.cpp src/ProcError.cpp)

add_library(proc STATIC ${SOURCE_LIB})
target_include_directories(proc PUBLIC include)
target_link_libraries(proc PRIVATE Descriptor)
target_compile_options(proc PRIVATE -Wall -Wextra -Werror)
target_compile_features(proc PUBLIC cxx_std_17)