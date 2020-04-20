cd /D "%~dp0"
del LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.xex

Z:\home\kobi\cc65-win\bin\cl65.exe --version

Z:\home\kobi\cc65-win\bin\cl65.exe --debug-info -Wl --dbgfile,LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.lab -m LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.map -Ln LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.lbl -t atari -Oi segments.s main.c -o LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.xex -C atari.cfg 

Z:\home\kobi\Altirra-3.10\Altirra.exe LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.xex

rem cl65 --debug-info -Wl --dbgfile,LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.lab -m LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.map -Ln LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.lbl -t atari -Oi segments.s main.c -o LESSON_5_PHARAOHS_CURSE_SCREEN_SCRAPE.xex -C atari.cfg 
