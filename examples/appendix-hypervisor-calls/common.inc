        * = $2001

        !16     +
!ifdef NOAUTOSTART {
        !16     5                                       ; Line number
        !8      $80                                     ; END
        !8      0                                       ; End of BASIC line
+       !16     +
}
        !16     10                                      ; Line number
        !8      $E8                                     ; SCNCLR
        !8      ':'
        !8      $FE, $41, '0', ',', '1', '0'            ; CURSOR 0,10
        !8      ':'
        !8      $FE, $02, '0'                           ; BANK 0
        !8      ':'
        !8      $9E                                     ; SYS startup
        !8      <(((startup /  1000) % 10) + '0')
        !8      <(((startup /   100) % 10) + '0')
        !8      <(((startup /    10) % 10) + '0')
        !8      <(((startup /     1) % 10) + '0')
        !8      0                                       ; End of BASIC line
+       !16     0                                       ; BASIC end marker


startup:
        sei
        tsx                                             ; Save the system stack ptr
        stx     sp_save
        lda     #1                                      ; Don't call BASIC in the kernel's IRQ handler
        trb     $1104
        lda     $d030                                   ; Map ROM into $C000-$CFFF
        sta     d030_save
        ora     #%00100000
        sta     $d030
        lda     $01                                     ; … I/O into $D000—$DFFF
        sta     mmu_save
        ora     #%00000101
        and     #%11111101
        sta     $1
        lda     #0                                      ; … bank 0 RAM into $0000 - $BFFF
        tax
        tay
        ldz     #%10000011                              ; … and kernel into $E000—$FFFF
        jsr     $038d
        map
        eom
        cli
        jsr     main
        sei
        lda     mmu_save                                ; Restore memory configuration
        sta     $01                                     ; … but DON'T restore the memory map!
        lda     d030_save                               ; … We're still executing, and the kernel
        sta     $d030                                   ; … will take care of that
        lda     #1                                      ; Resume calling BASIC in the kernel's IRQ handler
        tsb     $1104                                   ; … once the kernel clears the I flag
        ldx     sp_save                                 ; Restore stack pointer
        txs
        rts


d030_save:      !skip 1
mmu_save:       !skip 1
sp_save:        !skip 1
