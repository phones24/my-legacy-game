# My DOS Game

## Introduction

When I was in school (around 1994), I got a book called "Tricks of the Game-Programming Gurus".
I was so obsessed with the idea of making a game that I spent days behind the computer trying to figure things out.
Back then, I only knew Pascal, but in the book they used C and Assembly making it more challenging for me.
Moreover, I didn't have the source code that was supposed to come with the book. Nevertheless, I had best time back then,
and in the end, I made a very simple game that would pause when playing any sound (I didn't know how to program the sound driver).

Through the process, I learned C and Assembly, which allowed me to move on to more complex projects.
I adopted Watcom C, protected mode, learned some demoscene stuff, etc.
Unfortunately, I never really got to the point of making a real game after that.

## Now

Thirty years later, I decided to fulfill my dream of making a game and continued from where I left off.
So here we are - a simple DOS vertical shooter, now with enemies, sounds, and even music.
Still not a finished game, though.

## Implementation details

The game uses Watcom C, DOS Protected Mode, and the Midas Sound System.
I cross-compiled the game under Linux, so there are no DOS makefiles.

Also used:
- GrafX2 to prepare the graphics.
- MilkyTracker to create the music.

## Compile

You will need:
- Open Watcom C 2.0
- Housemarque Audio System (http://www.s2.org/hmqaudio/)

To compile and run:
```
wmake clean && wmake && dosbox game.exe
```

