    !source "common.inc"

example:

; >>> EXAMPLE BEGINS
    ; Assume the current Hyppo filename has already been set.
    ; Find the file
    LDA #$34 : STA $D640 : CLV : BCC error
    ; Open the file
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

    !align 255, 0 ; Align with the new page boundary
filename:
    !text "CHARROM.M65", 0
