file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(interpol_${TNAME} 
		${T}
	)
	target_compile_definitions(interpol_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(interpol_${TNAME} ca::interpol)
	add_test(NAME interpol_${TNAME}
		COMMAND interpol_${TNAME}
	)
endforeach()
