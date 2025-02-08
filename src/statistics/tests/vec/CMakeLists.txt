file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(calgo_statistics_vec_${TNAME} 
		${T}
	)
	target_compile_definitions(calgo_statistics_vec_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(calgo_statistics_vec_${TNAME} ca::statistics)
	add_test(NAME calgo_statistics_vec_${TNAME}
		COMMAND calgo_statistics_vec_${TNAME}
	)
endforeach()
