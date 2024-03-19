#include <stdio.h>
#include <windows.h>

// Fungsi untuk meminta input nama dan umur, dan mencetak informasi dengan efek animasi
void login(char *nama, int *umur) {
    printf("Nama Anda: ");
    scanf("%s", nama);
    printf("Umur Anda: ");
    scanf("%d", umur);

    printf("Selamat datang, %s!\n", nama);
    Sleep(1000); // Jeda 1 detik
    printf("\n");
    Sleep(500); // Jeda 0.5 detik
    printf("Loading.");
    Sleep(500); // Jeda 0.5 detik
    printf(".");
    Sleep(500); // Jeda 0.5 detik
    printf(".");
    Sleep(500); // Jeda 0.5 detik
    printf(".\n");
    Sleep(1000); // Jeda 1 detik
    printf("Login berhasil!\n\n");
    Sleep(1500); // Jeda 0.5 detik
    system("cls");
}

int main() {
    char nama[50];
    int umur;

    // Memanggil fungsi login
    login(nama, &umur);

    // Menggunakan nilai nama dan umur yang diperoleh dari fungsi login
    printf("Dalam fungsi main: Nama: %s, Umur: %d\n", nama, umur);

    return 0;
}
