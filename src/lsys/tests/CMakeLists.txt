enable_testing()

file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(lsys_${TNAME} 
		${T}
	)
	target_compile_definitions(lsys_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(lsys_${TNAME} ca::lsys)
	add_test(NAME lsys_${TNAME}
		COMMAND lsys_${TNAME}
	)
endforeach()
