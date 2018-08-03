#include "systick_port.h"

#if (SYSTICK_PLATFORM == XSCU)

#include "xscutimer.h"
#include "xscugic.h"

XScuTimer scuTimer;
XScuGic   intCtrl;

#endif

void systick_init(void* timerBase, void* intCtrlBase, void* sysTickHandler, int sysTickInterval) {
#if (SYSTICK_PLATFORM == XSCU)
	XScuTimer_Config* scu_config;
	XScuGic_Config*   intCtrl_config;

	// Set timer to count milliseconds
	scu_config = XScuTimer_LookupConfig(XPAR_XSCUTIMER_0_DEVICE_ID);
	XScuTimer_CfgInitialize(&scuTimer, scu_config, scu_config->BaseAddr);
	sysTicks = 0;
	XScuTimer_LoadTimer(&scuTimer, sysTickInterval);
	XScuTimer_EnableAutoReload(&scuTimer);
	XScuTimer_Start(&scuTimer);

	// Initialize generic interrupt controller
	intCtrl_config = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
	XScuGic_CfgInitialize(&intCtrl, intCtrl_config, intCtrl_config->CpuBaseAddress);
	//XScuGic_Enable(&intCtrl, XPAR_SCUGIC_0_DEVICE_ID);

	// Register systick handler with the interrupt controller
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(
		XIL_EXCEPTION_ID_IRQ_INT,
		(Xil_ExceptionHandler)XScuGic_InterruptHandler,
		&intCtrl
	);

	// Enable timer interrupts
	XScuGic_Connect(&intCtrl, XPAR_SCUTIMER_INTR, sysTickHandler, &scuTimer);
	XScuGic_Enable(&intCtrl, XPAR_SCUTIMER_INTR);
	XScuTimer_EnableInterrupt(&scuTimer);

	// Enable processor interrupts
	Xil_ExceptionEnable();
#endif
}
