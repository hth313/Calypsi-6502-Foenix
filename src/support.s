move8:	      .macro  src, dest
	      lda     \src
	      sta     \dest
#ifdef __CALYPSI_CORE_65C02__
	      stz     \dest + 1
#else
	      lda     #0
	      sta     \dest + 1
#endif
	      .endm

	      .public _ClearScreen
	      .extern _Row, _Col, _Zp
	      .section code, text
_ClearScreen:
	      lda     #0xc0
	      sta     zp:_Zp+1
	      ldy     #0
	      sty     zp:_Zp+0

	      lda     #2
	      sta     zp:1

locations:    .equ    60 * 80
	      ldx     #locations / 256
	      lda     #32
clear256$:    sta     (_Zp),y
	      iny
	      bne     clear256$
	      inc     zp:_Zp+1
	      dex
	      bne     clear256$

	      ldy     #locations % 256
clear$:	      sta     (_Zp),y
	      dey
	      cpy     #255
	      bne     clear$

#ifdef __CALYPSI_CORE_65C02__
	      stz     zp:1
#else
	      lda     #0
	      sta     zp:1
#endif
	      lda     #9
	      sta     0xd010
	      move8   _Row,0xd016
	      move8   _Col,0xd014
	      move8   #'_', 0xd012
	      rts
