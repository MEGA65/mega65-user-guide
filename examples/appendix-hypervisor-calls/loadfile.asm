    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    ; Set the filename to SHOOTEMUP-001.LEVEL.
    LDY #>filename
    LDA #$2E
    STA $D640
    CLV
    BCC @error
    ; No need to find the file first.
    ; Load the file starting at $42000.
    LDZ #$04 ; Most significant byte
    LDY #$80 ; Middle byte
    LDX #$00 ; Least significant byte
    LDA #$36
    STA $D640
    CLV
    BCC @error
    RTS
@error:
    BRK
    ; Fill with zeros until a page boundary is reached
    !align 255, 0
filename:
    !text "SHOOTEMUP-001.LEVEL", 0
; >>> EXAMPLE ENDS
