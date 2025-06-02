set(CALGO_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD ${CALGO_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(calgo)
add_subdirectory(linear)
add_subdirectory(interpolation)
add_subdirectory(numeric)
add_subdirectory(optimization)
add_subdirectory(cryptology)

if (CALGO_STATISTICS)
	add_subdirectory(statistics)
endif()

if (CALGO_QT)
	add_subdirectory(qt)
endif()
