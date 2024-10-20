WATCOM = /opt/watcom

CC = wcl386
OBJDIR = obj
MIDASDIR = ./hmqaudio
MIDASINCDIR = $(MIDASDIR)/include
MIDASLIB = $(MIDASDIR)/lib/dos/wcretail/midas.lib

CFLAGS = -i$(%WATCOM)/h -i$(MIDASINCDIR) -d1 -fp3 -zastd=c99 -zp4 -5r -mf -ox -lm -l=dos32a -bt=dos -fo=$(OBJDIR)/.o

SOURCES = &
    game.c &
    hud.c &
    level_events.c &
    collision.c &
    keyboard.c &
    event_enemy1.c &
	event_enemy2.c &
    ship.c &
    sprite.c &
    all_sprites.c &
    timer.c &
    res.c &
    level.c &
    graphics.c &
    font.c &
    sound.c &
	event_rock.c &
	titlescr.c

EXE = game.exe

all: $(OBJDIR) $(EXE)

$(OBJDIR): .SYMBOLIC
    -mkdir -p $(OBJDIR)

$(EXE): $(SOURCES)
    $(CC) $(CFLAGS) $(MIDASLIB) -fe=$@ $<

clean: .SYMBOLIC
    -rm -f $(OBJDIR)/*.o
    -rm -f $(EXE)
    -rm -f *.err
