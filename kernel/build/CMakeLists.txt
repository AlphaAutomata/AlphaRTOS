# from 3.3 onwards, detecting whether a string exists in a list has a new syntax using if()
cmake_minimum_required(VERSION 3.3)

####################################
# Set Project-Level Configurations #
####################################

project(AlphaRTOS C ASM)

# set the policy to enable inter-procedural optimization if available
if(POLICY CMP0069)
	cmake_policy(SET CMP0069 NEW)
endif()
if(NOT DEFINED IPO_SUPPORTED)
	# check for inter-procedural optimization support
	include(CheckIPOSupported OPTIONAL RESULT_VARIABLE HAS_CHECK_IPO)
	if(HAS_CHECK_IPO)
		message(STATUS "Determining inter-procedural optimization support.")
		
		check_ipo_supported(RESULT IPO)
		
		if(IPO)
			message(STATUS "Inter-procedural optimization enabled.")
		else()
			message(STATUS "Inter-procedural optimization disabled.")
		endif()
	else()
		message(STATUS "Cannot determine inter-procedural optimization support.")
		message(STATUS "Inter-procedural optimization is disabled.")
		
		set(IPO FALSE)
	endif()
	
	set(IPO_SUPPORTED ${IPO} CACHE STRING "Inter-procedural optimzation support.")
endif()

set(AlphaRTOS_K_VERSION_MAJOR 0)
set(AlphaRTOS_K_VERSION_MINOR 0)
set(AlphaRTOS_K_VERSION_PATCH 0)

# The toolchain script sets these variables. If not cross compiling, we still need values here.
if(NOT DEFINED ARCH)
	set(ARCH ARMCA9)
endif()
if(NOT DEFINED ARTOS_TARGET_PLATFORM)
	set(ARTOS_TARGET_PLATFORM NONE)
endif()

######################
# Derive Directories #
######################

# get the Alpha RTOS repository root directory
get_filename_component(ARTOS_KERNEL_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
# get the directory this repository, and all related repositories, reside in
get_filename_component(REPO_PRENT_DIR ${ARTOS_KERNEL_DIR} DIRECTORY)

# get top level directories containing code
set(ARTOS_PUBLIC_DIR ${ARTOS_KERNEL_DIR}/api)
set(ARTOS_IMPL_DIR   ${ARTOS_KERNEL_DIR}/impl)
set(ARTOS_TEST_DIR   ${ARTOS_KERNEL_DIR}/test)

# get directories containing internal code
set(KERNEL_INC_DIR ${ARTOS_IMPL_DIR}/include)
set(KERNEL_SRC_DIR ${ARTOS_IMPL_DIR}/src)

# get directories containing test code
set(TEST_INC_DIR ${ARTOS_TEST_DIR}/include)
set(TEST_SRC_DIR ${ARTOS_TEST_DIR}/src)

# get HAL dependency directories
set(HAL_ROOT_DIR ${REPO_PRENT_DIR}/hal)

# get CMSIS dependency directories
set(CMSIS_ROOT_DIR       ${REPO_PRENT_DIR}/AlphaCMSIS)
set(CMSIS_Core_A_INC_DIR ${CMSIS_ROOT_DIR}/CMSIS/Core_A/Include)
set(CMSIS_RTOS2_DIR      ${CMSIS_ROOT_DIR}/CMSIS/RTOS2)

set(CMSIS_RTOS2_INC_DIR ${CMSIS_RTOS2_DIR}/Include)
set(CMSIS_RTOS2_SRC_DIR ${CMSIS_RTOS2_DIR}/Source)

#################
# Miscellaneous #
#################

configure_file(
	"${ARTOS_KERNEL_DIR}/config/cmake_config.h.in"
	"${ARTOS_PUBLIC_DIR}/cmake_config.h"
)

#########################
# Gather Target Sources #
#########################

# all public API headers
set(
	ARTOS_API_HEADERS
		${ARTOS_PUBLIC_DIR}/AlphaRTOS_config.h
		${ARTOS_PUBLIC_DIR}/AlphaRTOS_types.h
		${ARTOS_PUBLIC_DIR}/AlphaRTOS.h
		${ARTOS_PUBLIC_DIR}/artos_compiler.h
		${ARTOS_PUBLIC_DIR}/artos_event_data_types.h
		${ARTOS_PUBLIC_DIR}/artos_event_types.h
		${ARTOS_PUBLIC_DIR}/artos_gpio_types.h
		${ARTOS_PUBLIC_DIR}/artos_i2c_types.h
		${ARTOS_PUBLIC_DIR}/artos_service_types.h
		${ARTOS_PUBLIC_DIR}/artos_spi_types.h
		${ARTOS_PUBLIC_DIR}/artos_task_msg_types.h
		${ARTOS_PUBLIC_DIR}/artos_timer_types.h
		${ARTOS_PUBLIC_DIR}/artos_uart_types.h
)

# all kernel internal files
set(
	K_INC_FILES
		${KERNEL_INC_DIR}/api_contract.h
		${KERNEL_INC_DIR}/circular_buffer.h
		${KERNEL_INC_DIR}/concurrency.h
		${KERNEL_INC_DIR}/event_service.h
		${KERNEL_INC_DIR}/scheduler.h
)
set(
	K_SRC_FILES
		${KERNEL_SRC_DIR}/AlphaRTOS.c
		${KERNEL_SRC_DIR}/circular_buffer.c
		${KERNEL_SRC_DIR}/event_service.c
		${KERNEL_SRC_DIR}/scheduler.c
)

# all test files
set(
	TEST_INC_FILES
		""
)
set(
	TEST_SRC_FILES
		${TEST_SRC_DIR}/kernel_test.c
)

###################################
# Configure Kernel Library Target #
###################################

# compile RTOS kernel as static library
add_library(
	artos
	STATIC
		${ARTOS_API_HEADERS}
		${K_INC_FILES}
		${K_SRC_FILES}
)
# set include directories, making only the public API header directory public
target_include_directories(
	artos
	PUBLIC
		${ARTOS_PUBLIC_DIR}
	PRIVATE
		${KERNEL_INC_DIR}
		${HAL_RTOS_INC_DIR}
)
# define platform architecture constant
target_compile_definitions(
	artos
	PUBLIC
		HAL_PLATFORM=HAL_PLATFORM_${ARTOS_TARGET_PLATFORM}
)
# use link-time optimization if available, and use C11 for standard atomics
set_target_properties(
	artos
	PROPERTIES
		INTERPROCEDURAL_OPTIMIZATION ${IPO_SUPPORTED}
		C_STANDARD   11
		CXX_STANDARD 11
		C_STANDARD_REQUIRED   TRUE
		CXX_STANDARD_REQUIRED TRUE
)

# link against HAL library
add_subdirectory(${HAL_ROOT_DIR}/build hal)
target_link_libraries(artos hal)

####################################
# Configure Test Executable Target #
####################################

if(ARTOS_TEST_TARGET)
	message(STATUS "Creating test executable target.")
	
	# compile executable operating system
	add_executable(
		artos_os
			${TEST_INC_FILES}
			${TEST_SRC_FILES}
	)
	# use link-time optimization if available, and use C11 for standard atomics
	set_target_properties(
		artos
		PROPERTIES
			INTERPROCEDURAL_OPTIMIZATION ${IPO_SUPPORTED}
			C_STANDARD   11
			CXX_STANDARD 11
			C_STANDARD_REQUIRED   TRUE
			CXX_STANDARD_REQUIRED TRUE
	)
	# link against RTOS kernel library and the HAL
	target_link_libraries(artos_os artos)
else()
	message(STATUS "Test executable target not selected.")
endif()

############################
# Configure Doxygen Target #
############################

# check if Doxygen is installed
find_program(DOXYGEN doxygen)

# configure documentation target
if(DOXYGEN STREQUAL "DOXYGEN-NOTFOUND")
	message(STATUS "No Doxygen installation found. Documentation target will not be created.")
else()
	message(STATUS "Doxygen installation found. A documentation target will be created.")
	
	# get documentation directory
	set(DOCS_ROOT_DIR ${ARTOS_KERNEL_DIR}/docs)
	set(DOCS_HTML_DIR ${DOCS_ROOT_DIR}/html)
	
	# set files to generated documentation from
	set(
		DOCUMENTED_FILES
			${ARTOS_API_HEADERS}
			${K_INC_FILES}
			${K_SRC_FILES}
	)
	# convert CMake files list to a space-separated list for Doxygen
	string(REPLACE ";" " " DOX_INPUT "${DOCUMENTED_FILES}")
	
	# check if GraphViz dot is installed
	find_program(DOT dot)
	# set dot existence flag and dot installation path
	if(DOT STREQUAL "DOT-NOTFOUND")
		message(STATUS "GraphViz dot tool not found. Configuring Doxygen to use native graphs.")
		set(DOX_HAVE_DOT NO)
		set(DOX_DOT_PATH "")
	else()
		message(STATUS "GraphViz dot tool found. Configuring Doxygen to use dot graphs.")
		set(DOX_HAVE_DOT YES)
		set(DOX_DOT_PATH DOT)
	endif()
	
	# generate the Doxygen configuration file
	configure_file(
		"${DOCS_ROOT_DIR}/Doxyfile.in"
		"${DOCS_ROOT_DIR}/Doxyfile"
	)
	
	# create custom command to generate documentation dependent on source files
	add_custom_command(
		OUTPUT
			${DOCS_HTML_DIR}/index.html
		COMMAND
			${DOXYGEN}
		WORKING_DIRECTORY
			${DOCS_ROOT_DIR}
		DEPENDS
			${DOCUMENTED_FILES}
			${DOCS_ROOT_DIR}/Doxyfile
	)
	
	# create documentation target
	add_custom_target(docs DEPENDS ${DOCS_HTML_DIR}/index.html)
endif()
