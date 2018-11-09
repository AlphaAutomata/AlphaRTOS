# This minimum version ensures that we can enforce C99 standards in a cross-platform way
cmake_minimum_required(VERSION 3.1)

set(AlphaRTOS_K_VERSION_MAJOR 0)
set(AlphaRTOS_K_VERSION_MINOR 0)

set(ARTOS_ROOT_DIR ..)
set(ARTOS_PUBLIC_DIR ${ARTOS_ROOT_DIR}/api/)
set(ARTOS_KERNEL_DIR ${ARTOS_ROOT_DIR}/kernel/)

set(KERNEL_INC_DIR ${ARTOS_KERNEL_DIR}/include/)
set(KERNEL_SRC_DIR ${ARTOS_KERNEL_DIR}/src/)

set(HAL_ROOT_DIR ${ARTOS_ROOT_DIR}/../hal)
set(HAL_RTOS_DIR ${HAL_ROOT_DIR}/rtos)
set(HAL_RTOS_INC_DIR ${HAL_RTOS_DIR}/include/)
set(HAL_RTOS_SRC_DIR ${HAL_RTOS_DIR}/src/)

set(CMSIS_ROOT_DIR ${ARTOS_ROOT_DIR}/../AlphaCMSIS)
set(CMSIS_RTOS2_DIR ${CMSIS_ROOT_DIR}/CMSIS/RTOS2)

set(CMSIS_RTOS2_INC_DIR ${CMSIS_RTOS2_DIR}/include/)
set(CMSIS_RTOS2_SRC_DIR ${CMSIS_RTOS2_DIR}/Source/)

project(AlphaRTOS)

# include assembly and special optimizations only when cross compiling
if(CMAKE_CROSSCOMPILING)
	message(STATUS "Configuring cross-compile build system.")
	
	# get the assembly sources
	set(K_ASM_FILES ${KERNEL_SRC_DIR}/arm_context.s)

	# enable link-time optimization and optimize for size
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -flto -nostartfiles -fno-exceptions")
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Os -flto -nostartfiles")

	# enable link-time optimization and set the linker script
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
else()
	message(STATUS "Configuring for compilation targeting the host system.")
	
	# set blank assembly sources
	set(K_ASM_FILES "")
endif()

configure_file(
	"${ARTOS_ROOT_DIR}/config/cmake_config.h.in"
	"${ARTOS_PUBLIC_DIR}/cmake_config.h"
)

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

set(
	HAL_INC_FILES
		${HAL_RTOS_INC_DIR}/hal_systick.h
		${HAL_RTOS_INC_DIR}/hal_timer_gp.h
)
set(
	HAL_SRC_FILES
		${HAL_RTOS_SRC_DIR}/hal_systick.c
		${HAL_RTOS_SRC_DIR}/hal_timer_gp.c
)

set(
	CMSIS_RTOS2_INC_FILES
		${CMSIS_RTOS2_INC_DIR}/cmsis_os2.h
		${CMSIS_RTOS2_INC_DIR}/os_tick.h
)

add_library(
	artos STATIC
		${ARTOS_API_HEADERS}
		${K_INC_FILES}
		${K_SRC_FILES}
		${K_ASM_FILES}
		${HAL_INC_FILES}
		${HAL_SRC_FILES}
		${CMSIS_RTOS2_INC_FILES}
)
target_include_directories(
	artos
	PUBLIC
		${ARTOS_PUBLIC_DIR}
	PRIVATE
		${KERNEL_INC_DIR}
		${HAL_RTOS_INC_DIR}
		${CMSIS_RTOS2_INC_DIR}
)
set_target_properties(
	artos PROPERTIES
	C_STANDARD_REQUIRED 99 # Enforce C99 minimum for all sorts of niceties
)
