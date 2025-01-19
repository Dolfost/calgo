add_library(ca_optimization INTERFACE)
add_library(ca::optimization ALIAS ca_optimization)
target_sources(ca_optimization
	PUBLIC FILE_SET HEADERS
			BASE_DIRS include/
			FILES 
				include/calgo/opt/packing2D.hpp
				include/calgo/opt/simplex.hpp
				include/calgo/opt/transportationSimplex.hpp
)
set_target_properties(ca_optimization PROPERTIES
	OUTPUT_NAME caoptimization
	EXPORT_NAME optimization
)
target_compile_features(ca_optimization
	INTERFACE cxx_std_${CALGO_CXX_STANDARD}
)

target_link_libraries(ca_optimization
	INTERFACE ca::calgo
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
