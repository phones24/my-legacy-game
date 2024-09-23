WATCOM = /opt/watcom
WATCOM_BIN = $(WATCOM)/binl64
WCL = $(WATCOM_BIN)/wcl386

all : game.exe

game.exe : game.c
	$(WCL) -i$(WATCOM)/h -za99 -zp4 -mf -ox -lm -l=dos32a -bt=dos game.c graphics.c font.c

clean :
	rm -f game.o graphics.o font.o game.exe
