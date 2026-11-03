hepsi: derle calistir

derle:
	g++ -I ./include/ -o ./lib/Konum.o -c ./src/Konum.cpp
	g++ -I ./include/ -o ./lib/Yigit.o -c ./src/Yigit.cpp
	g++ -I ./include/ -o ./lib/Labirent.o -c ./src/Labirent.cpp
	g++ -I ./include/ -o ./lib/LabirentUretici.o -c ./src/LabirentUretici.cpp
	g++ -I ./include/ -o ./bin/Labirent ./lib/Konum.o ./lib/Yigit.o ./lib/Labirent.o ./lib/LabirentUretici.o ./src/main.cpp

calistir:
	./bin/Labirent.exe
