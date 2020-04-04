target_sources(language_test PUBLIC 
    main.cpp)

find_package(ordeal REQUIRED)
target_link_libraries(language_test PUBLIC ordeal language)