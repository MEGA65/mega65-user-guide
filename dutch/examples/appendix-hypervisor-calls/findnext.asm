    !source "common.inc"

example:

; >>> EXAMPLE BEGINS
    ; Neem aan dat de huidige hyppo-bestandsnaam al ingesteld is.
    ; Herstel de teller in X.
    LDX #0
    ; Probeer het eerste overeenkomende bestand te vinden.
    LDA #$30
@10:
    STA $D640 : CLV : BCC @20
    ; Verhoog de teller in X
    INX
    ; Probeer het volgende overeenkomende bestand te vinden.
    LDA #$32
    BRA @10
@20:
    ; Als de foutcode in A $88 is, zijn er geen overeenkomende bestanden meer; anders is er een fout opgetreden.
    CMP #$88 : BNE error
    ; Sluiten is niet nodig omdat de zoekactie voltooid is.
; >>> EXAMPLE ENDS

done:
    LDY #$FF : STY $802
    STX $803
    RTS

error:
    LDY #$21 : STY $821
    STA $822
    RTS

main:
    LDY #$00 : STY $800
    LDY #>filename : LDA #$2E : STA $D640 : CLV : BCC error
    LDZ #$01 : STZ $801
    JMP example

    !align 255, 0
filename:
    !text "charrom.m65", 0
