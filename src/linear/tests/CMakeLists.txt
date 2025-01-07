file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(lin_${TNAME} 
		${T}
	)
	target_compile_definitions(lin_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(lin_${TNAME} ca::linear)
	add_test(NAME lin_${TNAME}
		COMMAND lin_${TNAME}
	)
endforeach()
