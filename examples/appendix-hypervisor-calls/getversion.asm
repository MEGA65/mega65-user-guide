    !source "common.inc"

main:

; >>> EXAMPLE BEGINS
    ; Get the version numbers
    LDA #$00 : STA $D640 : CLV
    ; Test if the major version number of Hyppo in A is 1
    CMP #1 : BNE incompatible
    ; Test if the minor version number of Hyppo in X is 2 or more
    TXA : CMP #2 : BMI incompatible
    ; This Hyppo version is compatible
; >>> EXAMPLE ENDS

compatible:
    LDA #$53 : STA $800 : RTS

incompatible:
    LDA #$56 : STA $800 : RTS
