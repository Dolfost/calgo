add_library(calgo_optimization_testutils INTERFACE)

target_link_libraries(calgo_optimization_testutils
	INTERFACE ca::optimization
)

target_sources(calgo_optimization_testutils 
	PUBLIC FILE_SET HEADERS 
	BASE_DIRS include/
	FILES 
		include/calgo/opt/draw.hpp
		include/calgo/opt/test.hpp
		include/calgo/opt/boxSet2D.hpp
)
