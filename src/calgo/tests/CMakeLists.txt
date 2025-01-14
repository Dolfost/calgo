file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(calgo_${TNAME} 
		${T}
	)
	target_compile_definitions(calgo_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(calgo_${TNAME} 
		ca::calgo
	)
	add_test(NAME calgo_${TNAME}
		COMMAND calgo_${TNAME}
	)
endforeach()
