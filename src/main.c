/* Main program */
#include <stdio.h>
#include <stdlib.h>

#include "knapsack.h"

int
main(int argc, char **argv)
{
    /* Mendefinisikan data */
    const int widths[16] = {2, 7, 8, 3, 3, 5, 3, 3, 5, 2, 3, 4, 3, 4, 9, 11};
    const int heights[16] = {12, 12, 6, 6, 5, 5, 12, 7, 7, 6, 2, 2, 4, 4, 2, 2};

    /* Print data */
    printf("Barang\twidths\theights\n");
    for (int i = 0; i < 16; i++)
    {
        printf("%d\t%d\t%d\n", i+1, widths[i], heights[i]);
    }

    /* GA untuk mencari urutan penumpukan barang
     * pack: input urutan barang, output tinggi suatu packing
     * Misalkan urutannya: */
    const int urutan[16] = {6, 8, 3, 4, 2, 1, 16, 15, 13, 12, 14, 10, 11, 5, 9, 7};
    int tinggi = knapsack_pack(widths, heights, urutan, 16);
    printf("Tinggi susunan barang: %d\n", tinggi);
    const int urutan2[16] = {3, 4, 2, 1, 6, 16, 15, 13, 8, 12, 14, 10, 11, 5, 9, 7};
    tinggi = knapsack_pack(widths, heights, urutan2, 16);
    printf("Tinggi susunan barang: %d\n", tinggi);
    return 0;
}

