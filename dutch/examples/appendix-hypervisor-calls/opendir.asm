    !source "common.inc"

main:
    LDZ #0

; >>> EXAMPLE BEGINS
    ; Open de huidige werkmap
    LDA #$12 : STA $D640 : CLV : BCC error
    ; Verplaats de bestandsdescriptor naar X
    TAX
    ; Stel Y in op het MSB van het overdrachtsgebied
    LDY #>transferarea
@10:
    ; Lees het directory-item
    LDA #$14 : STA $D640 : CLV : BCC @20
    ; Roep processdirentry aan (wordt elders gedefinieerd)
    PHX : PHY : JSR processdirentry : PLY : PLX
    BRA @10
@20:
    ; Als de foutcode in A $85 is, hebben we het einde van de map bereikt; anders is er een fout opgetreden
    CMP #$85 : BNE error
    ; Sluit de bestandsindicator in X
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
