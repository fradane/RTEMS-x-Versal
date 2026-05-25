/*
 * blinking_led.c — RTEMS LED blink application for Versal PL
 *
 * This is the top-level RTEMS Init task for the LED blinking demo.
 * It drives a user-facing LED that is mapped to the Programmable Logic (PL)
 * fabric of the AMD/Xilinx Versal device via a custom AXI GPIO IP core.
 * The LED register is memory-mapped at 0x80000000 (AXI GPIO base address
 * set in the Vivado hardware design).  A data-synchronisation barrier
 * (DSB) is issued after every register write to prevent out-of-order
 * execution on the ARM Cortex-R5F RPU core.
 * The task waits 5 seconds after boot, then toggles the LED on/off
 * ten times with a 2-second period before calling rtems_task_exit().
 */

#include <rtems.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define USR_LED_ADRR 0x80000000
#define USR_LED (* (volatile uint32_t *) USR_LED_ADRR)

#define USR_LED_ON 0x0
#define USR_LED_OFF 0x1

#define MEMORY_BARRIER __asm__ volatile ("dsb sy" ::: "memory")

void turn_led_on() {
  USR_LED = USR_LED_ON;
}

void turn_led_off() {
  USR_LED = USR_LED_OFF;
}

rtems_task Init(rtems_task_argument ignored) {

  sleep(5);

  printf("Starting...\n");

  for (volatile uint32_t i = 0; i < 10; i++) {
    if (i % 2 == 0)
      turn_led_on();
    else
      turn_led_off();

    MEMORY_BARRIER;

    sleep(2);
  }

  printf("...exiting.\n");

  rtems_task_exit();
}
