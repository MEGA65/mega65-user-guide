    !source "common.inc"

example:

; >>> EXAMPLE BEGINS
    ; Get the current drive
    LDA #$04 : STA $D640 : CLV : BCC error

    ; Convert the drive number in A into a screen code
    CLC : ADC #$30

    ; Put the screen code into the top-left of screen memory
    STA $0800
; >>> EXAMPLE ENDS

done:
    LDA #$53 : STA $850 : RTS

error:
    LDA #$56 : STA $850 : RTS

main:
    JMP example
