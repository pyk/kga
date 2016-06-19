/* Test for knapsack algorithm */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "knapsack.h"

int
main(int argc, char **argv)
{
    /* Mendefinisikan data */
    const int widths[16] = {2, 7, 8, 3, 3, 5, 3, 3, 5, 2, 3, 4, 3, 4, 9, 11};
    const int heights[16] = {12, 12, 6, 6, 5, 5, 12, 7, 7, 6, 2, 2, 4, 4, 2, 2};

    const int urutan[16] = {6, 8, 3, 4, 2, 1, 16, 15, 13, 12, 14, 10, 11, 5, 9, 7};
    int tinggi = knapsack_pack(widths, heights, urutan, 16);
    if(tinggi == -1) {
        printf("Err: %s\n", strerror(errno));
        return 1;
    }
    assert(tinggi == 35);
    return 0;

}
