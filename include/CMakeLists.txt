target_include_directories(language INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> 
	$<INSTALL_INTERFACE:include/language>)

target_include_directories(language_test PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

set(headers 
	composition.hpp
	concepts.hpp
	define.hpp
	dot.hpp
	grammar.hpp
	lexer.hpp
	parser.hpp
	production.hpp
	state.hpp
	states.hpp)

install(FILES ${headers} DESTINATION include/language)
