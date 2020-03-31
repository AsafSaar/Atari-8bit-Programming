cl65 -t atari -O -S main.c
move main.s main.asm

cl65 --debug-info -Wl --dbgfile,main.lab -m main.map -Ln main.lbl -t atari -Oi main.c -C atari.cfg -o game.xex

game.xex
