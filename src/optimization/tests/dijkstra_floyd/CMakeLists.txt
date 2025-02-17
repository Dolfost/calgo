file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(opt_dijkstra_floyd_${TNAME} 
		${T}
	)
	target_compile_definitions(opt_dijkstra_floyd_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(opt_dijkstra_floyd_${TNAME} 
		ca::optimization
	)
	add_test(NAME opt_dijkstra_floyd_${TNAME}
		COMMAND opt_dijkstra_floyd_${TNAME}
	)
endforeach()
