/* Main program */
#include <stdio.h>

int
main(int argc, char **argv)
{
    /* Define data */
    int width[16] = {2, 7, 8, 3, 3, 5, 3, 3, 5, 2, 3, 4, 3, 4, 9, 11};
    int height[16] = {12, 12, 6, 6, 5, 5, 12, 7, 7, 6, 2, 2, 4, 4, 2, 2};

    /* Print data */
    printf("Barang\tWidth\tHeight\n");
    for (int i = 0; i < 16; i++)
    {
        printf("%d\t%d\t%d\n", i+1, width[i], height[i]);
    }
    return 0;
}
