# compile for bare metal ARM
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# toolchain binary path and toolchain exacutable prefix
if(WIN32)
	set(TC_PATH       "C:/CC_TC/gcc-arm-8.2-2018.08-i686-mingw32-arm-eabi/bin/")
	set(EXTENSION     ".exe")
	set(CROSS_COMPILE arm-eabi-)
else()
	set(TC_PATH       "")
	set(CROSS_COMPILE arm-none-eabi-)
	set(EXTENSION     "")
endif()

# set compilers, linker, and archiver
set(CMAKE_ASM_COMPILER "${TC_PATH}${CROSS_COMPILE}as${EXTENSION}")
set(CMAKE_C_COMPILER   "${TC_PATH}${CROSS_COMPILE}gcc${EXTENSION}")
set(CMAKE_CXX_COMPILER "${TC_PATH}${CROSS_COMPILE}g++${EXTENSION}")
set(CMAKE_LINKER       "${TC_PATH}${CROSS_COMPILE}ld${EXTENSION}")
set(CMAKE_AR           "${TC_PATH}${CROSS_COMPILE}ar${EXTENSION}")

# force convert .elf executables into .bin raw binary maps
set(
	CMAKE_OBJCOPY "$(TC_PATH)$(CROSS_COMPILE)objcopy"
	CACHE FILEPATH "The toolchain command objcopy command " FORCE
)

# force compile test library instead of a test executable to avoid testing for startup code, etc.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# disable various system calls
set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")

# do not search for binaries on compile host
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# do not use system includes and libraries
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# specify core and FPU architectures
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a9")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon-fp16")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=cortex-a9")

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a9")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon-fp16")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mtune=cortex-a9")

# cache the flags
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}"   CACHE STRING "CFLAGS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "CXXFLAGS")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}"   CACHE STRING "")
