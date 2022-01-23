    !source "common.inc"

; >>> EXAMPLE BEGINS
; Somewhere in the program's data area
    !align 255, 0 ; Align with the next page boundary
filename:
    !text "GAME.MAP", 0 ; Must end with a 0 byte

; Elsewhere in the progrma's code
setfilename:
    LDY #>filename : LDA #$2E : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

good:
    LDX #$53 : STX $800 : RTS

error:
    LDX #$56 : STX $800 : STA $801 : RTS

main:
    JMP setfilename
