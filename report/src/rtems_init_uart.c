#include <rtems.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#define SHARED_MEM_ADDR     0x20000000U
#define SHARED_FLAG         ((volatile uint32_t *)SHARED_MEM_ADDR)

#define WRITER_READY_VALUE  0x1U   /* written by RPU0 to signal RPU1 */

/* UART1 base: 0xFF010000  (UART0 base would be 0xFF000000) */
#define UART_DR  (*(volatile uint32_t *)0xFF010000)
#define UART_FR  (*(volatile uint32_t *)0xFF010018)

void custom_printf(char *str);

rtems_task Init(rtems_task_argument ignored)
{
    *SHARED_FLAG = 0U;
    __asm__ volatile ("dsb sy" ::: "memory");

    sleep(3);

    *SHARED_FLAG = WRITER_READY_VALUE;
    __asm__ volatile ("dsb sy" ::: "memory");

    if (*SHARED_FLAG == WRITER_READY_VALUE) {
        custom_printf("[RPU_0] Write succeeded\n");
    } else {
        custom_printf("[RPU_0] Write NOT succeeded\n");
    }

    rtems_task_exit();
}

void custom_printf(char *str) {
    for (uint32_t i = 0; i < strlen(str); i++) {
        while (UART_FR & (1 << 5)) { } /* wait while TX FIFO full */
        UART_DR = str[i];
        for (volatile int d = 0; d < 100000; d++) { }
    }
}
