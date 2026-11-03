/**
 * @file    Yigit.cpp
 * @brief   Bagli liste tabanli yigitin gerceklenmesi.
 */
#include "Yigit.hpp"

Dugum::Dugum(const Konum& veri, Dugum* sonraki)
    : veri(veri), sonraki(sonraki) {
}

Yigit::Yigit() : tepeDugum(0), sayac(0) {
}

Yigit::~Yigit() {
    temizle();
}

void Yigit::ekle(const Konum& konum) {
    tepeDugum = new Dugum(konum, tepeDugum);
    sayac++;
}

Konum Yigit::cikar() {
    // Bos yigittan cikarma cagiran taraf bosMu() ile kontrol etmelidir
    Dugum* silinecek = tepeDugum;
    Konum veri = silinecek->veri;
    tepeDugum = silinecek->sonraki;
    delete silinecek;
    sayac--;
    return veri;
}

const Konum& Yigit::tepe() const {
    return tepeDugum->veri;
}

bool Yigit::bosMu() const {
    return tepeDugum == 0;
}

int Yigit::elemanSayisi() const {
    return sayac;
}

void Yigit::temizle() {
    while (!bosMu())
        cikar();
}
