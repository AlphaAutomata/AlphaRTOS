#ifndef HAL_PLATFORM_H
#define HAL_PLATFORM_H

//! @defgroup HAL_PLATFORM
//! @brief Supported hardware platforms and the target platform selection.
//! @{

#define HAL_PLATFORM_NONE        0 //!< Target the compilation host for off-device testing.

//! @name Xilinx Zynq
//! @{
#define HAL_PLATFORM_XC7Zxxx  1 //!< Target Zynq-7000 SoC.
#define HAL_PLATFORM_XC7ZxxxS 2 //!< Target Zynq-7000S SoC.
//! @} @name Xilinx Zynq

#ifndef HAL_PLATFORM
#define HAL_PLATFORM HAL_PLATFORM_NONE //!< The selected underlying platform.
#endif

//! @} @defgroup HAL_PLATFORM

#endif // #ifndef HAL_PLATFORM_H
