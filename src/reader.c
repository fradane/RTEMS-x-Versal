/*
 * reader.c — Bare-metal reader application for RPU1 (Vitis / standalone)
 *
 * This file is a bare-metal (non-RTEMS) Vitis application that runs on RPU
 * core 1 (RPU1) and acts as the receiving end of the inter-processor
 * communication (IPC) demo.  It polls the shared OCM location 0x20000000
 * until the writer (RPU0) sets the flag to 5, then acknowledges by writing 2
 * back to the same address.  A data-synchronisation barrier (DSB) is issued
 * before each read to prevent stale cache hits.  The polling loop prints a
 * status message every 5 cycles so progress can be monitored on the console.
 * This file uses the Vitis platform helpers init_platform/cleanup_platform.
 */

#define SHARED_FLAG ((volatile uint32_t *) 0x20000000)

int main()
{
    init_platform();

    sleep(3);

    int cycle = 0;

    __asm__ volatile ("dsb sy" ::: "memory");
    while (*SHARED_FLAG != 5) {
        cycle++;

        if (cycle % 5 == 0)
            printf("Reader: polling... cycle %d\n", cycle);

        sleep(1);
        __asm__ volatile ("dsb sy" ::: "memory");
    }

    __asm__ volatile ("dsb sy" ::: "memory");
    *SHARED_FLAG = 2;
    __asm__ volatile ("dsb sy" ::: "memory");

    printf("Reader: successful communication\n");

    cleanup_platform();
    return 0;
}
