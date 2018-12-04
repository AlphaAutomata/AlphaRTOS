##################
# Program Memory #
##################

set(ROM_BASE "0x00000000" CACHE STRING "ROM base address" FORCE)
set(ROM_SIZE "0x00010000" CACHE STRING "ROM size"         FORCE)

set(RAM_BASE "0x00010000" CACHE STRING "RAM base address" FORCE)
set(RAM_SIZE "0x00010000" CACHE STRING "RAM size"         FORCE)

set(RW_DATA_SIZE "0x00008000" CACHE STRING "Read/write data size"       FORCE)
set(ZI_DATA_SIZE "0x00004000" CACHE STRING "Zero-initialized data size" FORCE)

set(STACK_SIZE "0x00001000" CACHE STRING "Total stack area size" FORCE)
set(HEAP_SIZE  "0x00004000" CACHE STRING "Total heap area size"  FORCE)

set(UND_STACK_SIZE "0x00000100" CACHE STRING "Undefined-mode stack size"  FORCE)
set(ABT_STACK_SIZE "0x00000100" CACHE STRING "Abort-mode stack size"      FORCE)
set(SVC_STACK_SIZE "0x00000100" CACHE STRING "Supervisor-mode stack size" FORCE)
set(IRQ_STACK_SIZE "0x00000100" CACHE STRING "Interrupt-mode stack size"  FORCE)
set(FIQ_STACK_SIZE "0x00000100" CACHE STRING "Fast-data-mode stack size"  FORCE)

set(TTB_BASE "0x00020000" CACHE STRING "Translation table base address" FORCE)
set(TTB_SIZE "0x00004000" CACHE STRING "Translation table size"         FORCE)
