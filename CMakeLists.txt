cmake_minimum_required(VERSION 3.13)

project(language)

add_library(language INTERFACE)
add_executable(language_test)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)

target_compile_features(language INTERFACE cxx_std_20)
target_compile_features(language_test PUBLIC cxx_std_20)

#Set(CMAKE_CXX_FLAGS "-Wall" "-Wextra")
set(CMAKE_CXX_FLAGS -fmax-errors=1)


install(TARGETS language EXPORT language DESTINATION lib/language)
install(EXPORT language DESTINATION lib/)
install(FILES language-config.cmake DESTINATION lib/language)


#libs
#find_Package(LIB REQUIRED)
#target_link_libraries(language INTERFACE LIB)
