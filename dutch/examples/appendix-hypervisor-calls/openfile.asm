    !source "common.inc"

example:

; >>> EXAMPLE BEGINS
    ; Neem aan dat de huidige hyppo-bestandsnaam al is ingesteld.
    ; Zoek het bestand
    LDA #$34 : STA $D640 : CLV : BCC error
    ; Open het bestand
    LDA #$18 : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

done:
    LDX #$53 : STX $802 : RTS

error:
    LDX #$56 : STX $802 : STA $803 : RTS

main:
    LDY #$00 : STY $800
    LDY #>filename : LDA #$2E : STA $D640 : CLV : BCC error
    LDZ #$01 : STZ $801
    JMP example

    !align 255, 0 ; Uitlijnen op de nieuwe paginagrens
filename:
    !text "CHARROM.M65", 0
