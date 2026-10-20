    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    ; Verkrijg het standaardstation
    LDA #$02 : STA $D640 : CLV : BCC @error

    ; Kopieer het stationsnummer van A naar X
    TAX

    ; Selecteer het standaardstation
    LDA #$06 : STA $D640 : CLV : BCC @error
; >>> EXAMPLE ENDS

@done
    RTS

@error:
    BRK
