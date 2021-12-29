    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    ; Get the current drive
    LDA #$04
    STA $D640
    CLV
    BCC @error
    ; Convert the drive number in A into a screen code
    CLC
    ADC #$30
    ; Put the screen code into the top-left of screen memory
    STA $0800
    RTS
@error:
    BRK
; >>> EXAMPLE ENDS
