/**
 * @file    Konum.cpp
 * @brief   Konum sinifinin gerceklenmesi.
 */
#include "Konum.hpp"

// Yon enum sirasina gore satir/sutun degisimleri: ASAGI, SOL, YUKARI, SAG
static const int dSatir[YON_SAYISI] = { 1,  0, -1, 0 };
static const int dSutun[YON_SAYISI] = { 0, -1,  0, 1 };

Konum::Konum(int satir, int sutun, Yon yon)
    : satir(satir), sutun(sutun), yon(yon) {
}

Konum Konum::komsu(Yon y) const {
    return Konum(satir + dSatir[y], sutun + dSutun[y], y);
}

bool Konum::operator==(const Konum& diger) const {
    return satir == diger.satir && sutun == diger.sutun;
}

bool Konum::operator!=(const Konum& diger) const {
    return !(*this == diger);
}

Yon tersYon(Yon y) {
    return (Yon)((y + 2) % YON_SAYISI);
}
