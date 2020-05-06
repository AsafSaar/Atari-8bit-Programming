.segment "MYDLIST"
_L_MYDLIST: 
.incbin "dlist.bin"
.export _L_MYDLIST

.segment "MEM1"
_L_MEM1: 
.incbin "mem1.bin"
.export _L_MEM1

.segment "MEM2"
_L_MEM2: 
.incbin "mem2.bin"
.export _L_MEM2

.segment "CHR1"
_L_CHR1: 
.incbin "chr1.bin"
.export _L_CHR1

.segment "CHR2"
_L_CHR2: 
.incbin "chr2.bin"
.export _L_CHR2

.segment "CHR3"
_L_CHR3: 
.incbin "chr3.bin"
.export _L_CHR3