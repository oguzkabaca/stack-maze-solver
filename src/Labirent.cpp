/**
 * @file    Labirent.cpp
 * @brief   Labirent sinifinin gerceklenmesi: harita okuma, kapi (baslangic /
 *          cikis) bulma, yigit ile geri izleme ve konsolda animasyonlu cizim.
 */
#include <windows.h>
#include <cstdio>
#include <cstring>
#include "Labirent.hpp"
#include "Yigit.hpp"

static char YonChr[] = { 31, 17, 30, 16 };   // ASAGI, SOLA, YUKARI, SAG

#define SOL_BOSLUK 8    // Haritanin konsolda soldan ne kadar iceride cizilecegi

// Metni konsolun (x, y) noktasina yazar. 437 kod sayfasi glif donusumu
// (MB_USEGLYPHCHARS) ile 16, 17, 30, 31 kodlari dolu ok olarak gorunur.
static void konsolaYaz(int x, int y, const char* metin, int uzunluk) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    wchar_t genis[256];
    int n = MultiByteToWideChar(437, MB_USEGLYPHCHARS, metin, uzunluk, genis, 256);
    COORD nokta = { (SHORT)x, (SHORT)y };
    DWORD yazilan;
    SetConsoleCursorPosition(h, nokta);
    WriteConsoleW(h, genis, n, &yazilan, 0);
}

static void imleciGoster(bool goster) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO bilgi;
    GetConsoleCursorInfo(h, &bilgi);
    bilgi.bVisible = goster;
    SetConsoleCursorInfo(h, &bilgi);
}

Labirent::Labirent(const char* dosyaYolu)
    : satirSayisi(0), sutunSayisi(0), yuklendi(false) {
    DWORD mod;
    konsolVar = GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mod) != 0;
    yuklendi = dosyadanOku(dosyaYolu) && kapilariBul();
}

bool Labirent::yuklendiMi() const {
    return yuklendi;
}

const Konum& Labirent::getBaslangic() const {
    return baslangic;
}

const Konum& Labirent::getCikis() const {
    return cikis;
}

bool Labirent::dosyadanOku(const char* dosyaYolu) {
    FILE* fp = fopen(dosyaYolu, "r");
    if (fp == 0)
        return false;

    memset(harita, ' ', sizeof(harita));
    char satir[256];
    while (satirSayisi < YUKSEKLIK && fgets(satir, sizeof(satir), fp) != 0) {
        int uzunluk = (int)strcspn(satir, "\r\n");
        if (uzunluk > GENISLIK)
            uzunluk = GENISLIK;
        // Kisa satirlarin (sondaki bosluklari silinmis) eksik kismi bosluk kalir
        memcpy(harita[satirSayisi], satir, uzunluk);
        if (uzunluk > sutunSayisi)
            sutunSayisi = uzunluk;
        satirSayisi++;
    }
    fclose(fp);

    // Dosyanin sonundaki bos satirlar haritaya dahil edilmez
    while (satirSayisi > 0) {
        bool bos = true;
        for (int j = 0; j < sutunSayisi && bos; j++)
            if (harita[satirSayisi - 1][j] != ' ')
                bos = false;
        if (!bos)
            break;
        satirSayisi--;
    }
    return satirSayisi >= 2 && sutunSayisi >= 2;
}

// Haritanin kenarlarindaki bosluklar labirentin kapilaridir. Ust kenardan
// baslayip saat yonunun tersine taranir; bulunan ilk kapi baslangic, ikincisi
// cikis kabul edilir. Baslangic yonu labirentin icine dogrudur.
bool Labirent::kapilariBul() {
    Konum kapilar[2 * (YUKSEKLIK + GENISLIK)];
    int n = 0;
    int sonSatir = satirSayisi - 1;
    int sonSutun = sutunSayisi - 1;

    for (int j = 0; j <= sonSutun; j++)
        if (harita[0][j] == ' ')
            kapilar[n++] = Konum(0, j, ASAGI);
    for (int i = 1; i < sonSatir; i++)
        if (harita[i][0] == ' ')
            kapilar[n++] = Konum(i, 0, SAG);
    for (int i = 1; i < sonSatir; i++)
        if (harita[i][sonSutun] == ' ')
            kapilar[n++] = Konum(i, sonSutun, SOL);
    for (int j = 0; j <= sonSutun; j++)
        if (harita[sonSatir][j] == ' ')
            kapilar[n++] = Konum(sonSatir, j, YUKARI);

    if (n < 2)
        return false;
    baslangic = kapilar[0];
    cikis = kapilar[1];
    return true;
}

bool Labirent::gecilebilirMi(const Konum& k) const {
    return k.satir >= 0 && k.satir < satirSayisi &&
           k.sutun >= 0 && k.sutun < sutunSayisi &&
           harita[k.satir][k.sutun] == ' ';
}

void Labirent::haritayiCiz() const {
    if (!konsolVar)
        return;
    system("color 1F");
    system("cls");
    imleciGoster(false);
    for (int i = 0; i < satirSayisi; i++)
        konsolaYaz(SOL_BOSLUK, i, harita[i], sutunSayisi);
}

void Labirent::hucreCiz(const Konum& k, char c) const {
    if (konsolVar)
        konsolaYaz(SOL_BOSLUK + k.sutun, k.satir, &c, 1);
}

void Labirent::mesajYaz(const char* mesaj) const {
    konsolaYaz(0, satirSayisi, mesaj, (int)strlen(mesaj));
    konsolaYaz(0, satirSayisi + 1, "", 0);
    imleciGoster(true);
}

// Konsol yoksa (cikti dosyaya yonlendirildiyse) bulunan yol '.' ile isaretlenip
// harita bir kez yazdirilir. Yigitta kalan konumlar baslangictan cikisa giden yoldur.
void Labirent::sonucuYazdir(Yigit& yol, const char* mesaj, int adimSayisi) const {
    int yolUzunlugu = yol.elemanSayisi();
    if (konsolVar) {
        mesajYaz(mesaj);
    } else {
        char cizim[YUKSEKLIK][GENISLIK];
        memcpy(cizim, harita, sizeof(cizim));
        while (!yol.bosMu()) {
            Konum k = yol.cikar();
            cizim[k.satir][k.sutun] = '.';
        }
        for (int i = 0; i < satirSayisi; i++)
            printf("%.*s\n", sutunSayisi, cizim[i]);
        printf("%s\n", mesaj);
    }
    printf("Baslangic: Konum(%d, %d)  Cikis: Konum(%d, %d)\n",
           baslangic.satir, baslangic.sutun, cikis.satir, cikis.sutun);
    printf("Yol uzunlugu: %d hucre  Toplam hamle: %d\n", yolUzunlugu, adimSayisi);
}

bool Labirent::coz(int beklemeMs) {
    memset(ziyaret, 0, sizeof(ziyaret));
    haritayiCiz();

    Yigit yigit;
    ziyaret[baslangic.satir][baslangic.sutun] = true;
    yigit.ekle(baslangic);
    hucreCiz(baslangic, YonChr[baslangic.yon]);
    int adimSayisi = 0;

    while (!yigit.bosMu()) {
        if (konsolVar)
            Sleep(beklemeMs);

        Konum mevcut = yigit.tepe();
        if (mevcut == cikis) {
            sonucuYazdir(yigit, "CIKISA GELINDI", adimSayisi);
            return true;
        }

        // Yonler enum sirasiyla denenir; ilk uygun ve ziyaret edilmemis komsuya gecilir
        bool ilerlendi = false;
        for (int y = 0; y < YON_SAYISI && !ilerlendi; y++) {
            Konum komsu = mevcut.komsu((Yon)y);
            if (gecilebilirMi(komsu) && !ziyaret[komsu.satir][komsu.sutun]) {
                ziyaret[komsu.satir][komsu.sutun] = true;
                yigit.ekle(komsu);
                hucreCiz(mevcut, ' ');
                hucreCiz(komsu, YonChr[komsu.yon]);
                ilerlendi = true;
            }
        }

        // Cikmaz: tepedeki konum atilir ve bir onceki konuma geri donulur.
        // Geri donerken ok, gelinen yonun tersini gosterir.
        if (!ilerlendi) {
            Konum geriAlinan = yigit.cikar();
            hucreCiz(geriAlinan, ' ');
            if (!yigit.bosMu())
                hucreCiz(yigit.tepe(), YonChr[tersYon(geriAlinan.yon)]);
        }
        adimSayisi++;
    }

    sonucuYazdir(yigit, "CIKIS BULUNAMADI", adimSayisi);
    return false;
}
