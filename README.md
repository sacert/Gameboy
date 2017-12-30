# Gameboy Emulator
 
A Gameboy emulator created to better understand computer architecture, design, as well as get more experience with low level programming. Only tested for Tetris. At this point I am content with how far I've reached on the emulator and may finish it up as well as test other games in the future.

![Alt text](tetris.gif?raw=true "Tetris")

**Tested on OS X but should be cross platform**

## Dependencies
SDL2

# Getting Start
``` sh
$ git clone https://github.com/sacert/Gameboy.git
$ cd Gameboy
$ make
$ ./gameboy [rom file]
```

## What isn't implemented
- Sound
- Memory mapping (switching between internal ram)

>**NOTE:** Only tested to ensure Tetris is working, other games would require more debugging
