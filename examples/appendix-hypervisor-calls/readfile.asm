    !source "common.inc"

main:
    ; Find and open the file
    LDX #0: STX $800
    LDY #>filename
    LDA #$2E
    STA $D640
    CLV
    BCC @error
    LDX #1: STX $801
    LDA #$34
    STA $D640
    CLV
    BCC @error
    LDX #2: STX $802
    LDA #$18
    STA $D640
    CLV
    BCC @error
    LDX #3: STX $803
    PHA
    LDZ #0
    JSR example
    LDX #4: STX $804
    PLX
    LDA #$20
    STA $D640
    CLV
    BCC @error
    LDX #5: STX $805
    RTS
@error:
    LDX #$21: STX $806
    STA $807
    BRK

example:

; >>> EXAMPLE BEGINS
    ; Unmap the colour RAM from $DC00 because that will prevent us from mapping in the sector buffer
    LDA $d030
    PHA
    AND #%11111110
    STA $d030
@nextsector:
    ; Read the next sector
    LDA #$1A
    STA $D640
    CLV
    BCC @eoforerror
    ; Map the sector buffer to $DE00
    LDA #$81
    STA $D680
    ; Call processsector (assumed to be defined elsewhere)
    JSR processsector
    ; Unmap the sector buffer from $DE00
    LDA #$82
    STA $D680
    BRA @nextsector
@eoforerror:
    ; If the error code in A is $FF we have reached the end of the file otherwise there's been an error
    CMP #$FF
    BNE @error
    ; Map the colour RAM at $DC00 if it was previously mapped
    PLA
    STA $d030
    RTS
@error:
    BRK
; >>> EXAMPLE ENDS

processsector:
    TZA
    TAX
    STZ $850,X
    INZ
    RTS

    !align 255, 0
filename:
    !text "CHARROM.M65", 0
