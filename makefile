WATCOM = /opt/watcom
WATCOM_BIN = $(WATCOM)/binl64
WCL = $(WATCOM_BIN)/wcl386

all : game.exe

game.exe : game.c
	$(WCL) -i$(WATCOM)/h -d1 -fp3 -zastd=c99 -zp4 -5r -mf -ox -lm -l=dos32a -bt=dos game.c keyboard.c event_enemy1.c ship.c sprite.c all_sprites.c timer.c res.c level.c graphics.c font.c

clean :
	rm -f keyboard.o sprite.o all_sprites.o timer.o res.o game.o graphics.o font.o level.o ship.o event_enemy1.o game.exe
