#include <stdio.h>

#include "stdin/setecho.h"

int main(void)
{
    char secret[256];

    printf("Hello, What is your secret?\n");

    setecho(0); /* disable echo */
    fgets(secret, 256, stdin); /* secret will not be visible */
    setecho(1); /* enable echo */

    return 0;
}
