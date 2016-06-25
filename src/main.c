/* Main program */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "knapsack.h"
#include "matrix.h"

void ga_bangkit(matrix_t *popawal, int ukuranpop, int jumlahb);

void
ga_bangkit(matrix_t *popawal, int ukuranpop, int jumlahb)
{
    /* Langkah 1: membangkitkan matrix ukuranpopxjumlahb dengan nilai random */
    matrixf_t *random = mfinit(ukuranpop, jumlahb);
    for(int i = 0; i < ukuranpop; i++) {
        for(int j = 0; j < jumlahb; j++) {
            float value = (float)rand()/RAND_MAX;
            mfset(random, i, j, value);
        }
    }
    printf("Nilai random:\n");
    for(int i = 0; i < ukuranpop; i++) {
        printf("%d: ", i + 1);
        for(int j = 0; j < jumlahb; j++) {
            printf("%f ", mfget(random, i, j));
        }
        printf("\n");
    }

    /* Langkah 2: meranking tiap individu */
    matrix_t *ranking = minit(ukuranpop, jumlahb);
    for(int i = 0; i < ukuranpop; i++) {
        for(int j = 0; j < jumlahb; j++) {
            /* Kita merangking dengan membandingkan nilai dengan yang lain dalam
             * satu kromosom */
            int rankingn = 0;
            float current = mfget(random, i, j);
            for(int k = 0; k < jumlahb; k++) {
                if(j == k) continue;
                float compare = mfget(random, i, k);
                assert(current != compare);
                if(current > compare) {
                    rankingn += 1;
                }
            }
            mset(ranking, i, j, rankingn + 1);
        }
    }
    printf("Ranking:\n");
    for(int i = 0; i < ukuranpop; i++) {
        printf("%d: ", i+1);
        for(int j = 0; j < jumlahb; j++) {
            int rankingn = mget(ranking, i, j);
            mset(popawal, i, j, rankingn);
            printf("%d ", rankingn);
        }
        printf("\n");
    }

    mffree(random);
    mfree(ranking);
}

int
main(int argc, char **argv)
{
    /* Mendefinisikan data */
    /* data tinggi barang */
    const int datatb[16] = {12, 12, 6, 6, 5, 5, 12, 7, 7, 6, 2, 2, 4, 4, 2, 2};
    /* data lebar barang */
    const int datalb[16] = {2, 7, 8, 3, 3, 5, 3, 3, 5, 2, 3, 4, 3, 4, 9, 11};

    /* Print data */
    printf("==== DATA ====\n");
    printf("Barang\tTinggi\tLebar\n");
    for (int i = 0; i < 16; i++)
    {
        printf("%d\t%d\t%d\n", i+1, datatb[i], datalb[i]);
    }

    printf("\n\n");
    printf("==== Algoritma Genetika ===\n");
    printf("Langkah 1: Membangkitkan populasi awal\n");
    /* Algoritma genetika */
    /* Langkah 1: Membangkitkan populasi */
    int ukuranpop = 10;
    int jumlahb = 16;
    printf("Ukuran populasi: %d\n", ukuranpop);
    printf("Jumlah barang: %d\n", jumlahb);
    matrix_t *popawal = minit(ukuranpop, jumlahb);
    if(popawal == NULL) {
        printf("Err: alokasi popawal\n");
        return EXIT_FAILURE;
    }
    printf("\n");
    ga_bangkit(popawal, ukuranpop, jumlahb);
    printf("\n");
    printf("Populasi awal:\n");
    for(int i = 0; i < ukuranpop; i++) {
        printf("%d: ", i+1);
        for(int j = 0; j < jumlahb; j++) {
            int popi = mget(popawal, i, j);
            printf("%d ", popi);
        }
        printf("\n");
    }

    printf("Langkah 2: Evaluasi\n");
    int hasileval[ukuranpop];
    for(int i = 0; i < ukuranpop; i++) {
        int kromosom[jumlahb];
        for(int j = 0; j < jumlahb; j++) {
            *(kromosom + j) = mget(popawal, i, j);
        }
        int tinggi = knapsack_pack(datalb, datatb, kromosom, jumlahb);
        printf("Tinggi Kromosom %d: %d\n", i+1, tinggi);
        *(hasileval + i) = tinggi;
    }

    printf("\n");
    printf("Hasil Evaluasi:\n");
    for(int i = 0; i < ukuranpop; i++) {
        printf("Kromosom %d: %d\n", i+1, hasileval[i]);
    }

    mfree(popawal);
    return 0;
}

