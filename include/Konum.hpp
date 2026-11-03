/**
 * @file    Konum.hpp
 * @brief   Labirent icindeki bir hucreyi (satir, sutun) ve o hucreye
 *          hangi yonden gelindigini tutan sinif.
 */
#ifndef KONUM_HPP
#define KONUM_HPP

typedef enum {
    ASAGI, SOL, YUKARI, SAG
} Yon;

#define YON_SAYISI 4

class Konum {
public:
    int satir;
    int sutun;
    Yon yon;    // Bu konuma hangi yonde ilerlenerek gelindi

    Konum(int satir = 0, int sutun = 0, Yon yon = ASAGI);

    // Verilen yonde bir adim ilerlendiginde ulasilan komsu konum
    Konum komsu(Yon y) const;

    bool operator==(const Konum& diger) const;
    bool operator!=(const Konum& diger) const;
};

// Verilen yonun tersini dondurur (ASAGI <-> YUKARI, SOL <-> SAG)
Yon tersYon(Yon y);

#endif
