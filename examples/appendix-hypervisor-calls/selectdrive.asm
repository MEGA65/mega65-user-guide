    !source "common.inc"

; >>> EXAMPLE BEGINS
doesdrive2exist:
    ; Preserve the current drive so we can restore it later
    LDA #$04 : STA $D640 : CLV : BCC error
    PHA
    ; Try to select drive 2
    LDX #2 : LDA #$06 : STA $D640 : CLV : BCC @10
    ; Restore the previously selected drive
    PLX
    LDA #$06 : STA $D640 : CLV : BCC error
    ; The C flag was already set by the Hyppo service
    RTS

@10:
    ; If the error code in A is $80, the drive doesn't exist; otherwise some other kind of error occurred
    CMP #$80
    BNE error
    ; Forget about the current drive we preserved because it wasn't changed
    PLX
    ; Clear the C flag because the drive doesn't exist
    CLC
    RTS
; >>> EXAMPLE ENDS

main:
    JSR doesdrive2exist
    LDA #$53 : STA $800
    BCC @10
    LDA #$19
    !8 $2C
@10:
    LDA #$0E
    STA $801
    RTS

error:
    LDA #$56 : STA $800 : RTS
