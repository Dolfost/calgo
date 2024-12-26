enable_testing()

file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(numeric_${TNAME} 
		${T}
	)
	target_compile_definitions(numeric_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(numeric_${TNAME} ca::numeric)
	add_test(NAME numeric_${TNAME}
		COMMAND numeric_${TNAME}
	)
endforeach()
