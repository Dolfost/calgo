add_library(ca_numeric STATIC)
add_library(ca::numeric ALIAS ca_numeric)
target_sources(ca_numeric
	PRIVATE
		src/simpson.cpp
	PUBLIC FILE_SET HEADERS
			BASE_DIRS include/
			FILES 
				include/calgo/numeric/simpson.hpp
)
set_target_properties(ca_numeric PROPERTIES
	OUTPUT_NAME canumeric
	EXPORT_NAME numeric
)

target_link_libraries(ca_numeric
	PUBLIC ca::calgo
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
