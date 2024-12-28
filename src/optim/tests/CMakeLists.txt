add_subdirectory(testutils)

file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(optim_${TNAME} 
		${T}
	)
	target_compile_definitions(optim_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(optim_${TNAME} 
		ca::optim
		calgo_optim_testutils
	)
	add_test(NAME optim_${TNAME}
		COMMAND optim_${TNAME}
	)
endforeach()
