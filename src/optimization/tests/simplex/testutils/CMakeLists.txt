add_library(calgo_opt_simplex_testutils INTERFACE)

target_link_libraries(calgo_opt_simplex_testutils
	INTERFACE ca::optimization
)

target_sources(calgo_opt_simplex_testutils 
	PUBLIC FILE_SET HEADERS 
	BASE_DIRS include/
	FILES 
		include/calgo/opt/test.hpp
)
