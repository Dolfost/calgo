add_subdirectory(testutils)

file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(opt_packing2D_${TNAME} 
		${T}
	)
	target_compile_definitions(opt_packing2D_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(opt_packing2D_${TNAME} 
		ca::optimization
		calgo_opt_packing2D_testutils
	)
	add_test(NAME opt_packing2D_${TNAME}
		COMMAND opt_packing2D_${TNAME}
	)
endforeach()
