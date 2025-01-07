add_subdirectory(testutils)

file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(optimization_${TNAME} 
		${T}
	)
	target_compile_definitions(optimization_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(optimization_${TNAME} 
		ca::optimization
		calgo_optimization_testutils
	)
	add_test(NAME optimization_${TNAME}
		COMMAND optimization_${TNAME}
	)
endforeach()
