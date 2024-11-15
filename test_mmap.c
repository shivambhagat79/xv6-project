#include "types.h"
#include "stat.h"
#include "user.h"

void check_memory_status()
{
    int vp = numvp();
    int pp = numpp();

    printf(1, "\nNumber of virtual pages: %d\n", vp);
    printf(1, "Number of physical pages: %d\n\n", pp);
}

int main()
{
    // initial memory status
    printf(1, "\nInitial memory status:\n");
    check_memory_status();

    int size = 2 * 4096; // 2 page

    // allocate memory = size bytes
    char *address = (char *)mmap(size);

    if (address == 0)
    {
        printf(1, "mmap failed\n");
        exit();
    }

    // memory status after mmap
    printf(1, "Memory status after mmap:\n");
    check_memory_status();

    // accessing the mapped memory to trigger demand paging
    address[0] = 'a';
    address[4096] = 'b';

    // memory status after accessing the mapped memory
    printf(1, "Memory status after accessing the mapped memory:\n");
    check_memory_status();

    exit();
}