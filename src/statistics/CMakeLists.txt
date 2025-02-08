add_library(ca_statistics INTERFACE)
add_library(ca::statistics ALIAS ca_statistics)
target_sources(ca_statistics
	PUBLIC FILE_SET HEADERS
		BASE_DIRS include/ FILES
			include/calgo/ss/vec.hpp
			include/calgo/ss/mat.hpp
			include/calgo/ss/statistic.hpp
)
set_target_properties(ca_statistics PROPERTIES
	OUTPUT_NAME castatistics
	EXPORT_NAME statistics
)

target_link_libraries(ca_statistics
	INTERFACE ca::calgo
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
