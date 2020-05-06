.segment "CHARSET_0"
_L_CHARSET_0:
.incbin "charset_0_data.bin"
.export _L_CHARSET_0

.segment "SCREEN"
_L_SCREEN:
.incbin "screen_data.bin"
.export _L_SCREEN

.segment "SPRITES"
_L_SPRITES:
.incbin "sprites_data.bin"
.export _L_SPRITES
