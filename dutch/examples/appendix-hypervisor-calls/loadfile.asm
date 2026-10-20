    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    ; Neem aan dat de huidige hyppo-bestandsnaam al is ingesteld.
    ; Het is niet nodig het bestand eerst te zoeken.
    LDZ #$04 ; Meest significante byte
    LDY #$80 ; Middelste byte
    LDX #$00 ; Minst significante byte
    LDA #$36 : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

done:
    LDA #$53 : STA $800 : RTS

error:
    LDA #$56 : STA $800 : RTS

    !align 255, 0 ; Uitlijnen op de nieuwe paginagrens
filename:
    !text "SHOOTEMUP-001.LEVEL", 0
