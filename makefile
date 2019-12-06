CLAGS=-g -O2 
PFLAGS=-lsimlib -lm
 
CC = g++
AUTHOR = xmatya11_xtetur01
FILES = BrnoMHD.cpp Makefile dokumentace.pdf

all: BrnoMHD

BrnoMHD: BrnoMHD.cpp
	$(CC) $(CFLAGS) -o $@ BrnoMHD.cpp $(PFLAGS)

rebuild: clean all

#argumenty programu
#arg 1 - Kolik procent lidi ignoruje pocasi, pokud je vetsi jak 100, tak jsou pouze auta
#arg 2 - Kolik dni simulujeme
#arg 3 - Kolik lidi simulujeme x100
#arg 4 - Koeficient zaplneni MHD
run:
	./BrnoMHD 57 365 3800 20 >realna.out
	./BrnoMHD 0 365 3800 20 >experiment1.out
	./BrnoMHD 101 365 3800 20 >experiment2.out
	./BrnoMHD 57 365 3800 100 >experiment3.out
	./BrnoMHD 0 365 3800 100 >experiment4.out
	
clean:
	rm -f BrnoMHD *.zip *.out

zip:
	zip 01_$(AUTHOR).zip $(FILES)
