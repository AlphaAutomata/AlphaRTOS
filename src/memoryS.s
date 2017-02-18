	AREA	FLASH, CODE, READONLY
			
runTask PROC
		EXPORT	runTask
		
		STMIA	R1, {R0-R12}	; save registers 0-12 to *oldframe
		ADD		R1, #52			; increment pointer to SP field in struct
		STR		SP, [R1], #4	; save stack pointer in the SP field, increment to LR field
		STR		LR, [R1]		; save the link register in the LR field
		
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
		ADD		R1, #52			; increment pointer to SP field in struct
		STR		SP, [R1], #4	; save stack pointer in the SP field, increment to LR field
		STR		LR, [R1]		; save the link register in the LR field
		
		ADD		R0, #4			; we cannot load R0 yet since that's the sturct *newframe address
		LDMIA	R0, {R1-R12}	; load registers 1-12 from *newframe
		ADD		R0, #48			; increment pointer to SP field in struct
		LDR		SP, [R0], #4	; load the new stack pointer
		LDR		LR, [R0]		; load the link register
		SUB		R0, #56			; go back to the beginning of the struct to load R0
		LDR		R0, [R0]		; load R0 from struct
		
		BX		LR				; execute return
		
		ENDP
		
		END
