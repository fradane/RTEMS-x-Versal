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
