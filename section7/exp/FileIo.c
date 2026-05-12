#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
int main()
{
    FILE *f2 = fopen("exist.txt", "r");
    char buffer[3];
    if (f2)
    {
        while (fgets(buffer, 3, f2))
        {
            printf("%c%c\n", buffer[0], buffer[1]);
        }
    }
}