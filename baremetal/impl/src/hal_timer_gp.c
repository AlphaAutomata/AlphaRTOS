#include <irq_ctrl.h>

#include "hal_platform.h"
#include "hal_timer_gp.h"

#if ((HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx) || (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxxS))

#include "xparameters.h"
#include "xscutimer.h"

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 0)
static XScuTimer xscu_timer_00;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 0)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 1)
static XScuTimer xscu_timer_01;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 1)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 2)
static XScuTimer xscu_timer_02;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 2)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 3)
static XScuTimer xscu_timer_03;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 3)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 4)
static XScuTimer xscu_timer_04;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 4)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 5)
static XScuTimer xscu_timer_05;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 5)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 6)
static XScuTimer xscu_timer_06;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 6)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 7)
static XScuTimer xscu_timer_07;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 7)

#endif
// #if ((HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx ) || (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxxS))

void hal_timerGp_init(hal_timer_inst_t inst, hal_timer_isr_t handler, hal_timerGp_info* info) {
#if ((HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx) || (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxxS))
    u16 dev_id;
    XScuTimer* x_timer_inst;
    switch (inst) {
#if (XPAR_XSCUTIMER_NUM_INSTANCES > 0)
        case hal_timer_inst_00:
            dev_id = XPAR_XSCUTIMER_0_DEVICE_ID;
            x_timer_inst = &xscu_timer_00;
            break;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 0)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 1)
        case hal_timer_inst_01:
            dev_id = XPAR_XSCUTIMER_1_DEVICE_ID;
            x_timer_inst = &xscu_timer_01;
            break;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 1)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 2)
        case hal_timer_inst_02:
            dev_id = XPAR_XSCUTIMER_2_DEVICE_ID;
            x_timer_inst = &xscu_timer_02;
            break;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 2)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 3)
        case hal_timer_inst_03:
            dev_id = XPAR_XSCUTIMER_3_DEVICE_ID;
            x_timer_inst = &xscu_timer_03;
            break;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 3)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 4)
        case hal_timer_inst_04:
            dev_id = XPAR_XSCUTIMER_4_DEVICE_ID;
            x_timer_inst = &xscu_timer_04;
            break;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 4)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 5)
        case hal_timer_inst_05:
            dev_id = XPAR_XSCUTIMER_5_DEVICE_ID;
            x_timer_inst = &xscu_timer_05;
            break;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 5)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 6)
        case hal_timer_inst_06:
            dev_id = XPAR_XSCUTIMER_6_DEVICE_ID;
            x_timer_inst = &xscu_timer_06;
            break;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 6)

#if (XPAR_XSCUTIMER_NUM_INSTANCES > 7)
        case hal_timer_inst_07:
            dev_id = XPAR_XSCUTIMER_7_DEVICE_ID;
            x_timer_inst = &xscu_timer_07;
            break;
#endif // #if (XPAR_XSCUTIMER_NUM_INSTANCES > 7)

        default:
            return;
    }

    XScuTimer_Config* xscu_cfg = XScuTimer_LookupConfig(dev_id);

    XScuTimer_CfgInitialize(x_timer_inst, xscu_cfg, xscu_cfg->BaseAddr);
    XScuTimer_LoadTimer(x_timer_inst, info->loadValue);
    switch (info->rpt) {
        case hal_timer_rpt_ONESHOT:
            XScuTimer_DisableAutoReload(x_timer_inst);
            break;

        case hal_timer_rpt_REPEATED:
            XScuTimer_EnableAutoReload(x_timer_inst);
            break;

        default:
            return;
    }

    IRQ_Enable(XPAR_SCUTIMER_INTR);
    IRQ_SetHandler(XPAR_SCUTIMER_INTR, handler);

    if (info->start_immediately) {
        XScuTimer_Start(x_timer_inst);
    }
#endif
// #if ((HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxx) || (HAL_PLATFORM == HAL_PLATFORM_XSCU_Z7xxxS))
}

void hal_timerGp_cfg(hal_timer_inst_t inst, hal_timer_isr_t handler, hal_timerGp_info* info) {
    return;
}

void hal_timerGp_cfg_handler(hal_timer_inst_t inst, hal_timer_isr_t handler) {
    return;
}

void hal_timerGp_cfg_info(hal_timer_inst_t inst, hal_timerGp_info* info) {
    return;
}

void hal_timerGp_arm(hal_timer_inst_t inst) {
    return;
}

void hal_timerGp_disarm(hal_timer_inst_t inst) {
    return;
}
