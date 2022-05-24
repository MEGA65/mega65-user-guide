    !source "common.inc"

main:
    LDZ #0

; >>> EXAMPLE BEGINS
    ; Open the current working directory
    LDA #$12 : STA $D640 : CLV : BCC error
    ; Transfer the directory file descriptor into X
    TAX
    ; Set Y to the MSB of the transfer area
    LDY #>transferarea
@10:
    ; Read the directory entry
    LDA #$14 : STA $D640 : CLV : BCC @20
    ; Call processdirentry (assumed to be defined elsewhere)
    PHX : PHY : JSR processdirentry : PLY : PLX
    BRA @10
@20:
    ; If the error code in A is $85 we have reached the end of the directory otherwise there's been an error
    CMP #$85 : BNE error
    ; Close the directory file descriptor in X
    LDA #$16 : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

done:
    LDX #$53 : STX $800 : RTS

error:
    LDX #$56 : STX $800 : STA $801 : RTS

processdirentry:
    TZA : TAX : STZ $850,X
    INZ
    RTS

    !align 255, 0
transferarea:
    !skip 256
