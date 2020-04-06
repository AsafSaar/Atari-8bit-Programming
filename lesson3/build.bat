cl65 -t atari -O -S main.c

move main.s main.asm

cl65 --debug-info -Wl --dbgfile,lesson2.lab -m lesson2.map -Ln lesson2.lbl -t atari -Oi main.c -C atari.cfg -o game.xex

game.xex
