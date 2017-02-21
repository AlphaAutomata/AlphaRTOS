	AREA	FLASH, CODE, READONLY
			
runTask PROC
		EXPORT	runTask
		
		STMIA	R1, {R0-R12}	; save registers 0-12 to *oldframe
		MRS		R3, APSR		; get the APSR
		ADD		R1, #52			; increment pointer to SP field in struct
		STR		SP, [R1], #4	; save stack pointer in the SP field, increment to LR field
		STR		LR, [R1], #8	; save the link register in the LR field
		STR		R3, [R1]		; save the APSR into the xPSR field
		
		MOV		R4, R2			; save the branch address from the third function argument
		ADD		R0, #4			; we cannot load R0 yet since that's the sturct *newframe address
		LDMIA	R0, {R1-R3}		; load registers 1-3 from *newframe to pass parameters
		ADD		R0, #48			; increment pointer to SP field in struct
		LDR		SP, [R0], #4	; load the new stack pointer
		LDR		LR, [R0]		; load the link register so it returns to where we want it to
		SUB		R0, #56			; go back to the beginning of the struct to load R0
		LDR		R0, [R0]		; load R0 from struct
		
		BX		R4				; call into the new context
		
		ENDP
		
switchContext PROC
		EXPORT	switchContext
		STMIA	R1, {R0-R12}	; save registers 0-12 to *oldframe
		MRS		R2, APSR		; get the current APSR
		ADD		R1, #52			; increment pointer to SP field in struct
		STR		SP, [R1], #4	; save stack pointer in the SP field, increment to LR field
		STR		LR, [R1], #8	; save the link register in the LR field
		STR		R2, [R1]		; save the APSR register in the xPSR field
		
		ADD		R0, #8			; cannot load R0 yet since it holds *newframe, hold off on loading R1 to use it for the xPSR
		LDMIA	R0, {R2-R12}	; load registers 2-12 from *newframe
		ADD		R0, #44			; increment pointer to SP field in struct
		LDR		SP, [R0], #4	; load the new stack pointer
		LDR		LR, [R0], #8	; load the link register
		LDR		R1, [R0]		; load the xPSR into R1
		MSR		APSR, R1		; store the APSR
		SUB		R0, #64			; go back to the beginning of the struct to load R0
		LDMIA	R0, {R0-R1}		; load R0 from struct
		
		BX		LR				; execute return
		
		ENDP
		
SysTick_Handler PROC
		EXPORT SysTick_Handler
		IMPORT taskTable
		IMPORT currTaskID
		IMPORT uptime
		IMPORT runScheduler
		IMPORT kframe
		
		; increment uptime
		LDR		R0, =uptime
		LDR		R1, [R0]
		ADD		R1, #1
		STR		R1, [R0]
		
		; signal scheduler to run
		LDR		R0, =runScheduler
		MOV		R1, #1
		STR		R1, [R0]
		
		; if a user task is running, preempt with context switch
		LDR		R0, =currTaskID
		LDR		R1, [R0]
		CMP		R1, #0
		BEQ		return
		
		; preempt running task by dropping out of the ISR into a context switch routine
		LDMIA	SP, {R0-R3,R12}	; load the general purpose registers from IRQ stack
		SUB		SP, #4			; allocate a 32-bit word on stack, which we won't restore. This effectively moves the IRQ stack by a word. 
		STMIA	SP, {R0-R3,R12}	; store the general purpose registers to shifted IRQ stack
		
		LDR		R0, [SP,#24]	; load the LR from unshifted IRQ stack location
		LDR		R1, [SP,#28]	; load the PC from unshifted IRQ stack location
		LDR		R2, [SP,#32]	; load the xPSR from unshifted IRQ stack location
		
		STR		R0, [SP,#20]	; store the LR into shifted IRQ stack location
		STR		R1, [SP,#32]	; store the PC into new top of pre-IRQ stack
		LDR		R1, =preempt	; load the address of label preempt
		STR		R1, [SP,#24]	; replace return PC in shifted IRQ stack with pointer to preempt
		STR		R2, [SP,#28]	; store the xPSR into shifted IRQ stack location
		
		; return from IRQ
return	BX		LR				; execute return from ISR
		
preempt	PUSH	{R0-R3,R12,LR}	; push caller-save registers
		
		LDR		R0, =taskTable	; load the taskTable base
		LDR		R1, =currTaskID	; load the current task ID
		LDR		R1, [R1]
		MOV		R2, #96			; set R2 to sizeof(struct task)
		MLA		R1, R1, R2, R0	; multiply-accumulate to index into taskTable to currTask
		
		; currTask.status = TASK_STATUS_PREEMPTED;
		MOV		R2, #0x00000010	; set R2 to TASK_STATUS_PREEMPTED
		STR		R2, [R1,#8]		; store task status
		
		; switchContext(&kframe, &(currTask.frame));
		ADD		R1, #24			; offset into the frame field of the current task entry, loading currTask.frame as *oldframe
		LDR		R0, =kframe		; load kframe as *newframe
		BL		switchContext	; call function to perform context switch
		
		LDR		R0, [SP,#24]	; load the return PC
		ORR		R0, #0x00000001	; odd-align the return PC, since Cortex-M4 is Thumb-only
		STR		R0, [SP,#24]	; store back odd-aligned PC
		POP		{R0-R3,R12,LR}	; load the saved registers
		POP		{PC}			; return to user task
		
		NOP						; binary alignment padding
		
		ENDP
		
		END
