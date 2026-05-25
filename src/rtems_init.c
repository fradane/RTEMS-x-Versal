/*
 * rtems_init.c — RTEMS Init task for the writer (RPU0) side
 *
 * This file implements the RTEMS Init task that runs on RPU core 0 (RPU0)
 * as part of the inter-processor communication (IPC) demo between the two
 * Cortex-R5F cores of the Versal RPU subsystem.
 * After a 3-second startup delay, RPU0 writes a flag value (WRITER_READY_VALUE
 * = 0x1) to a shared OCM (On-Chip Memory) location at 0x20000000 to signal
 * RPU1 that it is ready.  Data-synchronisation barriers (DSB) are issued
 * before and after the write to guarantee memory ordering across cores.
 * The result is reported over the standard RTEMS console via printf().
 */

#include <rtems.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define SHARED_MEM_ADDR     0x20000000U
#define SHARED_FLAG         ((volatile uint32_t *)SHARED_MEM_ADDR)

#define WRITER_READY_VALUE  0x1U   /* written by RPU0 to signal RPU1 */

rtems_task Init(rtems_task_argument ignored)
{
    *SHARED_FLAG = 0U;
    __asm__ volatile ("dsb sy" ::: "memory");

    sleep(3);

    *SHARED_FLAG = WRITER_READY_VALUE;
    __asm__ volatile ("dsb sy" ::: "memory");

    if (*SHARED_FLAG == WRITER_READY_VALUE) {
        printf("[RPU_0] Write succeeded\n");
    } else {
        printf("[RPU_0] Write NOT succeeded\n");
    }

    rtems_task_exit();
}
