add_library(ca_interpolation)
add_library(ca::interpolation ALIAS ca_interpolation)
target_sources(ca_interpolation
	PRIVATE
		src/cubic.cpp
		src/cubicParametric.cpp
		src/lagrange.cpp
		src/newtonBackward.cpp
	PUBLIC FILE_SET HEADERS
		BASE_DIRS include/ FILES
			include/calgo/in/cubic.hpp
			include/calgo/in/cubicParametric.hpp
			include/calgo/in/lagrange.hpp
			include/calgo/in/newtonBackward.hpp
)
set_target_properties(ca_interpolation PROPERTIES
	OUTPUT_NAME cainterpolation
	EXPORT_NAME interpolation
)

target_link_libraries(ca_interpolation
	ca::calgo 
	ca::linear
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
