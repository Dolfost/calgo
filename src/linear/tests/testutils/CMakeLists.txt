add_library(calgo_lin_testutils INTERFACE)

target_link_libraries(calgo_lin_testutils
	INTERFACE ca::optimization
)

target_sources(calgo_lin_testutils 
	PUBLIC FILE_SET HEADERS 
	BASE_DIRS include/
	FILES 
		include/calgo/lin/test.hpp
)
