    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    LDY #>transferarea
    LDA #$3A
    STA $D640
    CLV
    BCC @error
    RTS
@error:
    BRK
    ; Fill with zeros until a page boundary is reached
    !align 255, 0
transferarea:
    ; Reserve 256 bytes
    !skip 256
; >>> EXAMPLE ENDS
