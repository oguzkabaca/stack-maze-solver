/**
 * @file    LabirentUretici.hpp
 * @brief   Rastgele labirent ureten sinif (yigit ile geri izlemeli DFS,
 *          "recursive backtracker"). Hucreler bit duzeyinde modellenir:
 *          her hucre 4 bitlik bir duvar maskesidir (K, D, G, B).
 *          Model ve yardimci fonksiyonlar mazeGen.cpp calismasindan uyarlanmistir.
 */
#ifndef LABIRENT_URETICI_HPP
#define LABIRENT_URETICI_HPP

#include <cstdint>
#include <random>
#include "Labirent.hpp"

// Duvar bitleri: 1 = duvar var, 0 = gecit acik
#define KUZEY 0b1000
#define DOGU  0b0100
#define GUNEY 0b0010
#define BATI  0b0001

// Karakter haritasi (2*satir+1) x (2*sutun+1) oldugundan hucre sayilari
// harita sinirlarina sigacak sekilde secilir: 20x50 icin 9x24 hucre.
#define HUCRE_SATIR ((YUKSEKLIK - 1) / 2)
#define HUCRE_SUTUN ((GENISLIK - 1) / 2)

class LabirentUretici {
public:
    explicit LabirentUretici(unsigned int tohum);

    void uret();
    bool dosyayaYaz(const char* dosyaYolu) const;

private:
    struct Komsu {
        int satir;
        int sutun;
        uint8_t duvar;     // Mevcut hucreden komsuya gecerken kaldirilacak duvar
    };

    uint8_t hucre[HUCRE_SATIR][HUCRE_SUTUN];
    bool ziyaret[HUCRE_SATIR][HUCRE_SUTUN];
    std::mt19937 uretec;

    static uint8_t tersDuvar(uint8_t duvar);
    static bool duvarKaldir(uint8_t& mevcutHucre, uint8_t& komsuHucre, uint8_t duvar);
    static bool icindeMi(int satir, int sutun);

    int ziyaretsizKomsular(int satir, int sutun, Komsu komsular[4]) const;
    int rastgele(int enAz, int enCok);
    Konum rastgeleKenarHucresi();
};

#endif
