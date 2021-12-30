    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    ; Assume the current Hyppo filename has already been set.
    ; No need to find the file first.
    LDZ #$04 ; Most significant byte
    LDY #$80 ; Middle byte
    LDX #$00 ; Least significant byte
    LDA #$36 : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

done:
    LDA #$53 : STA $800 : RTS

error:
    LDA #$56 : STA $800 : RTS

    !align 255, 0 ; Align with the new page boundary
filename:
    !text "SHOOTEMUP-001.LEVEL", 0
