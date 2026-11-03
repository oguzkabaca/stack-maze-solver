/**
 * @file    LabirentUretici.cpp
 * @brief   Bit duzeyi hucre modeli ile rastgele labirent uretimi.
 *
 * Algoritma: Kenardan rastgele bir hucre secilip yigita eklenir. Tepedeki
 * hucrenin ziyaret edilmemis komsularindan biri rastgele secilir, aradaki
 * duvar her iki hucrede de kaldirilir ve komsu yigita eklenir. Ziyaret
 * edilmemis komsu kalmayan hucre yigittan cikarilir (geri izleme). Yigit
 * bosaldiginda her hucreye tek bir yoldan ulasilabilen "kusursuz" bir
 * labirent elde edilir.
 */
#include <cstdio>
#include "LabirentUretici.hpp"
#include "Yigit.hpp"

LabirentUretici::LabirentUretici(unsigned int tohum) : uretec(tohum) {
}

uint8_t LabirentUretici::tersDuvar(uint8_t duvar) {
    switch (duvar) {
        case KUZEY: return GUNEY;
        case GUNEY: return KUZEY;
        case DOGU:  return BATI;
        case BATI:  return DOGU;
        default:    return 0;
    }
}

// Iki komsu hucre arasindaki duvar iki tarafta da tutuldugu icin
// (mevcut hucrenin duvari ve komsunun ters duvari) birlikte kaldirilir.
bool LabirentUretici::duvarKaldir(uint8_t& mevcutHucre, uint8_t& komsuHucre, uint8_t duvar) {
    uint8_t ters = tersDuvar(duvar);
    bool duvarA = mevcutHucre & duvar;
    bool duvarB = komsuHucre & ters;

    if (duvarA && duvarB) {
        mevcutHucre &= ~duvar;
        komsuHucre &= ~ters;
        return true;
    }
    return false;
}

bool LabirentUretici::icindeMi(int satir, int sutun) {
    return satir >= 0 && satir < HUCRE_SATIR &&
           sutun >= 0 && sutun < HUCRE_SUTUN;
}

int LabirentUretici::ziyaretsizKomsular(int satir, int sutun, Komsu komsular[4]) const {
    static const int dSatir[4] = { -1, 0, 1, 0 };
    static const int dSutun[4] = { 0, 1, 0, -1 };
    static const uint8_t duvarlar[4] = { KUZEY, DOGU, GUNEY, BATI };

    int n = 0;
    for (int i = 0; i < 4; i++) {
        int yeniSatir = satir + dSatir[i];
        int yeniSutun = sutun + dSutun[i];
        if (icindeMi(yeniSatir, yeniSutun) && !ziyaret[yeniSatir][yeniSutun]) {
            komsular[n].satir = yeniSatir;
            komsular[n].sutun = yeniSutun;
            komsular[n].duvar = duvarlar[i];
            n++;
        }
    }
    return n;
}

int LabirentUretici::rastgele(int enAz, int enCok) {
    std::uniform_int_distribution<int> dagilim(enAz, enCok);
    return dagilim(uretec);
}

// Kenardaki 2*(satir+sutun)-4 hucreden biri esit olasilikla secilir:
// ust kenar, sag kenar, alt kenar, sol kenar sirasiyla numaralanir.
Konum LabirentUretici::rastgeleKenarHucresi() {
    int indeks = rastgele(0, 2 * HUCRE_SATIR + 2 * HUCRE_SUTUN - 5);

    if (indeks < HUCRE_SUTUN)
        return Konum(0, indeks);
    indeks -= HUCRE_SUTUN;

    if (indeks < HUCRE_SATIR - 2)
        return Konum(indeks + 1, HUCRE_SUTUN - 1);
    indeks -= HUCRE_SATIR - 2;

    if (indeks < HUCRE_SUTUN)
        return Konum(HUCRE_SATIR - 1, HUCRE_SUTUN - 1 - indeks);
    indeks -= HUCRE_SUTUN;

    return Konum(HUCRE_SATIR - 2 - indeks, 0);
}

void LabirentUretici::uret() {
    for (int i = 0; i < HUCRE_SATIR; i++)
        for (int j = 0; j < HUCRE_SUTUN; j++) {
            hucre[i][j] = KUZEY | DOGU | GUNEY | BATI;
            ziyaret[i][j] = false;
        }

    Yigit yigit;
    Konum baslangic = rastgeleKenarHucresi();
    ziyaret[baslangic.satir][baslangic.sutun] = true;
    yigit.ekle(baslangic);

    while (!yigit.bosMu()) {
        Konum mevcut = yigit.tepe();
        Komsu komsular[4];
        int n = ziyaretsizKomsular(mevcut.satir, mevcut.sutun, komsular);

        if (n == 0) {
            yigit.cikar();
            continue;
        }

        Komsu secilen = komsular[rastgele(0, n - 1)];
        duvarKaldir(hucre[mevcut.satir][mevcut.sutun],
                    hucre[secilen.satir][secilen.sutun], secilen.duvar);
        ziyaret[secilen.satir][secilen.sutun] = true;
        yigit.ekle(Konum(secilen.satir, secilen.sutun));
    }

    // Giris ust kenarda, cikis alt kenarda: dis duvarda birer kapi acilir
    hucre[0][rastgele(0, HUCRE_SUTUN - 1)] &= ~KUZEY;
    hucre[HUCRE_SATIR - 1][rastgele(0, HUCRE_SUTUN - 1)] &= ~GUNEY;
}

// Hucre (i, j) karakter haritasinda (2i+1, 2j+1) noktasina dusar; aradaki
// duvarlar tek karakterlik '#' olarak yazilir.
bool LabirentUretici::dosyayaYaz(const char* dosyaYolu) const {
    const int satirSayisi = 2 * HUCRE_SATIR + 1;
    const int sutunSayisi = 2 * HUCRE_SUTUN + 1;
    char harita[2 * HUCRE_SATIR + 1][2 * HUCRE_SUTUN + 1];

    for (int i = 0; i < satirSayisi; i++)
        for (int j = 0; j < sutunSayisi; j++)
            harita[i][j] = '#';

    for (int i = 0; i < HUCRE_SATIR; i++)
        for (int j = 0; j < HUCRE_SUTUN; j++) {
            int s = 2 * i + 1, k = 2 * j + 1;
            harita[s][k] = ' ';
            if (!(hucre[i][j] & KUZEY)) harita[s - 1][k] = ' ';
            if (!(hucre[i][j] & GUNEY)) harita[s + 1][k] = ' ';
            if (!(hucre[i][j] & BATI))  harita[s][k - 1] = ' ';
            if (!(hucre[i][j] & DOGU))  harita[s][k + 1] = ' ';
        }

    FILE* fp = fopen(dosyaYolu, "w");
    if (fp == 0)
        return false;
    for (int i = 0; i < satirSayisi; i++)
        fprintf(fp, "%.*s\n", sutunSayisi, harita[i]);
    fclose(fp);
    return true;
}
