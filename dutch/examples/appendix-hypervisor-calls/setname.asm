    !source "common.inc"

; >>> EXAMPLE BEGINS
; Ergens in het gegevensgebied van het programma
    !align 255, 0 ; Uitlijnen op de volgende paginagrens
filename:
    !text "GAME.MAP", 0 ; Moet eindigen met een nulbyte

; Elders in de programmacode
setfilename:
    LDY #>filename : LDA #$2E : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

good:
    LDX #$53 : STX $800 : RTS

error:
    LDX #$56 : STX $800 : STA $801 : RTS

main:
    JMP setfilename
