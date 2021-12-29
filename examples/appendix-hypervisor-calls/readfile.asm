; >>> EXAMPLE BEGINS
    ; Unmap the colour RAM at $DC00 because that will prevent us from mapping in the sector buffer
    LDA #1
    TRB $D030
    ; Map the sector buffer to $DE00
    LDA #$81
    STA $D680
@nextsector:
    ; Read the next sector
    LDA #$1A
    STA $D640
    CLV
    BCC @eoforerror
    ; Call processsector (assumed to be defined elsewhere)
    JSR processsector
    BRA @nextsector
@eoforerror:
    ; If the error code in A is $FF we have reached the end of the file otherwise there's been an error
    CMP #$FF
    BNE @error
    RTS
@error:
    BRK
; >>> EXAMPLE ENDS
