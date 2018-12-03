#include "hal_platform.h"

#include "hal_systick.h"

#if (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx) && 0

#include "xscutimer.h"
#include "xscugic.h"

XScuTimer scuTimer;
XScuGic   intCtrl;

#endif

void hal_systick_init(hal_timer_isr_t sysTickHandler, int sysTickInterval) {
#if (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx) && 0
	XScuTimer_Config* scu_config;
	XScuGic_Config*   intCtrl_config;

	// Set timer to count milliseconds
	scu_config = XScuTimer_LookupConfig(XPAR_XSCUTIMER_0_DEVICE_ID);
	XScuTimer_CfgInitialize(&scuTimer, scu_config, scu_config->BaseAddr);
	XScuTimer_LoadTimer(&scuTimer, sysTickInterval);
	XScuTimer_EnableAutoReload(&scuTimer);
	XScuTimer_Start(&scuTimer);

	// Initialize generic interrupt controller
	intCtrl_config = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
	XScuGic_CfgInitialize(&intCtrl, intCtrl_config, intCtrl_config->CpuBaseAddress);

	// Register systick handler with the interrupt controller
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(
		XIL_EXCEPTION_ID_IRQ_INT,
		(Xil_ExceptionHandler)XScuGic_InterruptHandler,
		&intCtrl
	);

	// Enable interrupt controller
	XScuGic_Connect(&intCtrl, XPAR_SCUTIMER_INTR, sysTickHandler, &scuTimer);
	XScuGic_Enable(&intCtrl, XPAR_SCUTIMER_INTR);
	
	// Set timer to interrupt on timeout
	XScuTimer_EnableInterrupt(&scuTimer);

	// Enable processor interrupts
	Xil_ExceptionEnable();
#endif
}
