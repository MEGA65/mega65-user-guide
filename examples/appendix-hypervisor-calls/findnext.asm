    !source "common.inc"

main:
    LDZ #$00 : STZ $0800
    LDY #>filename
    LDA #$2E
    STA $D640
    CLV
    BCC @error
    LDZ #$01 : STZ $0801
    JSR example
    LDZ #$FF : STZ $0802
    STX $0803
    RTS
@error:
    LDZ #$21 : STZ $0821
    STA $0822
    BRK

example:

; >>> EXAMPLE BEGINS
    ; Assume the current Hyppo filename has already been set.
    ; Reset the count in X
    LDX #0
    ; Try to find the first matching file.
    LDA #$30
@loop:
    STA $D640
    CLV
    BCC @notfoundorerror
    ; Increment the count in X
    INX
    ; Try to find the next matching file.
    LDA #$32
    BRA @loop
@notfoundorerror:
    ; If the error code in A is $88 there are no more matching files, otherwise an error occurred.
    CMP #$88
    BNE @error
@done:
    ; No need to close the file handle because the search exhausted.
    RTS
@error:
    BRK
; >>> EXAMPLE ENDS

    ; Fill with zeros until a page boundary is reached
    !align 255, 0
filename:
    !text "charrom.m65", 0
