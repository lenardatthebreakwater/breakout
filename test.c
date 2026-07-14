#include <stdio.h>
#include <string.h>

int main(void) {
    char name [4] = { 0 };
    memcpy(name, "Tom", 3);
    printf("%d", name[3]);

    return 0;
}