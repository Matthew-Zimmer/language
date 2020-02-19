target_include_directories(language INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> 
	$<INSTALL_INTERFACE:include/language>)

set(headers 
	language.hpp)

install(FILES ${headers} DESTINATION include/language)
