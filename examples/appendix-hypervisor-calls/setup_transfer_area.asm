    !source "common.inc"

; >>> EXAMPLE BEGINS
;---------------------------------------------------------------------------------------
; Somewhere in the program's data area

    ; Align with the new page boundary
    !align 255, 0

transferarea:
    ; Reserve 256 bytes
    !skip 256

;---------------------------------------------------------------------------------------
; Elsewhere in the progrma's code

setuptransferarea:
    ; Set the transferarea as the Hyppo transfer area
    LDY #>transferarea : LDA #$3A : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

good:
    LDA #$53 : STA $800 : RTS

error:
    LDA #$56 : STA $800 : RTS

main:
    JMP setuptransferarea
