add_library(ca_optim INTERFACE)
add_library(ca::optim ALIAS ca_optim)
target_sources(ca_optim
	# PRIVATE
		# src/packing2D.cpp
	PUBLIC FILE_SET HEADERS
			BASE_DIRS include/
			FILES 
				include/calgo/optim/packing2D.hpp
)
set_target_properties(ca_optim PROPERTIES
	OUTPUT_NAME caoptim
	EXPORT_NAME optim
)

target_link_libraries(ca_optim
	INTERFACE ca::calgo
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
