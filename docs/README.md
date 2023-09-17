# How to use

To be able to run, this game engine requires the presence of a `game.bof` file in the current working directory. This file is a binary file that contains all the game data. The format of this file is described in the `binary_format.md` file.

## Init process

When the game engine is ready for your code, it will load the binary and run the code at the entry point until it finds a return instruction. This is the init process.

You can also register an address to be called in the game loop (every frame) TODO!

## Game loop

The game loop is a loop that runs every frame. Your code must end with a return instruction to hand control back to the game engine.
