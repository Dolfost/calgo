add_library(calgo_optim_testutils INTERFACE)

target_link_libraries(calgo_optim_testutils
	INTERFACE ca::optim 
)

target_sources(calgo_optim_testutils 
	PUBLIC FILE_SET HEADERS 
	BASE_DIRS include/
	FILES 
		include/calgo/optim/draw.hpp
		include/calgo/optim/test.hpp
		include/calgo/optim/boxSet2D.hpp
)
