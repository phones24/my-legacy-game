# My DOS Game

## Introduction

When I was in school (around 1994), I got this book called "Tricks of the Game-Programming Gurus".
I was so obsessed with the idea of making a game, I was sitting behind the computer all day trying to figure things out.
Back in the days I only knew Pascal, but in the book they used C and Assembly. So, it was even more challenging for me.
Moreover I didn't have the source code that was supposed to come with the book. Anyways I had best fun time back there
and in the end I made a very simple game which pauses when playing any sound (I didn't know how to programm the sound driver).

But I learned C and Assembly while making a game which allowed me move to something more complex.
I adopted Watcom C, protected mode, learned some demoscene stuff, etc...
Unfortunately I never really got to the point of making a real game after that.

## Now

30 years later I decided to fullfill my dream of making a game and continued where I left off.
So here we are - a simple DOS vertical shooter, now with enemies, sounds and even music.
Still not a finished game, though.

## Implementation details

The game uses Watcom C, DOS Protected Mode, and the Midas Sound System. I cross-compiled the game under linux, so no DOS makefiles.

Also used:
- GrafX2 to prepare the graphics.
- MilkyTracker to create the music.

## Compile

You will need:
- Open Watcom C 2.0
- Housemarque Audio System (http://www.s2.org/hmqaudio/)

To compile ad run:
```
wmake clean && wmake && dosbox game.exe
```

