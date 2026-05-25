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
