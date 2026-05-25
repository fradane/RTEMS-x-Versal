/*
 * writer.c — Bare-metal writer application for RPU0 (Vitis / standalone)
 *
 * This file is a bare-metal (non-RTEMS) Vitis application that runs on RPU
 * core 0 (RPU0) and acts as the sending end of the inter-processor
 * communication (IPC) demo.  After a 3-second startup delay it writes the
 * value 5 to a shared OCM location at 0x20000000, then immediately reads it
 * back to verify the write completed without error.  A data-synchronisation
 * barrier (DSB) is issued after the write to enforce memory ordering.
 * This file uses the Vitis platform helpers init_platform/cleanup_platform
 * and was an early proof-of-concept before the RTEMS-based variant was used.
 */

#define SHARED_FLAG ((volatile uint32_t *) 0x20000000)

int main()
{
    init_platform();

    sleep(3);

    *SHARED_FLAG = 5;
    __asm__ volatile ("dsb sy" ::: "memory");

    if (*SHARED_FLAG == 5) {
        print("Writer: write completed\n");
    } else {
        print("Writer: write not completed\n");
    }

    cleanup_platform();
    return 0;
}
