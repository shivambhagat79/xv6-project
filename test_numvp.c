#include "types.h"
#include "user.h"

int main()
{
    int vp = numvp();

    printf(1, "\nNumber of virtual pages (including stack guard page): %d\n\n", vp);

    exit();
}