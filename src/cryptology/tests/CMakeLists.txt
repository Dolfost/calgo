file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(calgo_cryptology_${TNAME} 
		${T}
	)
	target_compile_definitions(calgo_cryptology_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(calgo_cryptology_${TNAME} ca::cryptology)
	add_test(NAME calgo_cryptology_${TNAME}
		COMMAND calgo_cryptology_${TNAME}
	)
endforeach()
