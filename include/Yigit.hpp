/**
 * @file    Yigit.hpp
 * @brief   Konum nesnelerini saklayan, bagli liste tabanli yigit (stack).
 *          Ekleme ve cikarma islemleri yigitin tepesinde O(1) surede yapilir.
 */
#ifndef YIGIT_HPP
#define YIGIT_HPP

#include "Konum.hpp"

class Dugum {
public:
    Konum veri;
    Dugum* sonraki;

    Dugum(const Konum& veri, Dugum* sonraki);
};

class Yigit {
public:
    Yigit();
    ~Yigit();

    void ekle(const Konum& konum);   // push
    Konum cikar();                   // pop
    const Konum& tepe() const;       // top
    bool bosMu() const;
    int elemanSayisi() const;
    void temizle();

private:
    Dugum* tepeDugum;
    int sayac;
};

#endif
