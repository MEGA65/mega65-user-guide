    !source "common.inc"

; >>> EXAMPLE BEGINS
; Ergens in het gegevensgebied van het programma
    !align 255, 0 ; Lijn uit op de volgende paginagrens
disk2name:
    !text "DISK2.D81", 0 ; Moet eindigen met een nulbyte

; Elders in de code van het programma
attachdisk2:
    ; Stel de hyppo-bestandsnaam in op DISK2.D81
    LDY #>disk2name : LDA #$2E : STA $D640 : CLV : BCC error
    ; Koppel de schijfimage aan
    LDA #$40 : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

good:
    LDX #$53 : STX $800 : RTS

error:
    LDX #$56 : STX $800 : STA $801 : RTS

main:
    JMP attachdisk2
