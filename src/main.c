/* Main program */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "knapsack.h"
#include "matrix.h"

matrix_t *ga_bangkit(int ukuranpop, int jumlahb);
matrix_t *ga_eval(matrix_t *popawal, const int *datatb, const int *datalb,
        int ukuranpop, int jumlahb);

matrix_t *
ga_bangkit(int ukuranpop, int jumlahb)
{
    /* Populasi awal */
    matrix_t *popawal = minit(ukuranpop, jumlahb);
    if(popawal == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    /* Langkah 1: membangkitkan matrix ukuranpopxjumlahb dengan nilai random */
    matrixf_t *random = mfinit(ukuranpop, jumlahb);
    if(random == NULL) {
        errno = ENOMEM;
        return NULL;
    }
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
    if(ranking == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    for(int i = 0; i < ukuranpop; i++) {
        for(int j = 0; j < jumlahb; j++) {
            /* Kita merangking dengan membandingkan nilai dengan yang lain dalam
             * satu kromosom */
            int rankingn = 0;
            float current = mfget(random, i, j);
            for(int k = 0; k < jumlahb; k++) {
                if(j == k) continue;
                float compare = mfget(random, i, k);
                if(current == compare) {
                    errno = ECANCELED;
                    return NULL;
                }
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

    return popawal;
}

matrix_t *
ga_eval(matrix_t *popawal, const int *datatb, const int *datalb, int ukuranpop,
        int jumlahb)
{
    /* Alokasi bobot ke memory */
    matrix_t *bobot = minit(ukuranpop, 1);
    if(bobot == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    for(int i = 0; i < ukuranpop; i++) {
        int kromosom[jumlahb];
        for(int j = 0; j < jumlahb; j++) {
            *(kromosom + j) = mget(popawal, i, j);
        }
        int tinggi = knapsack_pack(datalb, datatb, kromosom, jumlahb);
        if(tinggi < 0) {
            errno = ECANCELED;
            return NULL;
        }
        mset(bobot, i, 0, tinggi);
    }

    return bobot;
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
    printf("\n");
    matrix_t *popawal = ga_bangkit(ukuranpop, jumlahb);
    if(popawal == NULL) {
        printf("Err: Pembangkitan populasi awal gagal\n");
        if(errno == ECANCELED) printf("Err: Ulangi lagi\n");
        return EXIT_FAILURE;
    }
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
    matrix_t *bobot = ga_eval(popawal, datatb, datalb, ukuranpop, jumlahb);
    if(bobot == NULL) {
        printf("Err: Evaluasi populasi awal gagal\n");
        return EXIT_FAILURE;
    }

    printf("Hasil Evaluasi:\n");
    printf("Kromosom\tBobot\n");
    for(int i = 0; i < ukuranpop; i++) {
        int tinggi = mget(bobot, i, 0);
        printf("%d\t\t%d\n", i+1, tinggi);
    }

    printf("\n");
    printf("Langkah 3: Seleksi\n");

    mfree(bobot);
    mfree(popawal);
    return 0;
}

