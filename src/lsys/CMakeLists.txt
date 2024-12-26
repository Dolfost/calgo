add_library(ca_lsys STATIC)
add_library(ca::lsys ALIAS ca_lsys)
target_sources(ca_lsys
	PRIVATE
		src/cyclicThomas.cpp
		src/thomas.cpp
	PUBLIC FILE_SET HEADERS 
			BASE_DIRS include/
			FILES
				include/calgo/lsys/cyclicThomas.hpp
				include/calgo/lsys/thomas.hpp
)
set_target_properties(ca_lsys PROPERTIES
	OUTPUT_NAME calsys
	EXPORT_NAME lsys
)

target_link_libraries(ca_lsys
	PUBLIC ca::calgo
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
