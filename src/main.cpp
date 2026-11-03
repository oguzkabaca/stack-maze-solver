/**
 * @file    main.cpp
 * @brief   Labirent (Maze) Oyunu: Yigit ile geri izleme (backtracking) uygulamasi.
 *
 * Kullanim:
 *   Labirent.exe [harita dosyasi] [adim bekleme suresi (ms)]
 *       Varsayilan olarak Harita.txt okunur ve cozulur.
 *   Labirent.exe --uret [tohum] [adim bekleme suresi (ms)]
 *       Rastgele bir labirent uretip RastgeleHarita.txt dosyasina yazar ve cozer.
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include "Labirent.hpp"
#include "LabirentUretici.hpp"

#define VARSAYILAN_BEKLEME_MS 40
#define URETILEN_HARITA "RastgeleHarita.txt"

int main(int argc, char* argv[]) {
    const char* dosyaYolu = argc > 1 ? argv[1] : "Harita.txt";
    int beklemeMs = argc > 2 ? atoi(argv[2]) : VARSAYILAN_BEKLEME_MS;
    bool uretildi = false;
    unsigned int tohum = 0;

    if (argc > 1 && strcmp(argv[1], "--uret") == 0) {
        tohum = argc > 2 ? (unsigned int)strtoul(argv[2], 0, 10) : std::random_device()();
        beklemeMs = argc > 3 ? atoi(argv[3]) : VARSAYILAN_BEKLEME_MS;

        LabirentUretici uretici(tohum);
        uretici.uret();
        if (!uretici.dosyayaYaz(URETILEN_HARITA)) {
            printf("Uretilen harita yazilamadi: %s\n", URETILEN_HARITA);
            return 1;
        }
        dosyaYolu = URETILEN_HARITA;
        uretildi = true;
    }

    Labirent labirent(dosyaYolu);
    if (!labirent.yuklendiMi()) {
        printf("Harita okunamadi veya kenarlarda en az iki kapi yok: %s\n", dosyaYolu);
        return 1;
    }

    labirent.coz(beklemeMs);
    if (uretildi)
        printf("Rastgele labirent tohumu: %u (%s)\n", tohum, URETILEN_HARITA);
    return 0;
}
