    !source "common.inc"

main:
    ;LDA #$72 : STA $D640 : CLV : BCC error

; >>> EXAMPLE BEGINS
@10
    ; Toggle the CPU personality
    LDA #$72 : STA $D640 : CLV : BCC error
    ; Toogle again if bit 5 of A is clear
    AND #%00100000 : BEQ @10
; >>> EXAMPLE ENDS

good:
    LDX #$53 : STX $800 : RTS

error:
    LDX #$56 : STX $800 : STA $801 : RTS
