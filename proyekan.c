#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#ifdef _WIN32
#define CLEAR "cls" // buat windows
#define PAUSE "pause"
#elif TARGET_OS_X
#define CLEAR "clear" // buat mac
#define PAUSE "pause"
#else
#define PAUSE "read -n1 -r -p \"Press any key to continue...\" key"
#define CLEAR "clear" // buat Unix/Linux
#endif

#define DEVICE_MAX 30
#define CATEGORY_MAX 20
#define PERIOD_MAX 5
typedef unsigned int uint;

// Struct Declarations
typedef struct Category Category;
typedef struct Period Period;
typedef struct Device Device;


typedef enum bool
{
    false, true //untuk membuat boolean dalam c
} bool;


struct Category {
    char name[16]; // Nama (maksimal 16 karakter)
    uint id;       // identifikasi
    float totalConsumption;
    float totalWatt;
    Device *devices[5]; // Device pada kategori ini
    uint device_num;
};

struct Period {
    char begin[6]; // Jam mulai
    char end[6];   // Jam selesai
};

struct Device {
    Category *category;
    Period periods[PERIOD_MAX];    // Maksimal 5 periode penggunaan
    uint activeMinutes; // Menit aktif
    uint period_num;    // Jumlah periode penggunaan
    float power;        // Watt
    float energyDaily;  // Total konsumsi kwh
};


// Deklarasi Fungsi
void printDevice(Device *device);
void addDevice(Device devices[], uint *deviceCount, Category categories[], uint *categoryCount);
void addPeriod(Device *device);
bool checkTimeFormat(char time[]);
void addCategory(Category categories[], uint *categoryCount);
void addCategoryManual(Category categories[], uint *categoryCount, char name[]);
uint parsePeriod(Period *period);
void updateConsumption(Device *device);
void printAllDevices(Category *categories, uint *categoryCount);
void printLogo();
void animateLogo();
void help();
uint getMenu();
void printHistogram(Category *categories[], uint categoryCount);
void swap(Category **a, Category **b);
bool comp(Category *a, Category *b);
void bubbleSort(Category *array[], int length, bool (*comp)(Category*, Category*));
void showStats(Category *sort_container[], uint categoryCount);
void defaultCategories(Category categories[], uint *categoryCount);
void printTips(uint id);
void printCredits();
void printToFile(char *nama, int *umur, Category categories[], uint *categoryCount);
void login(char *nama, int *umur);
void searchTotalConsumption(Category categories[], int select, uint categoryCount);
void searchWatt(Category categories[], int select, uint categoryCount);

int main(void) 
{
    char nama[50];
    int umur;

    //login
    login(nama, &umur);
    
    animateLogo();

    Device devices[DEVICE_MAX];
    Category categories[CATEGORY_MAX];
    Category *sort_container[CATEGORY_MAX];

    uint i;
    for (i = 0; i < CATEGORY_MAX; ++i)
    {
        sort_container[i] = &categories[i];
    }
    uint deviceCount = 0;
    uint categoryCount = 0;

    defaultCategories(categories, &categoryCount);


    uint menuInput;
    do {
        menuInput = getMenu();  
        switch (menuInput) 
        {
            case 1:
                addDevice(devices, &deviceCount, categories, &categoryCount);
                break;
            case 2:
                if (deviceCount <= 0)
                {
                    printf("PERANGKAT KOSONG!\n");
                    system(PAUSE);
                    break;
                }
                printAllDevices(categories, &categoryCount);
                break;
            case 3:
                if (deviceCount <= 0)
                {
                    printf("PERANGKAT KOSONG!\n");
                    system(PAUSE);
                    break;
                }
                showStats(sort_container, categoryCount);
                break;
            case 4:
                if (deviceCount <= 0)
                {
                    printf("PERANGKAT KOSONG!\n");
                    system(PAUSE);
                    break;
                }
                printToFile(nama, &umur, categories, &categoryCount);
                printf("File baru telah berhasil dibuat\n");
                system(PAUSE);
                break;
            case 5:
                if (deviceCount <= 0)
                {
                    printf("PERANGKAT KOSONG!\n");
                    system(PAUSE);
                    break;
                }

                system("cls");
                printf("|==================================================|\n");
                printf("|                   Searching Menu                 |\n");
                printf("|==================================================|\n");
                printf("| 1. Cari Berdasarkan Total Penggunaan Terbanyak   |\n");
                printf("| 2. Cari Berdasarkan Total Penggunaan Tersedikit  |\n");
                printf("| 3. Cari Berdasarkan Watt Terbesar                |\n");
                printf("| 4. Cari Berdasarkan Watt Terkecil                |\n");
                printf("| 5. Exit                                          |\n");
                printf("|==================================================|\n");
                uint menuInput;
                printf("\nMasukkan input (1 - 5) : ");
                scanf("%d", &menuInput);
                    if (menuInput == 1){
                        searchTotalConsumption(categories, 1, categoryCount);
                        system(PAUSE);
                    } else if (menuInput == 2){
                        searchTotalConsumption(categories, 2, categoryCount);
                        system(PAUSE);
                    } else if (menuInput == 3){
                        searchWatt(categories, 1, categoryCount);
                        system(PAUSE);
                    } else if (menuInput == 4){
                        searchWatt(categories, 2, categoryCount);
                        system(PAUSE);
                    } else if (menuInput == 5){
                        break;
                    } else {
                        printf("Input Invalid!\n");
                        Sleep(500);
                        system("cls");
                    }
                break;
            case 6:
                help();
                break;
            case 7:
                printCredits();
                break;
            case 8:
                break;
            default:
            printf("Input tidak valid!\n");
        }
    } while (menuInput != 8);

    return 0;
}

void searchWatt(Category categories[], int select, uint categoryCount){
    int i, temp;
    switch(select){
        case 1:
            for (i = 0; i<categoryCount - 1; i++){
                if(categories[i].totalWatt > categories[i+1].totalWatt){
                    temp = i;
                }
            }
            printf("\nKategori dengan daya terbesar: %s\n", categories[temp].name);
            printf("Total daya: %.2f watt\n", categories[temp].totalWatt);
            break;
        case 2:
            for (i = 0; i<categoryCount - 1; i++){
                if(categories[i].totalWatt < categories[i+1].totalWatt){
                    temp = i;
                }
            }
            printf("\nKategori dengan daya terkecil: %s\n", categories[temp].name);
            printf("Total daya: %.2f watt\n", categories[temp].totalWatt);
            break;
        default:
            break;
    }
}


void searchTotalConsumption(Category categories[], int select, uint categoryCount){
    int i, j;

    switch (select)
    {
    // Mencari dari terbanyak
    case 1:
        printf("\nKategori dengan konsumsi terbanyak: %s\n", categories[categoryCount-1].name);
        printf("Total konsumsi daya: %.2f kwh\n", categories[categoryCount].totalConsumption);
        break;
    // Mencari dari tersedikit
    case 2:
        printf("\nKategori dengan konsumsi paling sedikit: %s\n", categories[0].name);
        printf("Total konsumsi daya: %.2f kwh\n", categories[0].totalConsumption);
        break;
    default:
        break;
    }
}

void printToFile(char *nama, int *umur, Category categories[], uint *categoryCount){
    uint i;
    char namaFile[100];

    // Membuat Nama File Baru
    strcpy(namaFile, nama);
    strcat(namaFile, "_data.txt");

    FILE *filePointer;

    // Membuka file untuk ditulis
    filePointer = fopen(namaFile, "w");

    // Memeriksa apakah file berhasil dibuka
    if (filePointer == NULL) {
        printf("File tidak dapat dibuka atau dibuat.\n");
        return;
    }

    // Menulis data ke dalam file
    fprintf(filePointer, "================\n");
    fprintf(filePointer, "Nama Anda: %s\n", nama);
    fprintf(filePointer, "Umur Anda: %d\n", umur);
    fprintf(filePointer, "================\n\n");

    for (i = 0; i < *categoryCount; ++i)
    {
        if (categories[i].device_num == 0)
        {
            continue;
        }

        fprintf(filePointer, "===========\n");
        fprintf(filePointer, "Kategori: %s\n", categories[i].name);
        fprintf(filePointer, "------------------\n");
        uint j;
        for (j = 0; j < categories[i].device_num; ++j)
        {
            Device *device = categories[i].devices[j];
            fprintf(filePointer, "Device %d:\n", j + 1);
            fprintf(filePointer, "Konsumsi Daya: %.2f watt\n", device->power);
            fprintf(filePointer, "Periode Penggunaan:\n");
            uint i;
            for (i = 0; i < device->period_num; ++i)
            {
                fprintf(filePointer, "* %s - %s\n", device->periods[i].begin, device->periods[i].end);
            }
            fprintf(filePointer, "Waktu Aktif Harian: %u menit\n", device->activeMinutes);
            fprintf(filePointer, "Konsumsi Harian: %.2f kwh\n", device->energyDaily);
            fprintf(filePointer, "-------------\n");
        }
        fprintf(filePointer, "Total Konsumsi Harian: %.2f kwh\n", categories[i].totalConsumption);
        fprintf(filePointer, "========\n\n");
    }

    // Menutup file
    fclose(filePointer);
}

// Fungsi untuk meminta input nama dan umur, dan mencetak informasi dengan efek animasi
void login(char *nama, int *umur) {
    printf("===== LOGIN =====\n");
    printf("Nama Anda: ");
    scanf("%s", nama);
    printf("Umur Anda: ");
    scanf("%d", umur);
    printf("=================\n");

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

void printDevice(Device *device)
{
    printf("Konsumsi Daya: %.2f watt\n", device->power);
    printf("Periode Penggunaan:\n");
    uint i;
    for (i = 0; i < device->period_num; ++i)
    {
        printf("* %s - %s\n", device->periods[i].begin, device->periods[i].end);
    }
    printf("Waktu Aktif Harian: %u menit\n", device->activeMinutes);
    printf("Konsumsi Harian: %.2f kwh\n", device->energyDaily);
    printf("-------------\n");
}

void addDevice(Device devices[], uint *deviceCount, Category categories[], uint *categoryCount)
{
    system(CLEAR);
    uint categoryChoice;
    uint i;

    if (*deviceCount >= DEVICE_MAX)
    {
        printf("PERANGKAT PENUH!\n");
        return;
    }

    printf("--------\nPilih kategori perangkat\n------------\n");
    printf("Jumlah Perangkat Sekarang: %u\n", *deviceCount);
    printf("Jumlah Kategori Tersedia: %u\n", *categoryCount);
    printf("--------------------\n");
    for (i = 0; i < *categoryCount; ++i) // Kategori yang sudah ada
    {
        printf("%d. %s\n", i + 1, categories[i].name);
    }
    printf("---------------\n");
    printf("%d. Tambah Kategori Baru\n", i + 1);
    printf("Pilih Opsi: ");
    scanf("%u", &categoryChoice);

    if (categoryChoice == i + 1)
    {
        if (*categoryCount >= CATEGORY_MAX)
        {
            printf("KATEGORI PENUH!\n");
        }
        else
        {
            addCategory(categories, categoryCount);
        }
        addDevice(devices, deviceCount, categories, categoryCount);
        return;
    }
    else if (categoryChoice > i + 1)
    {
        printf("BUKAN OPSI!\n");
        system(PAUSE);
    }
    categoryChoice--;

    Device *currentDevice = &devices[*deviceCount];

    currentDevice->category = &categories[categoryChoice];
    categories[categoryChoice].devices[categories[categoryChoice].device_num] = currentDevice;
    categories[categoryChoice].device_num++;

    system(CLEAR);
    printf("--------------\nMenambahkan Perangkat ke Kategori %s\n-----------\n", categories[categoryChoice].name);

    printf("Daya Perangkat (WATT): ");
    scanf("%f", &(currentDevice->power));

    currentDevice->period_num = 0;
    addPeriod(currentDevice);

    do {
        system(CLEAR);
        printf("=Device Baru=\n");
        printf("----------\nKategori: %s\n----------\n", currentDevice->category->name);
        printDevice(currentDevice);
        printf("=================\n");
        printf("1. Tambah Periode\n");
        printf("2. Selesai\n");
        printf("Pilih Opsi: ");
        scanf("%u", &categoryChoice);

        if (categoryChoice == 1)
        {
            if (currentDevice->period_num >= PERIOD_MAX)
            {
                printf("PERIODE PENUH!\n");
            }
            else
            {
                addPeriod(currentDevice);
            }
        }
    } while (categoryChoice == 1);

    (*deviceCount)++;
}

void addPeriod(Device *device)
{
    system(CLEAR);
    printf("----------\nTambah Periode Penggunaan\n");
    printf("[Format Input JJ.MM]\n----------\n");
    do {
        printf("Waktu Mulai: ");
        scanf("%s", device->periods[device->period_num].begin);
    } while (!checkTimeFormat(device->periods[device->period_num].begin));

    do {
        printf("Waktu Selesai: ");
        scanf("%s", device->periods[device->period_num].end);
    } while (!checkTimeFormat(device->periods[device->period_num].end));

    device->activeMinutes += parsePeriod(&device->periods[device->period_num]);
    device->period_num++;
    updateConsumption(device);
    device->category->totalConsumption += device->energyDaily;
    device->category->totalWatt += device->power;
}

bool checkTimeFormat(char time[])
{
    if (time[2] != '.')
    {
        printf("FORMAT WAKTU SALAH!\n");
        return false;
    }
    uint i;
    for (i = 0; i < 4; ++i)
    {
        if ((time[i] < '0' || time[i] > '9') & time[i] != '.')
        {
            printf("FORMAT WAKTU SALAH!\n");
            return false;
        }
    }
    return true;
}

void addCategory(Category categories[], uint *categoryCount)
{
    system(CLEAR);
    printf("----------\nTambah Kategori Baru\n----------\n");
    printf("Nama Kategori: ");
    scanf("%s", categories[*categoryCount].name);
    categories[*categoryCount].id = *categoryCount;
    categories[*categoryCount].totalConsumption = 0;
    categories[*categoryCount].totalWatt = 0;
    categories[*categoryCount].device_num = 0;
    (*categoryCount)++;
}

void addCategoryManual(Category categories[], uint *categoryCount, char name[])
{
    strcpy(categories[*categoryCount].name, name);
    categories[*categoryCount].id = *categoryCount;
    categories[*categoryCount].totalConsumption = 0;
    categories[*categoryCount].device_num = 0;
    (*categoryCount)++;
}

uint parsePeriod(Period *period) // Mengubah periode menjadi menit
{
    // Parsing (10 * puluhan jam + satuan jam) * 60 + (puluhan menit) * 10 +
    // satuan menit
    if (strcmp(period->begin, period->end) == 0)
    {
        return 1440;
    }
    uint begin_minutes = (period->begin[0] * 10 + period->begin[1]) * 60 + (period->begin[3] * 10 + period->begin[4]);
    uint end_minutes = (period->end[0] * 10 + period->end[1]) * 60 + (period->end[3] * 10 + period->end[4]);

    return (end_minutes - begin_minutes + 1440) % 1440; // Operasi modulo, misalkan 17.00 - 16.00 berarti 23 jam.
                // Menit/hari: 1440
}

void updateConsumption(Device *device) 
{
    device->activeMinutes = 0; // Mulai dari nol

    uint i;
    for (i = 0; i < device->period_num; ++i) {
        device->activeMinutes += parsePeriod(&device->periods[i]);
    }
    device->energyDaily = device->activeMinutes * device->power / 6000; // kwh
}

void printAllDevices(Category *categories, uint *categoryCount)
{
    system(CLEAR);
    uint i;
    for (i = 0; i < *categoryCount; ++i)
    {
        if (categories[i].device_num == 0)
        {
            continue;
        }

        printf("===========\n");
        printf("Kategori: %s\n", categories[i].name);
        printf("------------------\n");
        uint j;
        for (j = 0; j < categories[i].device_num; ++j)
        {
            printf("Device %d:\n", j + 1);
            printDevice(categories[i].devices[j]);
        }
        printf("Total Konsumsi Harian: %.2f kwh\n", categories[i].totalConsumption);
        printf("========\n\n");
    }
    system(PAUSE);
}

void printLogo() 
{
    printf("  _      __     __  __ _      __     __      __       \n");
    printf(" | | /| / /__ _/ /_/ /| | /| / /__ _/ /_____/ /       \n");
    printf(" | |/ |/ / _ `/ __/ __/ |/ |/ / _ `/ __/ __/ _ \\      \n");
    printf(" |__/|__/\\__,_/\\__/\\__/|__/|__/\\__,___/\\__/\\_//_/     \n\a");
}

void animateLogo()
{
    uint i = 0;
    for (i = 0; i < 50; i++)
    {
        system(CLEAR);
        printLogo();
    }
}

void printCredits()
{
  printf("AZKA NABIHAN  HILMY (2306250541)\n");
  printf("MUHAMMAD ARYA WIANDRA UTOMO (2306218295)\n");
  system(PAUSE);
}

void help() 
{
    system(CLEAR);
    printf("\n\t|=====================================================================================|");
    printf("\n\t|------------------------------------------HELP---------------------------------------|");
    printf("\n\t|=====================================================================================|");
    printf("\n\t| 1. Tambahkan Device: Menambahkan perangkat baru ke dalam sistem.                    |");
    printf("\n\t| 2. Tampilkan Device: Menampilkan daftar perangkat yang sudah ditambahkan.           |");
    printf("\n\t| 3. Statistik: Melakukan analisis terhadap data perangkat.                           |");
    printf("\n\t| 4. Save data: Akan menyimpan data user ke dalam file .txt                           |");
    printf("\n\t| 5. Cari data : Akan mencari data dari kategori listrik                              |");
    printf("\n\t| 6. Help: Menampilkan menu bantuan.                                                  |");
    printf("\n\t| 7. Credits: Menampilkan Anggota Kelompok.                                           |");
    printf("\n\t| 8. Exit: Keluar dari program.                                                       |");
    printf("\n\t|=====================================================================================|\n");
    system("pause");
}

uint getMenu() {
    system(CLEAR);
    printLogo();
    printf("\n\n");
    printf("|=================================|\n");
    printf("|              MENU               |\n");
    printf("|=================================|\n");
    printf("| 1. Tambahkan Device             |\n");
    printf("| 2. Tampilkan Device             |\n");
    printf("| 3. Statistik                    |\n");
    printf("| 4. Save data                    |\n");
    printf("| 5. Cari Data                    |\n");
    printf("| 6. Help                         |\n");
    printf("| 7. Credits                      |\n");
    printf("| 8. Exit                         |\n");
    printf("|=================================|\n");

    uint menuInput;
    printf("\nMasukkan input (1 - 8) : ");
    scanf("%d", &menuInput);
    return menuInput;
}

#define HISTOGRAM_SCALE 1
void printHistogram(Category *categories[], uint categoryCount) {
    system(CLEAR);
    printf("=========HISTOGRAM=========\n");
    printf("Kategori Terurut Dari Konsumsi Daya Terbesar\n");
    int i, j;
    for (i = 0; i < categoryCount; i++) {
        printf("\n%d. %s : ", i + 1, categories[i]->name);
        uint histogramLength = categories[i]->totalConsumption * HISTOGRAM_SCALE;
        for (j = 0; j < histogramLength; j++) {
            printf("#");
        }
    }
    printf("\n-------------\n");
    printf("Kategori dengan konsumsi tertinggi adalah %s (id: %u) sebesar %.2f kwh\n", categories[0]->name, categories[0]->id, categories[0]->totalConsumption);
}

void swap(Category **a, Category **b)
{
    Category *temp = *a;
    *a = *b;
    *b = temp;
}

bool comp(Category *a, Category *b)
{
    return (a->totalConsumption) > (b->totalConsumption);
}

void bubbleSort(Category *array[], int length, bool (*comp)(Category*, Category*))
{
    int i, j;
    bool swapped;   
    for (i = 0; i < length - 1; i++)
    {
        swapped = false;
        for (j = 0; j < length - i - 1; j++)
        {
            if (!comp(array[j], array[j + 1]))
            {
                swap(&array[j], &array[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
        {
            break;
        }
    }
}

void showStats(Category *sort_container[], uint categoryCount)
{
    bubbleSort(sort_container, categoryCount, comp);
    printHistogram(sort_container, categoryCount);
    printTips(sort_container[0]->id);
    system(PAUSE);
}

void defaultCategories(Category categories[], uint *categoryCount)
{
    addCategoryManual(categories, categoryCount, "Kipas");
    addCategoryManual(categories, categoryCount, "AC");
    addCategoryManual(categories, categoryCount, "TV");
    addCategoryManual(categories, categoryCount, "Lampu");
}

void printTips(uint id)
{
    switch(id)
    {
        case 0:
            printf("Tips : 1. Matikan Kipas Saat Tidak Diperlukan: Matikan kipas saat Anda meninggalkan ruangan. Kipas tidak mendinginkan ruangan, melainkan membuat perasaan lebih nyaman saat berada di sana.\n");
            printf("       2. Pilih Kecepatan yang Sesuai: Pilih kecepatan kipas yang sesuai dengan tingkat kenyamanan Anda. Kecepatan rendah menggunakan lebih sedikit energi dibandingkan dengan kecepatan tinggi.\n");
            printf("       3. Manfaatkan Ventilasi Alami: Gunakan ventilasi alami seperti jendela terbuka untuk meningkatkan sirkulasi udara sebelum beralih ke kipas listrik.\n\n");
            break;
        case 1:
            printf("Tips :1. Gunakan Mode Otomatis (Auto): Atur AC Anda pada mode otomatis agar dapat menyesuaikan suhu ruangan secara otomatis. Ini membantu mencegah AC bekerja terlalu keras dan menghemat energi.\n");
            printf("      2. Atur Suhu yang Tepat: Jangan atur suhu AC terlalu rendah. Cobalah untuk menjaga suhu antara 24-26 derajat Celsius agar AC tidak perlu bekerja terlalu keras, mengurangi konsumsi energi.\n");
            printf("      3. Rutin Bersihkan Filter: Bersihkan atau ganti filter AC secara teratur. Filter yang bersih memungkinkan aliran udara yang lebih baik dan membuat AC lebih efisien.\n\n");
            break;
        case 2:
          printf("Tips : 1. Matikan TV : Matikan perangkat-perangkat ini sepenuhnya saat tidak digunakan. Mode standby pada TV masih mengonsumsi energi.\n");
          printf("       2. Atur Kecerahan TV: Kurangi kecerahan pada layar TV sesuai dengan kebutuhan. Semakin rendah kecerahan, semakin sedikit energi yang digunakan.\n\n");
          break;
        case 3:
          printf("Tips : 1. Matikan lamput jika tidak digunakan\n");
          printf("       2. Gunakan Lampu LED: Ganti lampu pijar dengan lampu LED yang lebih efisien energi. Lampu LED menghasilkan cahaya yang sama dengan watt yang lebih rendah.\n\n");
          break;
        default:
            printf("Tips : Matikan jika tidak digunakan\n\n");
            break;
    }
}
// Parkiran Kursor: [][][][]