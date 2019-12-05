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
#argv1 pocet zamestnancu
#argv2 pocet soubezne zkoumanych podnetu pro jednoho zamestnance
#argv3 pocet soubezne zpracovavanych 2 instancnich rizeni 1 zamestnancem
#argv4 pocet rizeni zpracovavanych najednou jednim zamestnancem
#argv5 doba behu simulace(roky), pouze celociselne
#argv6 nazev vystupniho souboru(bude dodana pripona .out)
run:
	./BrnoMHD d d d d d experiment1		#testujeme defaultni hodnoty, chovani systemu
	
clean:
	rm -f BrnoMHD *.tar.gz *.zip *.out

tarball:
	tar -zcf $(AUTHOR).tar.gz $(FILES)

tar:
	tar -cf $(AUTHOR).tar $(FILES)

zip:
	zip 07_$(AUTHOR).zip $(FILES)