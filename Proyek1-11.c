
/*Program Menghitung Konsumsi Alat Elektronik
/*
Tanggal : 18/03/2024

no. grup : 11
Nama anggota :
1. Azka Nabihan (2306250541)
2. Muhammad Arya Wiandra (2306218295)

Versi : 1.9
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include "header.h"


#define DEVICE_MAX 30
#define CATEGORY_MAX 20
#define PERIOD_MAX 5

int main(void) 
{
    char nama[50];
    int umur;

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

                searchingMenu();
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


