add_library(ca_linear)
add_library(ca::linear ALIAS ca_linear)
target_sources(ca_linear
	PRIVATE
		src/cyclicThomas.cpp
		src/thomas.cpp
	PUBLIC FILE_SET HEADERS 
			BASE_DIRS include/
			FILES
				include/calgo/lin/cyclicThomas.hpp
				include/calgo/lin/thomas.hpp
)
set_target_properties(ca_linear PROPERTIES
	OUTPUT_NAME calinear
	EXPORT_NAME linear
)

target_link_libraries(ca_linear
	PUBLIC ca::calgo
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
