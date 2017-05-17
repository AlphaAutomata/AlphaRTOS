; offset macros to access registers in regframe_t
; must update if regframe_t definition in memory.h is changed
__R0				EQU 0
__R1				EQU 4
__R2				EQU 8
__R3				EQU 12
__R4				EQU 16
__R5				EQU 20
__R6				EQU 24
__R7				EQU 28
__R8				EQU 32
__R9				EQU 36
__R10				EQU 40
__R11				EQU 44
__R12				EQU 48
__SP				EQU 52
__LR				EQU 56
__PC				EQU 60
__xPSR				EQU 64

; offset macros to access fields in struct task
; must change if struct task definition changes in scheduler.h
taskTicksInterval	EQU	0
taskTickRemaining	EQU	4
taskStatus			EQU	8
taskInitArg			EQU	12
taskTaskEntry		EQU	16
taskTimerCallback	EQU	20
taskIntCallback		EQU	24
taskFrame			EQU	28

; size macros for regframe_t and sizeof(struct task)
; must change if struct task or regframe_t definitions change
; struct task	defined in scheduler.h
; regframe_t	defined in memory.h
regframe_t_offset	EQU	24
regframe_t_size		EQU	68
structTaskSize		EQU	regframe_t_offset+regframe_t_size

; macros to access data from the IRQ stack
; these are defined by the Cortex-M4 ISA
__IRQR0				EQU	0
__IRQR1				EQU	4
__IRQR2				EQU	8
__IRQR3				EQU	12
__IRQR12			EQU	16
__IRQLR				EQU	20
__IRQPC				EQU	24
__IRQxPSR			EQU	28
__preIRQtop			EQU	32

; macros for Timer0 peripheral interrupt clear
TIMER0_BASE			EQU 0x40030000
TIMER_TIMA_TIMEOUT	EQU 0x00000001
TIMER_O_ICR			EQU 0x00000024
		
		AREA	VARS, DATA, READWRITE, ALIGN=3
isrContAddr DCD	0x00000000
		
		AREA	FLASH, CODE, READONLY
		
runTask PROC
		EXPORT	runTask
		
		STMIA	R1, {R0-R12}			; save registers 0-12 to *oldframe
		MRS		R3, APSR				; get the APSR
		STR		SP, [R1,#__SP]			; save stack pointer in the SP field, increment to LR field
		STR		LR, [R1,#__LR]			; save the link register in the LR field
		STR		R3, [R1,#__xPSR]		; save the APSR into the xPSR field
		
		MOV		R4, R2					; save the branch address from the third function argument
		ADD		R0, #__R1				; we cannot load R0 yet since that's the *newframe address
		LDMIA	R0, {R1-R3}				; load registers 1-3 from *newframe to pass parameters
		LDR		SP, [R0,#(__SP-__R1)]	; load the new stack pointer
		LDR		LR, [R0,#(__LR-__R1)]	; load the link register so it returns where we want it to
		SUB		R0, #__R1				; go back to the beginning of the struct to load R0
		LDR		R0, [R0]				; load R0 from struct
		
		BX		R4						; call into the new context
		
		ENDP
		
switchContext PROC
		EXPORT	switchContext
		STMIA	R1, {R0-R12}			; save registers 0-12 to *oldframe
		MRS		R2, APSR				; get the current APSR
		STR		SP, [R1,#__SP]			; save stack pointer in the SP field, increment to LR field
		STR		LR, [R1,#__LR]			; save the link register in the LR field
		STR		R2, [R1,#__xPSR]		; save the APSR register in the xPSR field
		
		ADD		R0, #__R2				; cannot load R0 yet since it holds *newframe, hold off on
										; loading R1 to use it for the xPSR
		LDMIA	R0, {R2-R12}			; load registers 2-12 from *newframe
		LDR		SP, [R0,#(__SP-__R2)]	; load the new stack pointer
		LDR		LR, [R0,#(__LR-__R2)]	; load the link register
		LDR		R1, [R0,#(__xPSR-__R2)]	; load the xPSR into R1
		MSR		APSR, R1				; store the APSR
		SUB		R0, #__R2				; go back to the beginning of the struct to load R0
		LDMIA	R0, {R0-R1}				; load R0 from struct
		
		BX		LR						; execute return
		
		ENDP
		
		; The SysTick interrupt serves as the operating system tick timer for scheduling, time
		; keeping, and task preemption. When the global currTaskID variable is set correctly, this
		; interrupt service routine always guarentees that the kernel will run at least once every
		; millisecond. This ISR does so by loading an IRQ return address onto the IRQ stack to drop
		; out of the IRQ into the preempt subroutine. The preempt subroutine then performs the
		; switchContext() function call as if it were the preempted task. 
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
IRQ_prep_preempt
		LDR		R0, =currTaskID
		LDR		R1, [R0]
		CMP		R1, #0
		BEQ		return
		
		; preempt running task by dropping out of the ISR into a context switch routine
		LDMIA	SP, {R0-R3,R12}			; load the general purpose registers from IRQ stack
		SUB		SP, #4					; allocate a 32-bit word on stack, which we won't restore.
										; This effectively moves the IRQ stack by a word. 
		STMIA	SP, {R0-R3,R12}			; store the general purpose registers to shifted IRQ stack
		
		LDR		R0, [SP,#(__IRQLR+4)]	; load the LR from unshifted IRQ stack location
		LDR		R1, [SP,#(__IRQPC+4)]	; load the PC from unshifted IRQ stack location
		LDR		R2, [SP,#(__IRQxPSR+4)]	; load the xPSR from unshifted IRQ stack location
		
		STR		R0, [SP,#(__IRQLR)]		; store the LR into shifted IRQ stack location
		STR		R1, [SP,#(__preIRQtop)]	; store the PC into new top of pre-IRQ stack
		LDR		R1, =preempt			; load the address of label preempt
		STR		R1, [SP,#(__IRQPC)]		; point return PC in shifted IRQ stack to preempt label
		STR		R2, [SP,#(__IRQxPSR)]	; store the xPSR into shifted IRQ stack location
		
		; return from IRQ
return	BX		LR						; execute return from ISR
		
		; the preempt subroutine runs as if it was inserted into whatever code the Systick
		; interrupted. It performs an EABI standard function call to switchContext() in order to
		; force the user task to switch back to the kernel context. After the kernel context
		; returns, this preemption code restores caller-save registers so the user task can resume
		; execution as if nothing happened. 
preempt	PUSH	{R0-R3,R12,LR}			; push caller-save registers
		
		LDR		R0, =taskTable			; load the taskTable base
		LDR		R1, =currTaskID			; get address of currTaskID
		LDR		R1, [R1]				; load the currTaskID value
		MOV		R2, #(structTaskSize)	; set R2 to sizeof(struct task)
		MLA		R1, R1, R2, R0			; multiply-accumulate to index into taskTable to currTask
		
		; currTask.status = TASK_STATUS_PREEMPTED;
		MOV		R2, #0x00000010			; set R2 to TASK_STATUS_PREEMPTED
		STR		R2, [R1,#taskStatus]	; store task status
		
		; switchContext(&kframe, &(currTask.frame));
		ADD		R1, #taskFrame			; offset into the frame field of the current task entry,
										; loading currTask.frame as *oldframe
		LDR		R0, =kframe				; load kframe as *newframe
		BL		switchContext			; call function to perform context switch
		
		LDR		R0, [SP,#__IRQPC]		; load the return PC
		ORR		R0, #0x00000001			; odd-align the return PC, since Cortex-M4 is Thumb-only
		STR		R0, [SP,#__IRQPC]		; store back odd-aligned PC
		POP		{R0-R3,R12,LR}			; load the saved registers
		POP		{PC}					; return to user task
		
		ENDP
		
		; this ISR functions identically to the SysTick ISR, except it shouldn't update timekeeping
		; globals. Therefore we simply jump into the SysTick ISR right after global variable update
TIMER0A_Handler PROC
		EXPORT TIMER0A_Handler
		
		; clear timer timeout interrupt
		MOV32	R0, #(TIMER0_BASE+TIMER_O_ICR)	; set mem-mapped address of timer interrupt clear
		MOV		R1, #TIMER_TIMA_TIMEOUT			; set register write value to clear timeout int
		STR		R1, [R0]						; do register write
		
		B		IRQ_prep_preempt				; branch to ISR-context preemption code
		
		NOP
		
		ENDP
		
		END
