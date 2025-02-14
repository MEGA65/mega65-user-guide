    !source "common.inc"

; >>> EXAMPLE BEGINS
; Somewhere in the program's data area
    !align 255, 0 ; Align with the next page boundary
disk2name:
    !text "DISK2.D81", 0 ; Must end with a 0 byte

; Elsewhere in the progrma's code
attachdisk:
    ; Set the Hyppo filename to DISK2.D81
    LDY #>disk2name : LDA #$2E : STA $D640 : CLV : BCC error
    ; Attach the disk image
    LDX #$00 : LDA #$4A : STA $D640 : CLV : BCC error
    RTS

detachdisk:
    ; Detach the disk image and reconnect real drive
    LDX #$80 : LDA #$4A : STA $D640 : CLV : BCC error
    RTS

good:
    LDX #$53 : STX $800 : RTS

error:
    LDX #$56 : STX $800 : STA $801 : RTS

main:
    JSR attachdisk
    JSR detachdisk
