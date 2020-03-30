cl65 -t atari -O -S main.c
move main.s main.asm

cl65 --debug-info -Wl --dbgfile,guess.lab -m guess.map -Ln guess.lbl -t atari -Oi main.c -o guess.xex -C atari.cfg

guess.xex
