    !source "common.inc"

; >>> EXAMPLE BEGINS
;---------------------------------------------------------------------------------------
; Ergens in het gegevensgebied van het programma

    ; Uitlijnen op de nieuwe paginagrens
    !align 255, 0

transferarea:
    ; 256 bytes reserveren
    !skip 256

;---------------------------------------------------------------------------------------
; Elders in de programmacode

setuptransferarea:
    ; Stel de transferarea in als het Hyppo-overdrachtsgebied
    LDY #>transferarea : LDA #$3A : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

good:
    LDA #$53 : STA $800 : RTS

error:
    LDA #$56 : STA $800 : RTS

main:
    JMP setuptransferarea
