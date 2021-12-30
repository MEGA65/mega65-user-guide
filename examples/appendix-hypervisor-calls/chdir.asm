    !source "common.inc"

main:
    LDX #0: STX $800
    LDX #<@path
    LDY #>@path
    JSR chdirpath
    BCC @10
    LDX #$53: STX $801
    RTS
@10:
    LDX #$56: STX $801; STA $802
    RTS
@path:
    !text "DIR-", 'A'+$80
    !text "DIR-A", 'A'+$80
    !text "DIR-AA", 'A'+$80
    !8 0

; >>> EXAMPLE BEGINS
    !addr pathptr = $C4
chdirpath:
    STX pathptr : STY pathptr+1 ; Set pathptr to Y:Z
@chdirpath10:
    JSR @copycomponent  ; Get the next component of the path
    BEQ @chdirpath20    ; Stop at a 0 byte in the path
    JSR @trychdir       ; Try to change into the directory
    BCC @chdirpath30    ; Stop if we cannot change into the directory
    BRA @chdirpath10    ; Repeat with the next component
@chdirpath20: SEC
@chdirpath30: RTS
;----------------------------------------------------------------------------------------
@trychdir:
    ; Set the Hyppo filename from @transferbuffer
    LDY #>@transferbuffer : LDA #$2E : STA $D640 : CLV : BCC @trychdir10
    ; Find the FAT dir entry
    LDA #$34 : STA $D640 : CLV : BCC @trychdir10
    ; Chdir into the directory
    LDA #$0C : STA $D640 : CLV
@trychdir10:
    RTS
;----------------------------------------------------------------------------------------
    ; Copy the next component of the path into @transferbuffer
@copycomponent:
    LDY #0
@copycomponent10:
    LDA (pathptr),Y
    BEQ @copycomponent20
    TAX
    AND #$7F : STA @transferbuffer,Y
    INY
    TXA : BPL @copycomponent10
    ; Add a 0 terminating byte to @transferbuffer
    LDA #0 : STA @transferbuffer,Y
    ; Move pathptr
    TYA
    CLC : ADC pathptr : STA pathptr : LDA #0 : ADC pathptr+1 : STA pathptr+1
@copycomponent20:
    RTS
;----------------------------------------------------------------------------------------
    !align 255, 0
@transferbuffer: !skip 256
; >>> EXAMPLE ENDS
