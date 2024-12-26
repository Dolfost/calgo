add_library(ca_interpol STATIC)
add_library(ca::interpol ALIAS ca_interpol)
target_sources(ca_interpol
	PRIVATE
		src/cubic.cpp
		src/cubicParametric.cpp
		src/lagrange.cpp
		src/newtonBackward.cpp
		src/nodes.cpp
	PUBLIC FILE_SET HEADERS
		BASE_DIRS include/ FILES
			include/calgo/interpol/cubic.hpp
			include/calgo/interpol/cubicParametric.hpp
			include/calgo/interpol/lagrange.hpp
			include/calgo/interpol/newtonBackward.hpp
)
set_target_properties(ca_interpol PROPERTIES
	OUTPUT_NAME cainterpol
	EXPORT_NAME interpol
)

target_link_libraries(ca_interpol
	PUBLIC ca::lsys
)

target_link_libraries(ca_lsys
	PUBLIC ca::calgo
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
