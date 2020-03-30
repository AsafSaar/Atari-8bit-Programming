cd /D "%~dp0"
del codebrkr.xex

Z:\home\kobi\cc65-win\bin\cl65.exe --version

Z:\home\kobi\cc65-win\bin\cl65.exe --debug-info -Wl --dbgfile,codebrkr.lab -m codebrkr.map -Ln codebrkr.lbl -t atari -Oi main.c -o codebrkr.xex -C atari.cfg 

Z:\home\kobi\Altirra-3.10\Altirra.exe codebrkr.xex
