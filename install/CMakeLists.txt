include(GNUInstallDirs)

if(CALGO_CALGO)
	install(TARGETS ca_calgo ca_interpolation ca_linear ca_numeric ca_optimization
		EXPORT calgo
		FILE_SET HEADERS
	)
	if (CALGO_QT)
		install(TARGETS ca_qt
			EXPORT calgoqt
			FILE_SET HEADERS
		)
	endif()
endif()
if(CALGO_DOCS)
	install(DIRECTORY "${CMAKE_BINARY_DIR}/doc/out/html" 
		TYPE DOC
	)
	if(NOT WIN32)
		install(DIRECTORY "${CMAKE_BINARY_DIR}/doc/out/man/" 
			TYPE MAN
		)
	endif()
endif()

include(CMakePackageConfigHelpers)
if(CALGO_CALGO)
	install(EXPORT calgo 
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
		NAMESPACE ca::
	)
	if (CALGO_QT)
		install(EXPORT calgoqt
			DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
			NAMESPACE ca::
		)
	endif()
	write_basic_package_version_file(
		"${CMAKE_INSTALL_LIBDIR}/cmake/"
		COMPATIBILITY SameMajorVersion
	)
	configure_package_config_file(
		calgoConfig.cmake.in calgoConfig.cmake
		INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/"
	#	PATH_VARS vars
		NO_SET_AND_CHECK_MACRO
		NO_CHECK_REQUIRED_COMPONENTS_MACRO
	)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/calgoConfig.cmake"
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/"
	)
endif()
