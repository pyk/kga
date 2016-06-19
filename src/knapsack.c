#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "knapsack.h"

/* knapsack_pack: fungsi untuk penyusunan barang */
/* TODO(pyk): fix the algorithm, see GA.xls file */
int
knapsack_pack(const int *widths, const int *heights, const int *urutan,
        const int jumlah_barang)
{
    printf("Penyusunan barang dimulai:\n");
    /* config */
    int MAX_WIDTH = 20;
    printf("Lebar maksimal: %d\n", MAX_WIDTH);
    /* simpan total heightnya */
    int total_height = 0;
    /* inialisasi space, ini untuk meyimpan subspace yg ada. Subspace adalah
     * array dengan len(subspace)=2 dan subspace[0] => width (lebar) dan
     * subspace[1] => height (tinggi) */
    int subspace_n = 0;
    int **space = (int **)calloc(1, sizeof(int *));
    if(space == NULL) {
        errno = ENOMEM;
        return -1;
    };

    for(int i = 0; i < jumlah_barang; i++) {
        /* simpan informasi barang ke-i */
        int index = urutan[i] - 1;
        int b_width = widths[index];
        int b_height = heights[index];

        printf("\tUrutan\t: %d\n", i+1);
        printf("\tNama\t: Barang %d\n", index+1);
        printf("\tLebar\t: %d\n", b_width);
        printf("\tTinggi\t: %d\n", b_height);

        /* Kasus 1: barang pertama. Langsung susun dan buat subspace baru */
        if(i == 0) {
            /* Iniliasisasi subspace pertama */
            subspace_n += 1;
            int *subspace = (int *)calloc(2, sizeof(int));
            if(subspace == NULL) {
                errno = ENOMEM;
                return -1;
            };
            subspace[0] = MAX_WIDTH - b_width;
            subspace[1] = b_height;
            space[0] = subspace;
            total_height += subspace[1];

            printf("\t\tBarang pertama, belum ada subspace.\n");
            printf("\t\tPembuatan subspace baru:\n");
            printf("\t\tLebar subspace #%d\t: %d\n", subspace_n, subspace[0]);
            printf("\t\tTinggi subspace #%d\t: %d\n", subspace_n, subspace[1]);
            printf("\t\tUpdate total tinggi\t: %d\n", total_height);

            /* langsung cek barang lain */
            continue;
        }

        /* Kasus 2: barang selain barang pertama dan muat di subspace yang sudah
         * ada. Disini tanpa ada penambahan total_height. */
        int is_sscreated = 0;
        for(int j = 0; j < subspace_n; j++) {
            int *subspace = space[j];

            printf("\t\tCek apakah barang muat di subspace #%d?\n", j+1);

            /* Kasus 2.1: barang muat tanpa di balik,
             * cek apakah muat di subspace ini */
            if(b_width <= subspace[0] && b_height <= subspace[1]) {
                /* jika muat, maka buat subspace baru dan update tinggi dari
                 * subspace yang dimuati oleh barang ini */
                subspace_n += 1;
                int *ssnew = (int *)calloc(2, sizeof(int));
                if(ssnew == NULL) {
                    errno = ENOMEM;
                    /* clear previous resource */
                    subspace_n -= 1;
                    for(int i = 0; i < subspace_n; i++) {
                        free(space[i]);
                    }
                    free(space);
                    return -1;
                };
                ssnew[0] = subspace[0] - b_width;
                ssnew[1] = b_height;
                /* realloc untuk subspace baru */
                space = (int **)realloc(space, subspace_n * sizeof(int *));
                space[subspace_n - 1] = ssnew;
                /* update height current subspace */
                subspace[1] -= ssnew[1];
                if(subspace[1] == 0) {
                    subspace[0] = 0;
                }

                printf("\t\tBarang muat tanpa di balik.\n");
                printf("\t\tPembuatan subspace baru:\n");
                printf("\t\tLebar subspace #%d\t: %d\n", subspace_n, ssnew[0]);
                printf("\t\tTinggi subspace #%d\t: %d\n", subspace_n, ssnew[1]);
                printf("\t\tUpdate subspace #%d:\n", j+1);
                printf("\t\tLebar subspace #%d\t: %d\n", j+1, subspace[0]);
                printf("\t\tTinggi subspace #%d\t: %d\n", j+1, subspace[1]);

                /* break untuk cek barang selanjutnya */
                is_sscreated = 1;
                break;
            }

            /* Kasus 2.2: barang muat dengan di balik. Width jadi height dan
             * sebaliknya */
            if(b_width <= subspace[1] && b_height <= subspace[0]) {
                /* Jika muat, maka buat subspace baru dan update tinggi dari
                 * subspace yang dimuati barang ini */
                subspace_n += 1;
                int *ssnew = (int *)calloc(2, sizeof(int));
                if(ssnew == NULL) {
                    errno = ENOMEM;
                    /* clear previous resource */
                    subspace_n -= 1;
                    for(int i = 0; i < subspace_n; i++) {
                        free(space[i]);
                    }
                    free(space);
                    return -1;
                };
                ssnew[0] = subspace[0] - b_height;
                ssnew[1] = b_width;
                /* realloc untuk subspace baru */
                space = (int **)realloc(space, subspace_n * sizeof(int *));
                space[subspace_n - 1] = ssnew;
                /* update height current subspace */
                subspace[1] -= ssnew[1];
                if(subspace[1] == 0) {
                    subspace[0] = 0;
                }

                printf("\t\tBarang muat dengan di balik.\n");
                printf("\t\tLebar\t: %d\n", b_height);
                printf("\t\tTinggi\t: %d\n", b_width);
                printf("\t\tPembuatan subspace baru:\n");
                printf("\t\tLebar subspace #%d\t: %d\n", subspace_n, ssnew[0]);
                printf("\t\tTinggi subspace #%d\t: %d\n", subspace_n, ssnew[1]);
                printf("\t\tUpdate subspace #%d:\n", j+1);
                printf("\t\tLebar subspace #%d\t: %d\n", j+1, subspace[0]);
                printf("\t\tTinggi subspace #%d\t: %d\n", j+1, subspace[1]);

                /* break untuk cek barang selanjutnya */
                is_sscreated = 1;
                break;
            }
        }

        /* Kasus 3: barang tidak muat di subspace yang sudah ada. Maka buat
         * subspace baru dan update total_height nya */
        if(!is_sscreated) {
            /* buat subspace baru dan update total_height nya */
            subspace_n += 1;
            int *subspace = (int *)calloc(2, sizeof(int));
            if(subspace == NULL) {
                errno = ENOMEM;
                /* clear previous resource */
                subspace_n -= 1;
                for(int i = 0; i < subspace_n; i++) {
                     free(space[i]);
                }
                free(space);
                return -1;
            };
            subspace[0] = MAX_WIDTH - b_width;
            subspace[1] = b_height;
            /* realloc untuk subspace baru */
            space = (int **)realloc(space, subspace_n * sizeof(int *));
            space[subspace_n - 1] = subspace;
            /* update total_height nya */
            total_height += subspace[1];

            printf("\t\tBarang tidak muat di semua subspace.\n");
            printf("\t\tPembuatan subspace baru:\n");
            printf("\t\tLebar subspace #%d\t: %d\n", subspace_n, subspace[0]);
            printf("\t\tTinggi subspace #%d\t: %d\n", subspace_n, subspace[1]);
            printf("\t\tUpdate total tinggi\t: %d\n", total_height);
        }
    }


    for(int i = 0; i < subspace_n; i++) {
        free(space[i]);
    }
    free(space);

    return total_height;
}
