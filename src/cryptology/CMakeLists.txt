add_library(ca_cryptology)
add_library(ca::cryptology ALIAS ca_cryptology)
target_sources(ca_cryptology
	PRIVATE
		src/scytale.cpp
		src/freq_analysis.cpp
		src/random.cpp
	PUBLIC FILE_SET HEADERS
		BASE_DIRS include/ FILES
			include/calgo/cr/cipher.hpp
			include/calgo/cr/scytale.hpp
			include/calgo/cr/random.hpp
)
set_target_properties(ca_cryptology PROPERTIES
	OUTPUT_NAME cacryptology
	EXPORT_NAME cryptology
)

target_link_libraries(ca_cryptology
	ca::calgo 
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
