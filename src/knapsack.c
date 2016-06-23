#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include "knapsack.h"

/* sr_create: fungsi untuk mengalokasi dan menginialisasi subruang */
subruang_t *
sr_create(int id, int tinggi, int lebar)
{
    subruang_t *sr = (subruang_t *)malloc(sizeof(subruang_t));
    if(sr == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    sr->id = id;
    sr->tinggi = tinggi;
    sr->lebar = lebar;

    return sr;
}

/* sr_free: fungsi untuk mengfree kan subruang dari memory */
void
sr_free(subruang_t *sr)
{
    assert(sr != NULL);

    free(sr);
}

/* knapsack_pack: fungsi untuk penyusunan barang;
 * Input:
 *  - datalb = data lebar tiap barang
 *  - datatb = daat tinggi tiap barang
 *  - urutanb = urutan barang
 *  - jumlahb = jumlah barang
 *
 * Output:
 *  - Tinggi susunan barang
 *
 * Algoritma:
 * Kita susun barang sesuai urutan ke dalam ruangan yang punya maksimal lebar
 * L dan tinggi ruangan tak terbatas
 * 1. Untuk barang urutan pertama dengan tinggi h1 dan lebar w1, kita langsung susun
 *    ke dalam ruangan. Maka kita punya subruang baru dengan tinggi h1 dan lebar L-w1
 * 2. Untuk barang kedua dan lainnya:
 *    2.a. Kita cek apakah barang muat di subruang yang sudah ada.
 *         Untuk setiap subruang kita cek:
 *         Jika muat:
 *             - kita masukkan barang ke subruang tersebut
 *             - update tinggi subruang
 *             - buat subruang baru dengan ukuran sesuai barang
 *         Jika tidak muat:
 *             - Balik barangnya (tinggi jadi lebar, dan sebaliknya)
 *             - Cek jika muat, lakukan sesuai prosedur jika muat diatas
 *             - jikat tidak muat, cek subruang lain
 *    2.b. Jika tidak ada subruang yang dapat dimuati oleh barang, maka buat
 *    subruang baru dengan tinggi sesuai barang dan lebar (L-lebar barang).
 * 3. Kita hitung tinggi susunan barangnya
 */
int
knapsack_pack(const int *datalb, const int *datatb, const int *urutanb,
        const int jumlahb)
{
    int LEBAR_MAKS = 20;
    int tinggi_total = 0;
    /* catat jumlah subruang */
    int jumlahsr = 0;

    printf("Penyusunan barang dimulai:\n");
    printf("Lebar maksimal: %d\n", LEBAR_MAKS);

    /* inialisasi ruang, ini untuk meyimpan subruang yg ada */
    subruang_t **ruang = (subruang_t **)malloc(1 * sizeof(subruang_t *));
    if(ruang == NULL) {
        errno = ENOMEM;
        return -1;
    }

    for(int i = 0; i < jumlahb; i++) {
        /* simpan informasi barang ke-i */
        int index = urutanb[i] - 1;
        int lebarb = datalb[index];
        int tinggib = datatb[index];

        printf("\tUrutan nomor %d\n", i+1);
        printf("\tBarang %d (T:%d L:%d)\n", index+1, tinggib, lebarb);

        /* Kasus 1: barang pertama. Langsung susun dan buat subruang baru */
        if(i == 0) {
            /* Iniliasisasi subruang pertama */
            int lebarsr = LEBAR_MAKS - lebarb;
            int tinggisr = tinggib;
            tinggi_total += tinggisr;

            printf("\t\tBarang pertama, belum ada subruang.\n");
            printf("\t\tPembuatan subruang baru T: %d L: %d\n", tinggisr, lebarsr);
            printf("\t\tUpdate tinggi total\t: %d\n", tinggi_total);

            jumlahsr += 1;
            subruang_t *subruang = sr_create(jumlahsr, tinggisr, lebarsr);
            if(subruang == NULL) {
                errno = ENOMEM;
                free(ruang);
                return -1;
            }
            *ruang = subruang;

            /* langsung cek barang lain */
            continue;
        }

        /* Kasus 2: barang selain barang pertama dan muat di subruang yang sudah
         * ada. Disini tanpa ada penambahan tinggi_total. */
        int masuksr = 0; /* Kita simpan informasi barang masuk subruang atau tidak */
        printf("\t\tKita cek apakah barang muat di subruang yang sudah ada\n");
        for(int j = 0; j < jumlahsr; j++) {
            subruang_t *subruang = *(ruang + j);
            printf("\t\tApakah muat di subruang #%d? (T: %d L: %d)\n",
                    subruang->id, subruang->tinggi, subruang->lebar);

            /* Kasus 2.1: barang muat tanpa di balik,
             * cek apakah muat di subruang ini */
            if(tinggib <= subruang->tinggi && lebarb <= subruang->lebar) {
                /* jika muat, maka buat subruang baru dan update tinggi dari
                 * subruang yang dimuati oleh barang ini */
                printf("\t\tBarang muat di subruang #%d tanpa di balik.\n",
                        subruang->id);

                int tinggisr = tinggib;
                int lebarsr = subruang->lebar - lebarb;
                /* update tinggi subruang yang di tempati benda ini */
                subruang->tinggi -= tinggisr;
                if(subruang->tinggi == 0) subruang->lebar = 0;
                if(lebarsr == 0) {
                    printf("\t\tTidak ada ruang lebar yang tersisa dalam"
                            " subruang %d\n", subruang->id);
                    masuksr = 1;
                    break;
                }

                jumlahsr += 1;
                subruang_t *srbaru = sr_create(jumlahsr, tinggisr, lebarsr);
                if(srbaru == NULL) {
                    errno = ENOMEM;
                    for(int i = 0; i < jumlahsr; i++) {
                        sr_free(*(ruang + i));
                    }
                    free(ruang);
                    return -1;
                };
                printf("\t\tSubruang baru ID: %d T: %d L: %d\n", srbaru->id,
                        srbaru->tinggi, srbaru->lebar);

                /* realloc untuk subruang baru dan penempatan subruang */
                ruang = (subruang_t **)realloc(ruang,
                        jumlahsr * sizeof(subruang_t *));
                if(ruang == NULL) {
                    errno = ENOMEM;
                    for(int i = 0; i < jumlahsr-1; i++) {
                        sr_free(*(ruang + i));
                    }
                    free(ruang);
                    return -1;
                };
                for(int k = jumlahsr-1; k > j; k--) {
                    *(ruang + k) = *(ruang + (k-1));
                }
                *(ruang + j) = srbaru;

                /* break untuk cek barang selanjutnya */
                masuksr = 1;
                break;
            }

            /* Kasus 2.2: barang muat dengan di balik. Width jadi height dan
             * sebaliknya */
            if(tinggib <= subruang->lebar && lebarb <= subruang->tinggi) {
                /* Jika muat, maka buat subruang baru dan update tinggi dari
                 * subruang yang dimuati barang ini */
                printf("\t\tBarang muat di subruang #%d dengan di balik.\n",
                        subruang->id);

                int tinggisr = lebarb;
                int lebarsr = subruang->lebar - tinggib;
                /* update tinggi subruang yang di tempati benda ini */
                subruang->tinggi -= tinggisr;
                if(subruang->tinggi == 0) subruang->lebar = 0;
                if(lebarsr == 0) {
                    printf("\t\tTidak ada ruang lebar yang tersisa dalam"
                            " subruang %d\n", subruang->id);
                    masuksr = 1;
                    break;
                }

                jumlahsr += 1;
                subruang_t *srbaru = sr_create(jumlahsr, tinggisr, lebarsr);
                if(srbaru == NULL) {
                    errno = ENOMEM;
                    for(int i = 0; i < jumlahsr-1; i++) {
                        sr_free(*(ruang + i));
                    }
                    free(ruang);
                    return -1;
                };
                printf("\t\tSubruang baru ID: %d T: %d L: %d\n", srbaru->id,
                        srbaru->tinggi, srbaru->lebar);

                /* realloc untuk subruang baru dan penempatan subruang */
                ruang = (subruang_t **)realloc(ruang,
                        jumlahsr * sizeof(subruang_t *));
                if(ruang == NULL) {
                    errno = ENOMEM;
                    for(int i = 0; i < jumlahsr-1; i++) {
                        sr_free(*(ruang + i));
                    }
                    free(ruang);
                    return -1;
                };
                for(int k = jumlahsr-1; k > j; k--) {
                    *(ruang + k) = *(ruang + (k-1));
                }
                *(ruang + j) = srbaru;

                /* break untuk cek barang selanjutnya */
                masuksr = 1;
                break;
            }
        }

        /* Kasus 3: barang tidak muat di subruang yang sudah ada. Maka buat
         * subruang baru dan update tinggi_total nya */
        if(!masuksr) {
            printf("\t\tBarang tidak muat di semua subruang.\n");

            /* buat subruang baru dan update tinggi_total nya */
            int tinggisr = tinggib;
            int lebarsr = LEBAR_MAKS - lebarb;
            tinggi_total += tinggisr;

            jumlahsr += 1;
            subruang_t *subruang = sr_create(jumlahsr, tinggisr, lebarsr);
            if(subruang == NULL) {
                errno = ENOMEM;
                for(int i = 0; i < jumlahsr-1; i++) {
                     sr_free(*(ruang + i));
                }
                free(ruang);
                return -1;
            };
            printf("\t\tSubruang baru ID: %d T: %d L: %d\n", subruang->id,
                subruang->tinggi, subruang->lebar);

            /* realloc untuk subruang baru */
            ruang = (subruang_t **)realloc(ruang, jumlahsr * sizeof(subruang_t *));
            if(ruang == NULL) {
                errno = ENOMEM;
                for(int i = 0; i < jumlahsr-1; i++) {
                    sr_free(*(ruang + i));
                }
                free(ruang);
                return -1;
            };
            *(ruang + (jumlahsr - 1)) = subruang;
        }
    }

    for(int i = 0; i < jumlahsr; i++) {
        sr_free(*(ruang + i));
    }
    free(ruang);

    return tinggi_total;
}
