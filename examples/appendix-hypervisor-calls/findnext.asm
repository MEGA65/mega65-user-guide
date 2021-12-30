    !source "common.inc"

example:

; >>> EXAMPLE BEGINS
    ; Assume the current Hyppo filename has already been set.
    ; Reset the count in X.
    LDX #0
    ; Try to find the first matching file.
    LDA #$30
@10:
    STA $D640 : CLV : BCC @20
    ; Increment the count in X
    INX
    ; Try to find the next matching file.
    LDA #$32
    BRA @10
@20:
    ; If the error code in A is $88 there are no more matching files, otherwise an error occurred.
    CMP #$88 : BNE error
    ; No need to close the file handle because the search exhausted.
; >>> EXAMPLE ENDS

done:
    LDY #$FF : STY $802
    STX $803
    RTS

error:
    LDY #$21 : STY $821
    STA $822
    RTS

main:
    LDY #$00 : STY $800
    LDY #>filename : LDA #$2E : STA $D640 : CLV : BCC error
    LDZ #$01 : STZ $801
    JMP example

    !align 255, 0
filename:
    !text "charrom.m65", 0
