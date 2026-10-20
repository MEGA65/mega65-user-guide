    !source "common.inc"

main:
    ;LDA #$72 : STA $D640 : CLV : BCC error

; >>> EXAMPLE BEGINS
@10
    ; Wissel de CPU-persoonlijkheid
    LDA #$72 : STA $D640 : CLV : BCC error
    ; Wissel opnieuw indien bit 5 van A niet ingesteld is
    AND #%00100000 : BEQ @10
; >>> EXAMPLE ENDS

good:
    LDX #$53 : STX $800 : RTS

error:
    LDX #$56 : STX $800 : STA $801 : RTS
