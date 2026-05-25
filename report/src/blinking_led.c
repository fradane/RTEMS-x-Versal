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