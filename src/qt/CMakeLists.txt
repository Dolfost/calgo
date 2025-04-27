find_package(Qt6 REQUIRED COMPONENTS 
	Core Widgets Gui
)

qt_standard_project_setup()

qt_add_library(ca_qt
	src/vecModel.cpp
	src/vecWidget.cpp
	src/matModel.cpp
	src/symmetricMatModel.cpp
	src/matWidget.cpp
	src/systemWidget.cpp
	src/simplexWidget.cpp
)
add_library(ca::qt ALIAS ca_qt)
target_sources(ca_qt
	PUBLIC FILE_SET HEADERS
			BASE_DIRS include/
			FILES 
				include/calgo/qt/vecModel.hpp
				include/calgo/qt/vecWidget.hpp
				include/calgo/qt/matModel.hpp
				include/calgo/qt/symmetricMatModel.hpp
				include/calgo/qt/matWidget.hpp
				include/calgo/qt/systemWidget.hpp
				include/calgo/qt/simplexWidget.hpp
)
set_target_properties(ca_qt PROPERTIES
	OUTPUT_NAME caqt
	EXPORT_NAME qt
)
target_compile_features(ca_qt
	PUBLIC cxx_std_${CALGO_CXX_STANDARD}
)

target_link_libraries(ca_qt
	PRIVATE ca::calgo
	PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui
)

if (CALGO_TESTING)
	add_subdirectory(tests)
endif()
