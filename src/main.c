/* Main program */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "knapsack.h"
#include "matrix.h"
#include "vector.h"

matrix_t *ga_bangkit(int ukuranpop, int jumlahb);
vector_t *ga_eval(matrix_t *popawal, const int *datatb, const int *datalb,
        int ukuranpop, int jumlahb);
vectorf_t *ga_fitnessk(vector_t *bobot, int ukuranpop);
vector_t *ga_indukco(const vectorf_t *fitnessk, const float pc, const int ukuranpop);
vector_t *ga_indukmu(const vectorf_t *fitnessk, const float pm, const int ukuranpop);

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

vector_t *
ga_eval(matrix_t *popawal, const int *datatb, const int *datalb, int ukuranpop,
        int jumlahb)
{
    /* Alokasi bobot ke memory */
    vector_t *bobot = vinit(ukuranpop);
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
        vpush(bobot, tinggi);
    }

    return bobot;
}

vectorf_t *
ga_fitnessk(vector_t *bobot, int ukuranpop) {
    /* Langkah 1: Menghitung total bobot keseluruhan */
    int totalbobot = 0;
    for(int i = 0; i < ukuranpop; i++) {
        totalbobot += vget(bobot, i);
    }

    /* Langkah 2: Menghitung nilai fitness tiap kromosom */
    int totalfitness = 0;
    vector_t *fitness = vinit(ukuranpop);
    if(fitness == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    for(int i = 0; i < ukuranpop; i++) {
        int nilai = totalbobot - vget(bobot, i);
        vpush(fitness, nilai);
        totalfitness += nilai;
    }

    /* Langkah 3: menghitung nilai fitness relative */
    vectorf_t *fitnessr = vfinit(ukuranpop);
    if(fitnessr == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    /* Langkah 4: menghitung nilai fitness kumulatif */
    float totalfitnessk = 0.0;
    vectorf_t *fitnessk = vfinit(ukuranpop);
    if(fitnessk == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    for(int i = 0; i < ukuranpop; i++) {
        float nilai = (float)vget(fitness, i)/totalfitness;
        totalfitnessk += nilai;
        vfpush(fitnessr, nilai);
        vfpush(fitnessk, totalfitnessk);
    }

    printf("Kromosom\tFitness\t\tF. Relative\tF. Kumulatif\n");
    for(int i = 0; i < ukuranpop; i++) {
        int f = vget(fitness, i);
        float fr = vfget(fitnessr, i);
        float fk = vfget(fitnessk, i);
        printf("%d\t\t%d\t\t%f\t%f\n", i+1, f, fr, fk);
    }

    vdestroy(fitness);
    vfdestroy(fitnessr);

    return fitnessk;
}

/* Mencari calon induk dari nilai fitness kumulatif */
vector_t *
_ga_caloninduk(const vectorf_t *fitnessk, const int ukuranpop)
{
    vector_t *calinduki = vinit(ukuranpop);
    if(calinduki == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    printf("Random Ke\tF.Kumulatif\tRandom\t\tCalon Induk\n");
    float fk1 = vfget(fitnessk, 0);
    for(int i = 0; i < ukuranpop; i++) {
        float r = (float)rand()/RAND_MAX;
        float fki = vfget(fitnessk, i);
        int calinduk = 0;
        if(r <= fk1) {
            vpush(calinduki, 1);
            calinduk = 1;
            printf("%d\t\t%f\t%f\t%d\n", i+1, fki, r, calinduk);
            continue;
        }

        for(int k = 0; k < ukuranpop-1; k++) {
            float fk = vfget(fitnessk, k);
            float fks = vfget(fitnessk, k+1);
            if(fk < r && r <= fks) {
                vpush(calinduki, k+2);
                calinduk = k + 2;
                printf("%d\t\t%f\t%f\t%d\n", i+1, fki, r, calinduk);
                break;
            }
        }
    }

    return calinduki;
}

/* Mencari induk dari calon induk dan nilai p */
vector_t *
_ga_induk(const vector_t *calinduki, const float p, const int ukuranpop)
{
    vector_t *induki = vinit(ukuranpop);
    if(induki == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    printf("Calon Induk\tRandom\t\tInduk\n");
    for(int i = 0; i < ukuranpop; i++) {
        float r = (float)rand()/RAND_MAX;
        int cali = vget(calinduki, i);
        int induk = 0;
        if(r < p) {
            vpush(induki, cali);
            induk = cali;
        }
        printf("%d\t\t%f\t%d\n", cali, r, induk);
    }

    /* Jika jumlah induk yang terpilih ganjil, maka kurangi satu */
    if(induki->currentn % 2 != 0) {
        vpop(induki);
    }

    return induki;
}

vector_t *
ga_indukco(const vectorf_t *fitnessk, const float pc, const int ukuranpop)
{
    /* Langkah 1: Memilih calon induk untuk di crossover */
    printf("\n");
    printf("Pemilihan calon induk cross over\n");
    vector_t *calinduki = _ga_caloninduk(fitnessk, ukuranpop);

    /* Langkah 2: Memilih induk berdasarkan nilai Pc */
    printf("\n");
    printf("Pemilihan induk cross over\n");
    printf("Nilai Pc = %f\n", pc);
    vector_t *induki = _ga_induk(calinduki, pc, ukuranpop);

    vdestroy(calinduki);
    return induki;
}

vector_t *
ga_indukmu(const vectorf_t *fitnessk, const float pm, const int ukuranpop)
{
    /* Langkah 1: Memilih calon induk untuk di mutasi */
    printf("\n");
    printf("Pemilihan calon induk mutasi\n");
    vector_t *calinduki = _ga_caloninduk(fitnessk, ukuranpop);

    /* Langkah 2: Memilih induk berdasarkan nilai Pm */
    printf("\n");
    printf("Pemilihan induk mutasi\n");
    printf("Nilai Pm = %f\n", pm);
    vector_t *induki = _ga_induk(calinduki, pm, ukuranpop);

    vdestroy(calinduki);
    return induki;
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
    vector_t *bobot = ga_eval(popawal, datatb, datalb, ukuranpop, jumlahb);
    if(bobot == NULL) {
        printf("Err: Evaluasi populasi awal gagal\n");
        return EXIT_FAILURE;
    }

    printf("Hasil Evaluasi:\n");
    printf("Kromosom\tBobot\n");
    for(int i = 0; i < ukuranpop; i++) {
        int tinggi = vget(bobot, i);
        printf("%d\t\t%d\n", i+1, tinggi);
    }

    printf("\n");
    printf("Langkah 3: Seleksi menggunakan Roda Rolet\n");
    printf("Perhitungan nilai fitness\n");
    vectorf_t *fitnessk = ga_fitnessk(bobot, ukuranpop);
    if(fitnessk == NULL) {
        printf("Err: tidak bisa mengalokasi fitness kumulatif ke memory\n");
        return EXIT_FAILURE;
    }
    float pc = 0.8;
    vector_t *indukcoi = ga_indukco(fitnessk, pc, ukuranpop);
    if(indukcoi == NULL) {
        printf("Err: tidak bisa mengalokasi induk cross over ke memory\n");
        return EXIT_FAILURE;
    }
    float pm = 0.01;
    vector_t *indukmui = ga_indukmu(fitnessk, pm, ukuranpop);
    if(indukmui == NULL) {
        printf("Err: tidak bisa mengalokasi induk mutilasi ke memory\n");
        return EXIT_FAILURE;
    }

    printf("\n");
    printf("Total Induk:\n");
    printf("\tCross Over: %d induk\n", indukcoi->currentn);
    printf("\tMutasi: %d induk\n", indukmui->currentn);

    vdestroy(indukmui);
    vdestroy(indukcoi);
    vfdestroy(fitnessk);
    vdestroy(bobot);
    mfree(popawal);
    return 0;
}

