add_library(ca_calgo INTERFACE)
add_library(ca::calgo ALIAS ca_calgo)

target_sources(ca_calgo
	INTERFACE FILE_SET HEADERS 
		BASE_DIRS include/
		FILES 
			include/calgo/calgo.hpp
			include/calgo/vector.hpp
			include/calgo/matrix.hpp
			# utilities
			include/calgo/macros.hpp
			include/calgo/nomacros.hpp
)

set_target_properties(ca_calgo PROPERTIES
	OUTPUT_NAME cacalgo
	EXPORT_NAME calgo
)
