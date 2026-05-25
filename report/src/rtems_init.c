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
