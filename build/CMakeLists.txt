# This minimum version ensures that we can enforce C99 standards in a cross-platform way
cmake_minimum_required(VERSION 3.1)
project(AlphaRTOS CXX C ASM)

set(AlphaRTOS_K_VERSION_MAJOR 0)
set(AlphaRTOS_K_VERSION_MINOR 0)

# linker script path
#set(LINKER_SCRIPT xc7z.ld)

# enable link-time optimization and optimize for size
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -flto -nostartfiles -fno-exceptions")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Os -flto -nostartfiles")

# enable link-time optimization and set the linker script
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")# -T ${LINKER_SCRIPT}")

set(ARTOS_ROOT_DIR ..)
set(ARTOS_PUBLIC_DIR ${ARTOS_ROOT_DIR}/api)
set(ARTOS_KERNEL_DIR ${ARTOS_ROOT_DIR}/kernel)

set(KERNEL_INC_DIR ${ARTOS_KERNEL_DIR}/include)
set(KERNEL_SRC_DIR ${ARTOS_KERNEL_DIR}/src)

configure_file(
	"${ARTOS_ROOT_DIR}/config/cmake_config.h.in"
	"${ARTOS_PUBLIC_DIR}/cmake_config.h"
)

file(
	GLOB K_INC_FILES
	${KERNEL_INC_DIR}/*.h
	${ARTOS_PUBLIC_DIR}/*.h
)
file(
	GLOB K_SRC_FILES
	${KERNEL_SRC_DIR}/*.c
)

add_library(
	artos STATIC
	${K_INC_FILES}
	${K_SRC_FILES}
)
target_include_directories(
	artos PUBLIC
	${KERNEL_INC_DIR}
	${ARTOS_PUBLIC_DIR}
)
set_target_properties(
	artos PROPERTIES
	C_STANDARD_REQUIRED 99 # Enforce C99 minimum for all sorts of niceties
)
