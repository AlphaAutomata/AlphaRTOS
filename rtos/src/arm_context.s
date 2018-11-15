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
