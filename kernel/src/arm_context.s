// offset macros to access registers in regframe_t
// must update if regframe_t definition in arm_types.h is changed
.equ OFF_R0  ,            0
.equ OFF_R1  ,            4
.equ OFF_R2  ,            8
.equ OFF_R3  ,            12
.equ OFF_R4  ,            16
.equ OFF_R5  ,            20
.equ OFF_R6  ,            24
.equ OFF_R7  ,            28
.equ OFF_R8  ,            32
.equ OFF_R9  ,            36
.equ OFF_R10 ,            40
.equ OFF_R11 ,            44
.equ OFF_R12 ,            48
.equ OFF_SP  ,            52
.equ OFF_LR  ,            56
.equ OFF_PC  ,            60
.equ OFF_xPSR,            64

// offset macros to access fields in struct task
// must change if struct task definition changes in scheduler.h
.equ taskStatus   , 0
.equ taskTaskEntry, 4
.equ taskFrame    , 8

// size macros for regframe_t and sizeof(struct task)
// must change if struct task or regframe_t definitions change
// struct task	defined in task.h
// regframe_t	defined in arm_types.h
.equ regframe_t_offset, 8
.equ regframe_t_size  , 68
.equ structTaskSize   , regframe_t_offset+regframe_t_size

.macro mov32, reg, val
	movw	\reg, #:lower16:\val
	movt	\reg, #:upper16:\val
.endm

	.arm
	.section ".text"
	.align 4

	.global switchContext
	.type switchContext, "function"
	.fnstart
	.cantunwind
switchContext:
		// store registers
		STMIA	R1, {R0-R12,SP,LR}		    // save registers 0-12, SP, and LR to *oldframe
		MRS		R2, CPSR				    // get the current PSR
		STR		R2, [R1,#OFF_xPSR]		    // save the APSR register in the xPSR field

		ADD		R0, #OFF_R2				    // cannot load R0 yet since it holds &newframe, hold off
										    // on loading R1 to use it for the xPSR
		LDMIA	R0, {R2-R12,SP,LR}		    // load registers 2-12, SP, and LR from *newframe
		LDR		R1, [R0,#(OFF_xPSR-OFF_R2)]	// load the xPSR into R1
		MSR		CPSR, R1			        // write the PSR
		SUB		R0, #OFF_R2				    // go back to the beginning of struct to load R0 and R1
		LDMIA	R0, {R0-R1}				    // load R0 and R1 from struct

		BX		LR						    // execute return
	.fnend
	.size switchContext, .-switchContext

.end
