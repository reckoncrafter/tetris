# Reckoncrafter's Tetris
![](https://upload.wikimedia.org/wikipedia/commons/thumb/5/50/All_5_free_tetrominoes.svg/1200px-All_5_free_tetrominoes.svg.png)
An implementation of Tetris that runs in the terminal, written for practice in C++.

## TODO
- [x] Add rotations
- [x] Add collisions
- [x] Add main game loop
- [x] Add line elimination
  - > Perhaps I should've used vectors...
- [x] Fix the bug that causes blocks to stop falling
- [x] Add end game state
- [x] Add block sliding
- [x] Add correct tetromino alignments
- [x] Add tetris standard drop order
  - Created bags branch. Will merge branch when I have a working implementation.
- [x] Prevent rotation from causing clipping (offset bumping)
  - Pieces will no longer be allowed to rotate if rotation would cause clipping.
- [x] Add color graphics
- [ ] Add game acceleration
- [ ] Support Windows 🪟

## Installation (but why would you want that?)
:warning: The windows branch contains windows-compatible code, but it only works on the Windows 11 Terminal. In reality, you should run the linux code on a WSL instance.

```sh
git clone https:://github.com/reckoncrafter/tetris.git
cd tetris
make
```
