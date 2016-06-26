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
matrix_t *ga_fitness(matrix_t *bobot, int ukuranpop);
matrix_t *ga_indukco(const matrixf_t *fitnessk, const float pc, const int ukuranpop);
matrix_t *ga_indukmu(const matrixf_t *fitnessk, const float pm, const int ukuranpop);

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

matrixf_t *
ga_fitnessk(matrix_t *bobot, int ukuranpop) {
    /* Langkah 1: Menghitung total bobot keseluruhan */
    int totalbobot = 0;
    for(int i = 0; i < ukuranpop; i++) {
        totalbobot += mget(bobot, i, 0);
    }

    /* Langkah 2: Menghitung nilai fitness tiap kromosom */
    int totalfitness = 0;
    matrix_t *fitness = minit(ukuranpop, 1);
    if(fitness == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    for(int i = 0; i < ukuranpop; i++) {
        int nilai = totalbobot - mget(bobot, i, 0);
        mset(fitness, i, 0, nilai);
        totalfitness += nilai;
    }

    /* Langkah 3: menghitung nilai fitness relative */
    matrixf_t *fitnessr = mfinit(ukuranpop, 1);
    if(fitnessr == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    /* Langkah 4: menghitung nilai fitness kumulatif */
    float totalfitnessk = 0.0;
    matrixf_t *fitnessk = mfinit(ukuranpop, 1);
    if(fitnessk == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    for(int i = 0; i < ukuranpop; i++) {
        float nilai = (float)mget(fitness, i, 0)/totalfitness;
        totalfitnessk += nilai;
        mfset(fitnessr, i, 0, nilai);
        mfset(fitnessk, i, 0, totalfitnessk);
    }

    printf("Kromosom\tFitness\t\tF. Relative\tF. Kumulatif\n");
    for(int i = 0; i < ukuranpop; i++) {
        int f = mget(fitness, i, 0);
        float fr = mfget(fitnessr, i, 0);
        float fk = mfget(fitnessk, i, 0);
        printf("%d\t\t%d\t\t%f\t%f\n", i+1, f, fr, fk);
    }

    mfree(fitness);
    mffree(fitnessr);

    return fitnessk;
}

/* Mencari calon induk dari nilai fitness kumulatif */
void
_ga_caloninduk(matrixf_t *bilacak, matrix_t *calinduk, const matrixf_t *fitnessk,
        const int ukuranpop)
{
    for(int i = 0; i < ukuranpop; i++) {
        float nilai = (float)rand()/RAND_MAX;
        float fk1 = mfget(fitnessk, 0, 0);
        mfset(bilacak, i, 0, nilai);
        if(nilai <= fk1) {
            mset(calinduk, i, 0, 1);
            continue;
        }

        for(int k = 0; k < ukuranpop-1; k++) {
            float fk = mfget(fitnessk, k, 0);
            float fks = mfget(fitnessk, k+1, 0);
            if(fk < nilai && nilai <= fks) {
                mset(calinduk, i, 0, k+2);
                break;
            }
        }
    }
}

/* Mencari induk dari calon induk dan nilai p */
void
_ga_induk(matrixf_t *bilacak, matrix_t *induk, const matrix_t *calinduk,
        const float p, const int ukuranpop)
{
    for(int i = 0; i < ukuranpop; i++) {
        float nilai = (float)rand()/RAND_MAX;
        int cal = mget(calinduk, i, 0);
        mfset(bilacak, i, 0, nilai);
        if(nilai < p) {
            mset(induk, i, 0, cal);
        } else {
            mset(induk, i, 0, 0);
        }
    }

}

matrix_t *
ga_indukco(const matrixf_t *fitnessk, const float pc, const int ukuranpop)
{
    /* Langkah 1: Memilih calon induk untuk di crossover */
    matrixf_t *bilacakco = mfinit(ukuranpop, 1);
    if(bilacakco == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    matrix_t *calindukco = minit(ukuranpop, 1);
    if(calindukco == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    _ga_caloninduk(bilacakco, calindukco, fitnessk, ukuranpop);

    printf("\n");
    printf("Kromosom\tF.Kumulatif\tRandom\t\tCalon Induk Cross Over\n");
    for(int i = 0; i < ukuranpop; i++) {
        float fk = mfget(fitnessk, i, 0);
        float acak = mfget(bilacakco, i, 0);
        int cal = mget(calindukco, i, 0);
        printf("%d\t\t%f\t%f\t%d\n", i+1, fk, acak, cal);
    }

    /* Langkah 2: Memilih induk berdasarkan nilai Pc */
    printf("\n");
    printf("Pemilihan induk cross over\n");
    printf("Nilai Pc = %f\n", pc);
    matrixf_t *bilacakcoi = mfinit(ukuranpop, 1);
    if(bilacakcoi == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    matrix_t *indukcoi = minit(ukuranpop, 1);
    if(indukcoi == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    _ga_induk(bilacakcoi, indukcoi, calindukco, pc, ukuranpop);

    printf("Calon Induk\tRandom\t\tInduk Cross Over\n");
    for(int i = 0; i < ukuranpop; i++) {
        int cal = mget(calindukco, i, 0);
        float acak = mfget(bilacakcoi, i, 0);
        int induk = mget(indukcoi, i, 0);
        printf("%d\t\t%f\t%d\n", cal, acak, induk);
    }

    mfree(calindukco);
    mffree(bilacakco);
    mffree(bilacakcoi);

    return indukcoi;
}

matrix_t *
ga_indukmu(const matrixf_t *fitnessk, const float pm, const int ukuranpop)
{
    /* Langkah 1: Memilih calon induk untuk di mutasi */
    matrixf_t *bilacakmu = mfinit(ukuranpop, 1);
    if(bilacakmu == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    matrix_t *calindukmu = minit(ukuranpop, 1);
    if(calindukmu == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    _ga_caloninduk(bilacakmu, calindukmu, fitnessk, ukuranpop);

    printf("\n");
    printf("Kromosom\tF.Kumulatif\tRandom\t\tCalon Induk Mutasi\n");
    for(int i = 0; i < ukuranpop; i++) {
        float fk = mfget(fitnessk, i, 0);
        float acak = mfget(bilacakmu, i, 0);
        int cal = mget(calindukmu, i, 0);
        printf("%d\t\t%f\t%f\t%d\n", i+1, fk, acak, cal);
    }

    /* Langkah 2: Memilih induk berdasarkan nilai Pm */
    printf("\n");
    printf("Pemilihan induk mutasi\n");
    printf("Nilai Pm = %f\n", pm);
    matrixf_t *bilacakmui = mfinit(ukuranpop, 1);
    if(bilacakmui == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    matrix_t *indukmui = minit(ukuranpop, 1);
    if(indukmui == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    _ga_induk(bilacakmui, indukmui, calindukmu, pm, ukuranpop);

    printf("Calon Induk\tRandom\t\tInduk Mutasi\n");
    for(int i = 0; i < ukuranpop; i++) {
        int cal = mget(calindukmu, i, 0);
        float acak = mfget(bilacakmui, i, 0);
        int induk = mget(indukmui, i, 0);
        printf("%d\t\t%f\t%d\n", cal, acak, induk);
    }

    mfree(calindukmu);
    mffree(bilacakmu);
    mffree(bilacakmui);

    return indukmui;
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
    printf("Pemilihan calon induk menggunakan seleksi roda rolet\n");
    printf("Perhitungan nilai fitness\n");
    matrixf_t *fitnessk = ga_fitnessk(bobot, ukuranpop);
    if(fitnessk == NULL) {
        printf("Err: tidak bisa mengalokasi fitness kumulatif ke memory\n");
        return EXIT_FAILURE;
    }
    float pc = 0.8;
    matrix_t *indukco = ga_indukco(fitnessk, pc, ukuranpop);
    if(indukco == NULL) {
        printf("Err: tidak bisa mengalokasi induk cross over ke memory\n");
        return EXIT_FAILURE;
    }
    float pm = 0.01;
    matrix_t *indukmu = ga_indukmu(fitnessk, pm, ukuranpop);
    if(indukmu == NULL) {
        printf("Err: tidak bisa mengalokasi induk mutilasi ke memory\n");
        return EXIT_FAILURE;
    }

    mfree(indukmu);
    mfree(indukco);
    mffree(fitnessk);
    mfree(bobot);
    mfree(popawal);
    return 0;
}

