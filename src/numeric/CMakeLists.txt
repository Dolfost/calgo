add_library(ca_numeric)
add_library(ca::numeric ALIAS ca_numeric)
target_sources(ca_numeric
	PRIVATE
		src/simpson.cpp
	PUBLIC FILE_SET HEADERS
			BASE_DIRS include/
			FILES 
				include/calgo/num/simpson.hpp
				include/calgo/num/nystromFredholm.hpp
				include/calgo/num/intervalMinimum.hpp
				include/calgo/num/fibonacciMinimum.hpp
				include/calgo/num/goldenRatioMinimum.hpp
				include/calgo/num/euler_cauchy.hpp
				include/calgo/num/runge_kutta.hpp
)

set_target_properties(ca_numeric PROPERTIES
	OUTPUT_NAME canumeric
	EXPORT_NAME numeric
)

target_link_libraries(ca_numeric
	PUBLIC ca::calgo ca::linear ca::interpolation
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
