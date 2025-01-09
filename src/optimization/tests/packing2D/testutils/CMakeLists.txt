add_library(calgo_opt_packing2D_testutils INTERFACE)

target_link_libraries(calgo_opt_packing2D_testutils
	INTERFACE ca::optimization
)

target_sources(calgo_opt_packing2D_testutils 
	PUBLIC FILE_SET HEADERS 
	BASE_DIRS include/
	FILES 
		include/calgo/opt/draw.hpp
		include/calgo/opt/test.hpp
		include/calgo/opt/boxSet2D.hpp
)
