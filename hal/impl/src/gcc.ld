MEMORY {
    EXROM (x)  : ORIGIN = __EXROM_BASE, LENGTH = __EXROM_SIZE
    DAROM (r)  : ORIGIN = __DAROM_BASE, LENGTH = __DAROM_SIZE
    DARAM (rw) : ORIGIN = __DARAM_BASE, LENGTH = __BASTC_SIZE
    PGRAM (rw) : ORIGIN = __PGRAM_BASE, LENGTH = __PGRAM_SIZE
}

ENTRY(reset_handler)

SECTIONS {
    .text : {
        *(RESET)
        KEEP(*(.isr_vector))

        *(.text)

        .*(SORT(.ctors))
        .*(SORT(.dtors))
    } > EXROM

    .rodata ALIGN(0x400) : {
        *(.rodata)
    } > DAROM

    .data ALIGN(0x400) : {
        *(.data)
    } > DARAM

    .bss ALIGN(0x400) : {
        *(.bss)
    } > DARAM

    .stack (NOLOAD) : {
        __priv_stacks_top = .;

        . = ORIGIN(PGRAM) +
            LENGTH(PGRAM) -
            __FIQ_STACK_SIZE -
            __IRQ_STACK_SIZE -
            __SVC_STACK_SIZE -
            __ABT_STACK_SIZE -
            __UND_STACK_SIZE;
        . = ALIGN(8);

        __fiq_stack_top = .;
        . += __FIQ_STACK_SIZE;
        __fiq_stack_bot = .;
        
        __irq_stack_top = .;
        . += __IRQ_STACK_SIZE;
        __irq_stack_bot = .;
        
        __svc_stack_top = .;
        . += __SVC_STACK_SIZE;
        __svc_stack_bot = .;
        
        __abt_stack_top = .;
        . += __ABT_STACK_SIZE;
        __abt_stack_bot = .;
        
        __und_stack_top = .;
        . += __UND_STACK_SIZE;
        __und_stack_bot = .;

        __priv_stacks_bot = .;
    } > PGRAM
}
