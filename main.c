#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur data untuk menyimpan informasi barang
struct Barang {
    int id;
    char kode[10];
    char nama[100];
    char brand[100];
    char jenis[100];
    float hargaBeli;
    float hargaJual;
    int stok;
    char updateTerakhir[100];
};

// Struktur data untuk node dalam binary search tree
struct Node {
    struct Barang data;
    struct Node* left;
    struct Node* right;
};

// Fungsi untuk membuat node baru dalam binary search tree
struct Node* createNode(struct Barang barang) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = barang;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Fungsi untuk menambahkan barang ke dalam binary search tree
struct Node* insertNode(struct Node* root, struct Barang barang) {
    // Jika tree kosong, buat node baru dan jadikan sebagai root
    if (root == NULL) {
        return createNode(barang);
    }

    // Jika kode barang yang akan ditambahkan lebih kecil, sisipkan ke kiri
    if (strcmp(barang.kode, root->data.kode) < 0) {
        root->left = insertNode(root->left, barang);
    }
    // Jika kode barang yang akan ditambahkan lebih besar, sisipkan ke kanan
    else if (strcmp(barang.kode, root->data.kode) > 0) {
        root->right = insertNode(root->right, barang);
    }

    return root;
}

// Fungsi untuk mencari barang berdasarkan kode barang dalam binary search tree
struct Node* searchNode(struct Node* root, char* kode) {
    // Jika tree kosong atau kode barang yang dicari ditemukan di root, kembalikan root
    if (root == NULL || strcmp(kode, root->data.kode) == 0) {
        return root;
    }

    // Jika kode barang yang dicari lebih kecil, cari di anak kiri
    if (strcmp(kode, root->data.kode) < 0) {
        return searchNode(root->left, kode);
    }
    // Jika kode barang yang dicari lebih besar, cari di anak kanan
    else {
        return searchNode(root->right, kode);
    }
}

// Fungsi untuk menghapus node dengan kode barang tertentu dalam binary search tree
struct Node* deleteNode(struct Node* root, char* kode) {
    // Jika tree kosong, kembalikan NULL
    if (root == NULL) {
        return root;
    }

    // Jika kode barang yang akan dihapus lebih kecil, cari di anak kiri
    if (strcmp(kode, root->data.kode) < 0) {
        root->left = deleteNode(root->left, kode);
    }
    // Jika kode barang yang akan dihapus lebih besar, cari di anak kanan
    else if (strcmp(kode, root->data.kode) > 0) {
        root->right = deleteNode(root->right, kode);
    }
    // Jika kode barang yang akan dihapus sama dengan kode di root
    else {
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }

        // Jika root memiliki dua anak
        struct Node* minValueNode = root->right;
        while (minValueNode->left != NULL) {
            minValueNode = minValueNode->left;
        }

        // Salin data dari node dengan nilai terendah ke root
        root->data = minValueNode->data;

        // Hapus node dengan nilai terendah dari anak kanan root
        root->right = deleteNode(root->right, minValueNode->data.kode);
    }

    return root;
}

// Fungsi untuk menampilkan informasi barang dalam bentuk tabel
void displayBarangTable(struct Barang barang) {
    printf("| %-6d | %-10s | %-30s | %-20s | %-20s | %-10.2f | %-10.2f | %-5d | %-20s |\n",
        barang.id, barang.kode, barang.nama, barang.brand, barang.jenis, barang.hargaBeli,
        barang.hargaJual, barang.stok, barang.updateTerakhir);
}

// Fungsi untuk melakukan traversal inorder pada binary search tree dan menampilkan semua barang dalam bentuk tabel
void inorderTraversal(struct Node* root) {
    printf("+--------+------------+--------------------------------+----------------------+----------------------+------------+------------+-------+----------------------+\n");
    printf("| No ID  | Kode Barang | Nama Barang                     | Brand                | Jenis Produk         | Harga Beli | Harga Jual | Stok  | Update Terakhir      |\n");
    printf("+--------+------------+--------------------------------+----------------------+----------------------+------------+------------+-------+----------------------+\n");

    if (root != NULL) {
        inorderTraversal(root->left);
        displayBarangTable(root->data);
        inorderTraversal(root->right);
    }
}

int main() {
    char email[100];
    char password[100];

    printf("=== Login ===\n");
    printf("Email: ");
    scanf("%s", email);
    printf("Password: ");
    scanf("%s", password);

    // Validasi email
    int validEmail = 0;
    if (strstr(email, "@") != NULL && (strstr(email, ".com") != NULL || strstr(email, ".co.id") != NULL)) {
        validEmail = 1;
    }

    // Cek login berhasil atau tidak
    if (validEmail && strcmp(password, "password123") == 0) {
        printf("Login berhasil!\n");

        // Baca file txt untuk mengambil data barang
        FILE* file = fopen("inventory.txt", "r");
        if (file == NULL) {
            printf("File tidak ditemukan!\n");
            return 0;
        }
        struct Node* root = NULL;
        struct Barang barang;
        while (fscanf(file, "%d %s %s %s %s %f %f %d %[^\n]",
                      &barang.id, barang.kode, barang.nama, barang.brand,
                      barang.jenis, &barang.hargaBeli, &barang.hargaJual,
                      &barang.stok, barang.updateTerakhir) != EOF) {
            root = insertNode(root, barang);
        }

        fclose(file);

        // Menu
        int choice;
        char searchKode[10];
        struct Node* searchResult;

        do {
            printf("\n=== Menu ===\n");
            printf("1. Tampilkan Semua Barang\n");
            printf("2. Tambah Barang\n");
            printf("3. Hapus Barang\n");
            printf("4. Cari Barang\n");
            printf("5. Keluar\n");
            printf("Pilihan: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("\n=== Daftar Barang ===\n");
                    inorderTraversal(root);
                    break;
                case 2:
                    printf("\n=== Tambah Barang ===\n");
                    struct Barang newBarang;
                    printf("ID: ");
                    scanf("%d", &newBarang.id);
                    printf("Kode Barang: ");
                    scanf("%s", newBarang.kode);
                    printf("Nama Barang: ");
                    scanf(" %[^\n]", newBarang.nama);
                    printf("Brand: ");
                    scanf(" %[^\n]", newBarang.brand);
                    printf("Jenis Produk: ");
                    scanf(" %[^\n]", newBarang.jenis);
                    printf("Harga Beli: ");
                    scanf("%f", &newBarang.hargaBeli);
                    printf("Harga Jual: ");
                    scanf("%f", &newBarang.hargaJual);
                    printf("Stok: ");
                    scanf("%d", &newBarang.stok);
                    printf("Update Terakhir: ");
                    scanf(" %[^\n]", newBarang.updateTerakhir);
                    root = insertNode(root, newBarang);

                    // Simpan data ke file inventory.txt
                    FILE* file = fopen("inventory.txt", "a");
                    if (file != NULL) {
                        fprintf(file, "%d %s %s %s %s %.2f %.2f %d %s\n",
                                newBarang.id, newBarang.kode, newBarang.nama, newBarang.brand,
                                newBarang.jenis, newBarang.hargaBeli, newBarang.hargaJual,
                                newBarang.stok, newBarang.updateTerakhir);
                        fclose(file);
                        printf("Barang berhasil ditambahkan!\n");
                    } else {
                        printf("Gagal menyimpan data!\n");
                    }
                    break;
                case 3:
                    printf("\n=== Hapus Barang ===\n");
                    printf("Kode Barang yang akan dihapus: ");
                    scanf("%s", searchKode);
                    searchResult = searchNode(root, searchKode);
                    if (searchResult == NULL) {
                        printf("Barang dengan kode %s tidak ditemukan.\n", searchKode);
                    } else {
                        root = deleteNode(root, searchKode);
                        printf("Barang dengan kode %s berhasil dihapus.\n", searchKode);
                    }
                    break;
                case 4:
                    printf("\n=== Cari Barang ===\n");
                    printf("Kode Barang yang akan dicari: ");
                    scanf("%s", searchKode);
                    searchResult = searchNode(root, searchKode);
                    if (searchResult == NULL) {
                        printf("Barang dengan kode %s tidak ditemukan.\n", searchKode);
                    } else {
                        printf("\n=== Hasil Pencarian ===\n");
                        printf("+--------+------------+--------------------------------+----------------------+----------------------+------------+------------+-------+----------------------+\n");
                        printf("| No ID  | Kode Barang | Nama Barang                     | Brand                | Jenis Produk         | Harga Beli | Harga Jual | Stok  | Update Terakhir      |\n");
                        printf("+--------+------------+--------------------------------+----------------------+----------------------+------------+------------+-------+----------------------+\n");
                        displayBarangTable(searchResult->data);
                    }
                    break;
                case 5:
                    printf("Terima kasih!\n");
                    break;
                default:
                    printf("Pilihan tidak valid.\n");
                    break;
            }
        } while (choice != 5);
    } else {
        printf("Login gagal!\n");
    }

    return 0;
}
