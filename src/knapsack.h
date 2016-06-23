#ifndef KNAPSACK
#define KNAPSACK

struct subruang {
    int id;
    int tinggi;
    int lebar;
};
typedef struct subruang subruang_t;

subruang_t *sr_create(int id, int tinggi, int lebar);
void sr_free(subruang_t *subruang);

int knapsack_pack(const int *widths, const int *heights, const int *urutan,
        const int jumlah_barang);

#endif
