    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    ; Verkrijg de versienummers
    LDA #$00 : STA $D640 : CLV
    ; Controleer of het hoofdversienummer van Hyppo in A gelijk is aan 1
    CMP #1 : BNE incompatible
    ; Controleer of het subversienummer van Hyppo in X gelijk is aan 2 of hoger
    TXA : CMP #2 : BMI incompatible
    ; Deze Hyppo-versie is compatibel
; >>> EXAMPLE ENDS

compatible:
    LDA #$53 : STA $800 : RTS

incompatible:
    LDA #$56 : STA $800 : RTS
