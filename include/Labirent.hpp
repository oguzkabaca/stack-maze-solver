/**
 * @file    Labirent.hpp
 * @brief   Harita dosyasini okuyan, baslangic/cikis noktalarini bulan ve
 *          yigit ile geri izleme (backtracking) yaparak cikisi arayan sinif.
 */
#ifndef LABIRENT_HPP
#define LABIRENT_HPP

#include "Konum.hpp"
#include "Yigit.hpp"

#define YUKSEKLIK 20
#define GENISLIK 50

class Labirent {
public:
    explicit Labirent(const char* dosyaYolu);

    bool yuklendiMi() const;
    const Konum& getBaslangic() const;
    const Konum& getCikis() const;

    // Cikisi arar; bulunursa true doner. Her adim arasinda beklemeMs kadar bekler.
    bool coz(int beklemeMs);

private:
    char harita[YUKSEKLIK][GENISLIK];
    bool ziyaret[YUKSEKLIK][GENISLIK];
    int satirSayisi;
    int sutunSayisi;
    Konum baslangic;
    Konum cikis;
    bool yuklendi;
    bool konsolVar;     // Cikti gercek bir konsola mi gidiyor

    bool dosyadanOku(const char* dosyaYolu);
    bool kapilariBul();
    bool kenardaMi(int satir, int sutun) const;
    bool gecilebilirMi(const Konum& k) const;

    void haritayiCiz() const;
    void hucreCiz(const Konum& k, char c) const;
    void mesajYaz(const char* mesaj) const;
    void sonucuYazdir(Yigit& yol, const char* mesaj, int adimSayisi) const;
};

#endif
