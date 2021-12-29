; >>> EXAMPLE BEGINS
    ; Preserve the current drive in Z so we can restore it later
    LDA #$04
    STA $D640
    CLV
    BCC @error
    TAZ
    ; Try to select drive 2
    LDX #2
    LDA #$06
    STA $D640
    CLV
    BCC @mightnotexist
    ; Transfer the previously selected drive number in Z to X
    TZA
    TAX
    ; Restore the previously selected drive
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
    ; Clear the C flag because the drive doesn't exist
    CLC
    RTS
@error:
    BRK
; >>> EXAMPLE ENDS
