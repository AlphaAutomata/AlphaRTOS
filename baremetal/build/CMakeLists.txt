# from 3.3 onwards, detecting whether a string exists in a list has a new syntax using if()
cmake_minimum_required(VERSION 3.3)

######################
# Derive Directories #
######################

# get bare-metal HAL root directory
get_filename_component(HAL_BAREMETAL_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
# get HAL root directory
get_filename_component(HAL_ROOT_DIR ${HAL_BAREMETAL_DIR} DIRECTORY)
# get repository parent directory
get_filename_component(REPO_PARENT_DIR ${HAL_ROOT_DIR} DIRECTORY)

# get HAL dependency directories
set(HAL_BAREMETAL_INC_DIR ${HAL_BAREMETAL_DIR}/include)
set(HAL_BAREMETAL_SRC_DIR ${HAL_BAREMETAL_DIR}/src)

# get CMSIS dependency directories
set(CMSIS_ROOT_DIR       ${REPO_PARENT_DIR}/AlphaCMSIS)
set(CMSIS_Core_A_INC_DIR ${CMSIS_ROOT_DIR}/CMSIS/Core_A/Include)
set(CMSIS_Core_A_SRC_DIR ${CMSIS_ROOT_DIR}/CMSIS/Core_A/Source)
set(CMSIS_RTOS2_DIR      ${CMSIS_ROOT_DIR}/CMSIS/RTOS2)
set(CMSIS_DEVICE_INC_DIR ${CMSIS_ROOT_DIR}/Device/ARM/${ARCH}/Include)
set(CMSIS_DEVICE_SRC_DIR ${CMSIS_ROOT_DIR}/Device/ARM/${ARCH}/Source)

set(CMSIS_RTOS2_INC_DIR ${CMSIS_RTOS2_DIR}/Include)
set(CMSIS_RTOS2_SRC_DIR ${CMSIS_RTOS2_DIR}/Source)

# get BSP include and lib paths
if(NOT XSDK_VERSION STREQUAL "NONE")
	set(BSP_ROOT_DIR ${XSDK_WORKSPACE}/${XSDK_BSP})
	
	set(BSP_CPU0_DIR ${BSP_ROOT_DIR}/ps7_cortexa9_0)
	set(BSP_CPU1_DIR ${BSP_ROOT_DIR}/ps7_cortexa9_1)
	
	set(BSP_CPU0_INC_DIR ${BSP_CPU0_DIR}/include)
	set(BSP_CPU0_LIB_DIR ${BSP_CPU0_DIR}/lib)
	
	set(BSP_CPU1_INC_DIR ${BSP_CPU1_DIR}/include)
	set(BSP_CPU1_LIB_DIR ${BSP_CPU1_DIR}/lib)
endif()

#####################################
# Generate CMSIS Environment Header #
#####################################

configure_file(
	"${HAL_BAREMETAL_DIR}/config/RTE_Components.h.in"
	"${HAL_BAREMETAL_INC_DIR}/RTE_Components.h"
)

#########################
# Gather Target Sources #
#########################

# all HAL dependency files
set(
	HAL_INC_FILES
		${HAL_BAREMETAL_INC_DIR}/hal_platform.h
		${HAL_BAREMETAL_INC_DIR}/hal_core.h
		${HAL_BAREMETAL_INC_DIR}/hal_systick.h
		${HAL_BAREMETAL_INC_DIR}/hal_timer_gp.h
)
set(
	HAL_SRC_FILES
		${HAL_BAREMETAL_SRC_DIR}/hal_core.c
		${HAL_BAREMETAL_SRC_DIR}/hal_systick.c
		${HAL_BAREMETAL_SRC_DIR}/hal_timer_gp.c
)
# include assembly only when cross compiling
if(CMAKE_CROSSCOMPILING)
	message(STATUS "Configuring cross-compile build system.")
	# get the assembly sources
	set(HAL_ASM_FILES ${HAL_BAREMETAL_SRC_DIR}/arm_context.s)
else()
	message(STATUS "Configuring for compilation targeting the host system.")
	# set blank assembly sources
	set(HAL_ASM_FILES "")
endif()

# all CMSIS dependency files
set(
	CMSIS_Core_A_INC_FILES
		${CMSIS_Core_A_INC_DIR}/cmsis_compiler.h
		${CMSIS_Core_A_INC_DIR}/core_ca.h
		${CMSIS_Core_A_INC_DIR}/irq_ctrl.h
)
set(
	CMSIS_CORE_A_SRC_FILES
		${CMSIS_Core_A_SRC_DIR}/irq_ctrl_gic.c
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
set(
	CMSIS_DEVICE_SRC_FILES
		${CMSIS_DEVICE_SRC_DIR}/mmu_${ARCH}.c
		${CMSIS_DEVICE_SRC_DIR}/system_${ARCH}.c
		${CMSIS_DEVICE_SRC_DIR}/GCC/startup_${ARCH}.c
)

# Xilinx BSP archive
if(NOT XSDK_VERSION STREQUAL NONE)
	add_library(libxil STATIC IMPORTED)
	set_target_properties(libxil PROPERTIES IMPORTED_LOCATION ${BSP_CPU0_LIB_DIR}/libxil.a)
endif()

####################
# Configure Target #
####################

# add the platform constant to HAL files only
set_source_files_properties(
	${HAL_INC_FILES}
	${HAL_SRC_FILES}
	PROPERTIES
		COMPILE_DEFINITIONS
			HAL_PLATFORM=HAL_PLATFORM_${ARTOS_TARGET_PLATFORM}
)
# compile bare-metal HAL as a static library
add_library(
	hal_baremetal
	STATIC
		${HAL_INC_FILES}
		${HAL_SRC_FILES}
		${HAL_ASM_FILES}
		${CMSIS_Core_A_INC_FILES}
		${CMSIS_CORE_A_SRC_FILES}
		${CMSIS_RTOS2_INC_FILES}
		${CMSIS_DEVICE_INC_FILES}
		${CMSIS_DEVICE_SRC_FILES}
)
# custom command to pre-process the CMSIS linker script
if(CMSIS_LINKER_SCRIPT)
	add_custom_command(
		TARGET hal_baremetal PRE_LINK
		COMMAND
			${CMAKE_C_PREPROCESSOR}
				${CMSIS_LINKER_RAW}
				-P
				-o ${CMSIS_LINKER_PROCESSED}
				-I ${CMSIS_DEVICE_INC_DIR}
		DEPENDS
			${CMSIS_LINKER_RAW}
	)
endif()
# set include directories
target_include_directories(
	hal_baremetal
	PUBLIC
		${HAL_BAREMETAL_INC_DIR}
		${CMSIS_Core_A_INC_DIR}
		${CMSIS_DEVICE_INC_DIR}
	PRIVATE
		${CMSIS_RTOS2_INC_DIR}
)
# use C11 for standard atomics
set_target_properties(
	hal_baremetal
	PROPERTIES
		C_STANDARD   11
		CXX_STANDARD 11
		C_STANDARD_REQUIRED   TRUE
		CXX_STANDARD_REQUIRED TRUE
)
# link to Xilinx BSP
if(NOT XSDK_VERSION STREQUAL "NONE")
	target_include_directories(
		hal_baremetal
		PRIVATE
			${BSP_CPU0_INC_DIR}
	)
	target_link_libraries(hal_baremetal libxil)
endif()

# configure optional test application target
if(HAL_BAREMETAL_TEST)
	set(HAL_BAREMETAL_TEST_DIR ${HAL_BAREMETAL_DIR}/test)
	
	add_executable(
		hal_baremetal_test
			${HAL_BAREMETAL_TEST_DIR}/main.c
	)
	target_link_libraries(hal_baremetal_test hal_baremetal)
endif()
