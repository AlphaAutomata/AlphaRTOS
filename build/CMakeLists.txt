# version required for consistent inter-procedural optimization behavior
cmake_minimum_required(VERSION 3.9)

####################################
# Set Project-Level Configurations #
####################################

project(AlphaRTOS)

# check for inter-procedural optimization support
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)

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

# get the Alpha RTOS repsitory root directory
get_filename_component(ARTOS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
# get the directory this repository, and all related repositories, reside in
get_filename_component(ARTOS_PARENT_DIR ${ARTOS_ROOT_DIR} DIRECTORY)

# get top level directories containing code
set(ARTOS_PUBLIC_DIR ${ARTOS_ROOT_DIR}/api)
set(ARTOS_KERNEL_DIR ${ARTOS_ROOT_DIR}/kernel)

# get directories containing internal code
set(KERNEL_INC_DIR ${ARTOS_KERNEL_DIR}/include)
set(KERNEL_SRC_DIR ${ARTOS_KERNEL_DIR}/src)

# get HAL dependency directories
set(HAL_ROOT_DIR ${ARTOS_PARENT_DIR}/hal)
set(HAL_RTOS_DIR ${HAL_ROOT_DIR}/rtos)
set(HAL_RTOS_INC_DIR ${HAL_RTOS_DIR}/include)
set(HAL_RTOS_SRC_DIR ${HAL_RTOS_DIR}/src)

# get CMSIS dependency directories
set(CMSIS_ROOT_DIR       ${ARTOS_PARENT_DIR}/AlphaCMSIS)
set(CMSIS_Core_A_INC_DIR ${CMSIS_ROOT_DIR}/CMSIS/Core_A/Include)
set(CMSIS_RTOS2_DIR      ${CMSIS_ROOT_DIR}/CMSIS/RTOS2)
set(CMSIS_DEVICE_INC_DIR ${CMSIS_ROOT_DIR}/Device/ARM/${ARCH}/Include)
set(CMSIS_DEVICE_SRC_DIR ${CMSIS_ROOT_DIR}/Device/ARM/${ARCH}/Source)

set(CMSIS_RTOS2_INC_DIR ${CMSIS_RTOS2_DIR}/Include)
set(CMSIS_RTOS2_SRC_DIR ${CMSIS_RTOS2_DIR}/Source)

###########################
# Generate Version Header #
###########################

configure_file(
	"${ARTOS_ROOT_DIR}/config/cmake_config.h.in"
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
# include assembly and special optimizations only when cross compiling
if(CMAKE_CROSSCOMPILING)
	message(STATUS "Configuring cross-compile build system.")
	# get the assembly sources
	set(K_ASM_FILES ${KERNEL_SRC_DIR}/arm_context.s)
else()
	message(STATUS "Configuring for compilation targeting the host system.")
	# set blank assembly sources
	set(K_ASM_FILES "")
endif()

# all HAL dependency files
set(
	HAL_INC_FILES
		${HAL_RTOS_INC_DIR}/hal_platform.h
		${HAL_RTOS_INC_DIR}/hal_core.h
		${HAL_RTOS_INC_DIR}/hal_systick.h
		${HAL_RTOS_INC_DIR}/hal_timer_gp.h
)
set(
	HAL_SRC_FILES
		${HAL_RTOS_SRC_DIR}/hal_systick.c
		${HAL_RTOS_SRC_DIR}/hal_timer_gp.c
)

# all CMSIS dependency files
set(
	CMSIS_Core_A_INC_FILES
		${CMSIS_Core_A_INC_DIR}/core_ca.h
)
set(
	CMSIS_RTOS2_INC_FILES
		${CMSIS_RTOS2_INC_DIR}/cmsis_os2.h
		${CMSIS_RTOS2_INC_DIR}/os_tick.h
)
set(
	CMSIS_DEVICE_INC_FILES
		${CMSIS_DEVICE_INC_DIR}/${ARCH}.h
		${CMSIS_DEVICE_INC_DIR}/mem_${ARCH}.h
		${CMSIS_DEVICE_INC_DIR}/system_${ARCH}.h
)

####################
# Configure Target #
####################

# compile RTOS kernel as static library
add_library(
	artos STATIC
		${ARTOS_API_HEADERS}
		${K_INC_FILES}
		${K_SRC_FILES}
		${K_ASM_FILES}
		${HAL_INC_FILES}
		${HAL_SRC_FILES}
		${CMSIS_Core_A_INC_FILES}
		${CMSIS_RTOS2_INC_FILES}
		${CMSIS_DEVICE_INC_FILES}
)
# set include directories, making only the public API header directory public
target_include_directories(
	artos
	PUBLIC
		${ARTOS_PUBLIC_DIR}
	PRIVATE
		${KERNEL_INC_DIR}
		${HAL_RTOS_INC_DIR}
		${CMSIS_Core_A_INC_DIR}
		${CMSIS_RTOS2_INC_DIR}
		${CMSIS_DEVICE_INC_DIR}
)
# define platform architecture constant
target_compile_definitions(
	artos
	PUBLIC
		-DHAL_PLATFORM=HAL_PLATFORM_${ARTOS_TARGET_PLATFORM}
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
# use CMSIS linker script
if(CMAKE_CROSSCOMPILING)
	set_directory_properties(
		artos
		PROPERTIES
			LINK_DEPENDS ${CMSIS_DEVICE_SRC_DIR}/${ARCH}.ld
	)
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
	set(DOCS_ROOT_DIR ${ARTOS_ROOT_DIR}/docs)
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
