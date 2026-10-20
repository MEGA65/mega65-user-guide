    !source "common.inc"

example:

; >>> EXAMPLE BEGINS
    ; Neem aan dat het bestand al geopend is.
    ; Koppel kleuren-RAM los van $DC00 om de sectorbuffer te kunnen inladen
    LDA $D030 : PHA : AND #%11111110 : STA $D030
@10:
    ; Lees de volgende sector
    LDA #$1A : STA $D640 : CLV : BCC @20
    ; Breng de sectorbuffer naar $DE00
    LDA #$81 : STA $D680
    ; Roep processsector aan (wordt elders gedefinieerd)
    JSR processsector
    ; Koppel de sectorbuffer los van $DE00
    LDA #$82 : STA $D680
    BRA @10
@20:
    ; Als de foutcode in A $FF is, hebben we het einde van het bestand bereikt; anders is er een fout opgetreden
    CMP #$FF : BNE error
    ; Breng kleuren-RAM terug naar $DC00 als het daarvoor in het geheugen stond
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
