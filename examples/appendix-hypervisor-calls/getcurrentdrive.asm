; >>> EXAMPLE BEGINS
    ; Get the current drive
    LDA #$04
    STA $D640
    CLV
    BCC @error
    ; Convert the drive number in A into a screen code
    STA @temp
    LDA #$30
    ADC @temp
    LDA @temp
    ; Put the screen code into the top-left of screen memory
    STA $0800
    RTS
@error:
    BRK
@temp:
    !skip 1
; >>> EXAMPLE ENDS
