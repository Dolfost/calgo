message("==>> Configuring documentation...")
execute_process( # configuration
	COMMAND "${CMAKE_COMMAND}" $ENV{CONFIGURATION_OPTIONS} -B build -S . 
	-DCALGO_DOCS=YES -DCALGO_TESTING=NO -DCALGO_CALGO=NO
	WORKING_DIRECTORY "$ENV{REPO}"
	COMMAND_ERROR_IS_FATAL ANY
)

file(MAKE_DIRECTORY "$ENV{REPO}/build")

message("==>> Building documentation...")
execute_process( # building
	COMMAND "${CMAKE_COMMAND}" --build build -t doc
	WORKING_DIRECTORY "$ENV{REPO}"
	COMMAND_ERROR_IS_FATAL ANY
)
