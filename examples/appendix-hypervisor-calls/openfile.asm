    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    ; Set the filename to CONFIG.DAT
    LDY #>filename
    LDA #$2E
    STA $D640
    CLV
    BCC @error
    ; Find the file
    LDA #$34
    STA $D640
    CLV
    BCC @error
    ; Open the file
    LDA #$18
    STA $D640
    CLV
    BCC @error
    ; Transfer the file descriptor in A to X
    TAX
    ; Close the file
    LDA #$20
    STA $D640
    CLV
    BCC @error
    RTS
@error:
    BRK
    ; Fill with zeros until a page boundary is reached
    !align 255, 0
filename:
    !text "CONFIG.DAT", 0
; >>> EXAMPLE ENDS
