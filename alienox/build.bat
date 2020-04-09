cd /D "%~dp0"
del alienox.xex

Z:\home\kobi\cc65-win\bin\cl65.exe --version

Z:\home\kobi\cc65-win\bin\cl65.exe --debug-info -Wl --dbgfile,alienox.lab -m alienox.map -Ln alienox.lbl -t atari -Oi main.c -o alienox.xex -C atari.cfg 

Z:\home\kobi\Altirra-3.10\Altirra.exe alienox.xex

rem cl65 --debug-info -Wl --dbgfile,alienox.lab -m alienox.map -Ln alienox.lbl -t atari -Oi main.c -o alienox.xex -C atari.cfg 
