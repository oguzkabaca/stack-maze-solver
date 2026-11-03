# Labirent Oyunu: Yığıt ile Geri İzleme (Backtracking)

Sakarya Uygulamalı Bilimler Üniversitesi, Bilgisayar Mühendisliği, **Veri Yapıları** dersi proje ödevi.
Ders sorumlusu: Doç. Dr. Selman HIZAL.

Bu program, `Harita.txt` dosyasından okunan bir labirentte başlangıçtan çıkışa giden yolu arar. Arama, kendi yazdığımız
**bağlı liste tabanlı yığıt (stack)** ile yapılan geri izlemeye (backtracking) dayanır ve konsolda yön oklarıyla
(▼ ◄ ▲ ►) adım adım gösterilir.

```
        ################### ##############################
        #                          #######################
        ...
        ###### #### ################ #####################
        ######                       #####################
        ######▼###########################################
CIKISA GELINDI
```

## Derleme ve çalıştırma

Windows ve MinGW (g++, mingw32-make) gerekir. Proje kök dizininde:

```
mingw32-make
```

Bu komut kaynak kodları `lib/` klasörüne derler, `bin/Labirent.exe` dosyasını oluşturur ve programı çalıştırır.
Yalnızca derlemek için `mingw32-make derle`, yalnızca çalıştırmak için `mingw32-make calistir` kullanılabilir.

| Komut | Açıklama |
|---|---|
| `bin\Labirent.exe` | `Harita.txt` dosyasını çözer |
| `bin\Labirent.exe haritalar\Harita2.txt 20` | Başka bir haritayı, adım başına 20 ms bekleyerek çözer |
| `bin\Labirent.exe --uret` | Rastgele bir labirent üretir (`RastgeleHarita.txt`) ve çözer |
| `bin\Labirent.exe --uret 2026` | Verilen tohumla her seferinde aynı labirenti üretir |

## Klasör yapısı

```
├── bin/            Labirent.exe (derleme çıktısı)
├── include/        Konum.hpp, Yigit.hpp, Labirent.hpp, LabirentUretici.hpp
├── lib/            .o dosyaları (derleme çıktısı)
├── src/            Konum.cpp, Yigit.cpp, Labirent.cpp, LabirentUretici.cpp, main.cpp
├── haritalar/      Ek test haritaları
├── prototip/       mazeGen.cpp (bit düzeyi labirent modeli prototipi)
├── Harita.txt      Varsayılan harita (20 x 50)
└── makefile
```

## Nasıl çalışır?

- **Harita:** `#` duvarı, boşluk geçilebilir yolu gösterir. Başlangıç ve çıkış, haritanın dış kenarındaki açıklıklardan
  otomatik bulunur. Bu yüzden farklı boyuttaki haritalar da ek ayar yapmadan çözülür.
- **Çözüm:** Başlangıç hücresi yığıta eklenir. Her adımda tepedeki hücrenin ziyaret edilmemiş bir komşusuna, yönler
  `ASAGI, SOL, YUKARI, SAG` sırasıyla denenerek ilerlenir (push). Gidilecek komşu yoksa yığıttan çıkarılıp bir önceki
  hücreye geri dönülür (pop). Çıkışa ulaşıldığında yığıtta kalan hücreler bulunan yoldur. Zaman ve bellek karmaşıklığı O(M·N).
- **Labirent üretimi (`--uret`):** `prototip/mazeGen.cpp` dosyasındaki bit düzeyi modelden uyarlanmıştır. Her hücre 4 bitlik
  bir duvar maskesidir (`KUZEY=1000, DOGU=0100, GUNEY=0010, BATI=0001`). Labirent yine yığıt ile geri izleme
  ("recursive backtracker") yapılarak kazılır. Ortaya çıkan labirentte her iki hücre arasında tek bir yol vardır,
  bu yüzden çıkış her zaman bulunur.

## Test haritaları

| Harita | Sonuç |
|---|---|
| `Harita.txt` | CIKISA GELINDI (115 hücrelik yol) |
| `haritalar/Harita2.txt` | CIKISA GELINDI (39 hücrelik yol) |
| `haritalar/CikisiKapali.txt` | CIKIS BULUNAMADI |
