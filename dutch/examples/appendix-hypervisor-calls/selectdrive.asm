    !source "common.inc"

; >>> EXAMPLE BEGINS
doesdrive2exist:
    ; Bewaar het huidige station zodat we het later kunnen herstellen
    LDA #$04 : STA $D640 : CLV : BCC error
    PHA
    ; Probeer station 2 te selecteren
    LDX #2 : LDA #$06 : STA $D640 : CLV : BCC @10
    ; Herstel het eerder geselecteerde station
    PLX
    LDA #$06 : STA $D640 : CLV : BCC error
    ; De C-vlag was reeds gezet door de Hyppo-service
    RTS

@10:
    ; Als de foutcode in A $80 is, bestaat het station niet; anders is een andere fout opgetreden
    CMP #$80
    BNE error
    ; Negeer het huidige station dat we bewaarden, want het is niet gewijzigd
    PLX
    ; Wis de C-vlag omdat het station niet bestaat
    CLC
    RTS
; >>> EXAMPLE ENDS

main:
    JSR doesdrive2exist
    LDA #$53 : STA $800
    BCC @10
    LDA #$19
    !8 $2C
@10:
    LDA #$0E
    STA $801
    RTS

error:
    LDA #$56 : STA $800 : RTS
