    !source "common.inc"

example:

; >>> EXAMPLE BEGINS
    ; Verkrijg het huidige station
    LDA #$04 : STA $D640 : CLV : BCC error

    ; Zet het stationsnummer in A om naar een schermcode
    CLC : ADC #$30

    ; Zet de schermcode in de linkerbovenhoek van het schermgeheugen
    STA $0800
; >>> EXAMPLE ENDS

done:
    LDA #$53 : STA $850 : RTS

error:
    LDA #$56 : STA $850 : RTS

main:
    JMP example
