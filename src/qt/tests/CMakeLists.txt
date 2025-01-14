file(GLOB TESTS CONFIGURE_DEPENDS
	*.cpp 
)

foreach(T ${TESTS})
	cmake_path(GET T STEM TNAME)
	add_executable(calgo_qt_${TNAME} 
		${T}
	)
	target_compile_definitions(calgo_qt_${TNAME} 
		PRIVATE TESTPATH="${CMAKE_CURRENT_LIST_DIR}"
	)
	target_link_libraries(calgo_qt_${TNAME} 
		ca::calgo
		ca::qt
		Qt6::Core Qt6::Widgets Qt6::Gui
	)
	# do not run with ctest
	# add_test(NAME calgo_qt_${TNAME}
	# 	COMMAND calgo_qt_${TNAME}
	# )
endforeach()
