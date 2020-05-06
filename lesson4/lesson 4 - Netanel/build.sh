#!/bin/bash
source ~/.bash_profile
cl65 --debug-info -Wl --dbgfile,main.lab -m main.map -Ln main.lbl -t atari -Oi main.c -o main.xex -C atari.cfg && altirra main.xex