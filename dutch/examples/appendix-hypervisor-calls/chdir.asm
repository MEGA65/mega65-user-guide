    !source "common.inc"

; >>> EXAMPLE BEGINS
    !addr pathptr = $C4
chdirpath:
    STX pathptr : STY pathptr+1 ; Stel pathptr in op Y:Z
@chdirpath10:
    JSR @copycomponent  ; Haal het volgende onderdeel van het pad op
    BEQ @chdirpath20    ; Stop bij een nulbyte in het pad
    JSR @trychdir       ; Probeer van map te veranderen
    BCC @chdirpath30    ; Stop als we niet van map kunnen veranderen
    BRA @chdirpath10    ; Herhaal met het volgende onderdeel
@chdirpath20: SEC
@chdirpath30: RTS
;----------------------------------------------------------------------------------------
@trychdir:
    ; Stel de hyppo-bestandsnaam in vanuit transferbuffer
    LDY #>transferbuffer : LDA #$2E : STA $D640 : CLV : BCC @trychdir10
    ; Zoek de FAT-dir-entry
    LDA #$34 : STA $D640 : CLV : BCC @trychdir10
    ; Verander van map
    LDA #$0C : STA $D640 : CLV
@trychdir10:
    RTS
;----------------------------------------------------------------------------------------
    ; Kopieer het volgende onderdeel van het pad naar transferbuffer
@copycomponent:
    LDY #0
@copycomponent10:
    LDA (pathptr),Y
    BEQ @copycomponent20
    TAX
    AND #$7F : STA transferbuffer,Y
    INY
    TXA : BPL @copycomponent10
    ; Voeg een nulbyte toe aan transferbuffer
    LDA #0 : STA transferbuffer,Y
    ; Verplaats pathptr
    TYA
    CLC : ADC pathptr : STA pathptr : LDA #0 : ADC pathptr+1 : STA pathptr+1
@copycomponent20:
    RTS
; >>> EXAMPLE ENDS

main:
    LDX #0: STX $800
    LDX #<@path
    LDY #>@path
    JSR chdirpath
    BCC @10
    LDX #$53: STX $801
    RTS
@10:
    LDX #$56: STX $801; STA $802
    RTS

@path:
    !text "DIR-", 'A'+$80
    !text "DIR-A", 'A'+$80
    !text "DIR-AA", 'A'+$80
    !8 0

    !align 255, 0
transferbuffer: !skip 256
