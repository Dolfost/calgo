add_subdirectory(testutils)

file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(opt_simplex_${TNAME} 
		${T}
	)
	target_compile_definitions(opt_simplex_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(opt_simplex_${TNAME} 
		ca::optimization
		calgo_opt_simplex_testutils
	)
	add_test(NAME opt_simplex_${TNAME}
		COMMAND opt_simplex_${TNAME}
	)
endforeach()
