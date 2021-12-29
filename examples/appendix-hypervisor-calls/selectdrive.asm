    !source "common.inc"

main:
    JSR example
    BCC @nodrive2
    LDA #$53
    !8 $2C
@nodrive2
    LDA #$56
    STA $800
    RTS

example:

; >>> EXAMPLE BEGINS
    ; Preserve the current drive so we can restore it later
    LDA #$04
    STA $D640
    CLV
    BCC @error
    PHA
    ; Try to select drive 2
    LDX #8
    LDA #$06
    STA $D640
    CLV
    BCC @mightnotexist
    ; Restore the previously selected drive
    PLX
    LDA #$06
    STA $D640
    CLV
    BCC @error
    ; The C flag was already set by the Hyppo service
    RTS
@mightnotexist:
    ; If the error code in A is $80, the drive doesn't exist; otherwise some other kind of error occurred
    CMP #$80
    BNE @error
    ; Forget about the current drive we preserved because it wasn't changed
    PLX
    ; Clear the C flag because the drive doesn't exist
    CLC
    RTS
@error:
    BRK
; >>> EXAMPLE ENDS
