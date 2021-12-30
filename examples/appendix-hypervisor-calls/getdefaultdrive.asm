    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    ; Get the default drive
    LDA #$02 : STA $D640 : CLV : BCC @error

    ; Transfer the drive number in A to X
    TAX

    ; Select the default drive
    LDA #$06 : STA $D640 : CLV : BCC @error
; >>> EXAMPLE ENDS

@done
    RTS

@error:
    BRK
