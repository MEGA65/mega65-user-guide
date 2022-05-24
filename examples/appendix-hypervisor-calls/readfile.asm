    !source "common.inc"

example:

; >>> EXAMPLE BEGINS
    ; Assume the file is already open.
    ; Unmap the colour RAM from $DC00 because that will prevent us from mapping in the sector buffer
    LDA $D030 : PHA : AND #%11111110 : STA $D030
@10:
    ; Read the next sector
    LDA #$1A : STA $D640 : CLV : BCC @20
    ; Map the sector buffer to $DE00
    LDA #$81 : STA $D680
    ; Call processsector (assumed to be defined elsewhere)
    JSR processsector
    ; Unmap the sector buffer from $DE00
    LDA #$82 : STA $D680
    BRA @10
@20:
    ; If the error code in A is $FF we have reached the end of the file otherwise there's been an error
    CMP #$FF : BNE error
    ; Map the colour RAM at $DC00 if it was previously mapped
    PLA : STA $D030
; >>> EXAMPLE ENDS

done:
    LDX #4: STX $804
    PLX
    LDA #$20 : STA $D640 : CLV : BCC error
    LDX #$41: STX $805 : RTS

error:
    LDX #$56 : STX $805 : STA $806 : RTS

processsector:
    TZA : TAX : STZ $850,X
    INZ
    RTS

main:
    LDX #0: STX $800
    LDY #>filename : LDA #$2E : STA $D640 : CLV : BCC error
    LDX #1: STX $801
    LDA #$34 : STA $D640 : CLV : BCC error
    LDX #2: STX $802
    LDA #$18 : STA $D640 : CLV : BCC error
    LDX #3: STX $803
    PHA
    LDZ #0
    JMP example

    !align 255, 0
filename:
    !text "CHARROM.M65", 0
