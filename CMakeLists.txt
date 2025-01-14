cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH 
	"${CMAKE_CURRENT_LIST_DIR}/cmake"
)
include(cmake-scripts/gitversion)
gitversion()

project(calgo
	LANGUAGES CXX 
	VERSION ${GIT_VERSION_TAG}
	DESCRIPTION "Small computation and algorithms library"
)

option(CALGO_CALGO   "Build library" YES)
option(CALGO_DOCS    "Build docs"    ${PROJECT_IS_TOP_LEVEL})
option(CALGO_TESTING "Build tests"   ${PROJECT_IS_TOP_LEVEL})
option(CALGO_QT      "Qt widgets"    YES)

if (CALGO_TESTING)
	enable_testing()
endif()

if (CALGO_CALGO)
	add_subdirectory(src)
endif()

if (CALGO_DOCS)
	add_subdirectory(doc)
endif()

add_subdirectory(install)

if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(packaging)
endif()
