add_library(ca_optim INTERFACE)
add_library(ca::optim ALIAS ca_optim)
target_sources(ca_optim
	PUBLIC FILE_SET HEADERS
			BASE_DIRS include/
			FILES 
				include/calgo/optim/packing2D.hpp
)
set_target_properties(ca_optim PROPERTIES
	OUTPUT_NAME caoptim
	EXPORT_NAME optim
)
target_compile_features(ca_optim 
	INTERFACE cxx_std_${CALGO_CXX_STANDARD}
)

target_link_libraries(ca_optim
	INTERFACE ca::calgo
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
