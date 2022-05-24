    !source "common.inc"

; >>> EXAMPLE BEGINS
; Somewhere in the program's data area
    !align 255, 0 ; Align with the next page boundary
disk2name:
    !text "DISK2.D81", 0 ; Must end with a 0 byte

; Elsewhere in the progrma's code
attachdisk2:
    ; Set the Hyppo filename to DISK2.D81
    LDY #>disk2name : LDA #$2E : STA $D640 : CLV : BCC error
    ; Attach the disk image
    LDA #$40 : STA $D640 : CLV : BCC error
; >>> EXAMPLE ENDS

good:
    LDX #$53 : STX $800 : RTS

error:
    LDX #$56 : STX $800 : STA $801 : RTS

main:
    JMP attachdisk2
