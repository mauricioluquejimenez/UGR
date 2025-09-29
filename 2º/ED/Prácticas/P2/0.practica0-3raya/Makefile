
all: raya raya3.ps

juego.o: juego.cpp juego.h tablero.h jugador.h
	g++ -Wall -c juego.cpp

tablero.o: tablero.cpp tablero.h ficha.h
	g++ -Wall -c tablero.cpp

ficha.o: ficha.cpp ficha.h
	g++ -Wall -c ficha.cpp

jugador.o: jugador.cpp jugador.h ficha.h
	g++ -Wall -c jugador.cpp

main.o: main.cpp tablero.h ficha.h jugador.h juego.h
	g++ -Wall -c main.cpp

raya: main.o jugador.o ficha.o tablero.o juego.o
	g++ -o raya main.o jugador.o ficha.o tablero.o juego.o

raya3.ps: main.cpp jugador.cpp ficha.cpp tablero.cpp juego.cpp \
         jugador.h ficha.h tablero.h juego.h Makefile
	a2ps -A fill ficha.h ficha.cpp tablero.h tablero.cpp jugador.h \
                     jugador.cpp juego.h juego.cpp main.cpp \
                     Makefile -o raya3.ps

clean :
	-rm main.o ficha.o jugador.o tablero.o juego.o

mrproper : clean
	-rm raya raya3.ps
