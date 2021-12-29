; >>> EXAMPLE BEGINS
    ; Get the version numbers
    LDA #$00
    STA $D640
    CLV
    ; Test if the major version number of Hyppo is 1
    CMP #1
    BNE @incompatible
    ; Test if the minor version number of Hyppo is 2 or more
    TXA
    CMP #2
    BMI @incompatible
    RTS
@incompatible:
    BRK
; >>> EXAMPLE ENDS
