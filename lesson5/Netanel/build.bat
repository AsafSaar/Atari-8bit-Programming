cl65 --debug-info -Wl --dbgfile,main.lab -m main.map -Ln main.lbl -t atari -Oi segments.s main.c -o main.xex -C atari.cfg
if %ERRORLEVEL% == 0 main.xex