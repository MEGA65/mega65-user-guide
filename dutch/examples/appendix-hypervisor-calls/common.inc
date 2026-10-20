        * = $2001

        !16     +
!ifdef NOAUTOSTART {
        !16     5                                       ; Regelnr.
        !8      $80                                     ; END
        !8      0                                       ; Einde van BASIC-regel
+       !16     +
}
        !16     10                                      ; Regelnr.
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
        !8      0                                       ; Einde van BASIC-regel
+       !16     0                                       ; BASIC-eindmarkering


startup:
        sei
        tsx                                             ; Sla de systeemstackpointer op
        stx     sp_save
        lda     #1                                      ; Roep BASIC niet aan in de IRQ-verwerker van de kernel
        trb     $1104
        lda     $d030                                   ; Breng ROM in het geheugen $C000-$CFFF
        sta     d030_save
        ora     #%00100000
        sta     $d030
        lda     $01                                     ; … I/O in $D000—$DFFF
        sta     mmu_save
        ora     #%00000101
        and     #%11111101
        sta     $1
        lda     #0                                      ; … bank 0 RAM in $0000 - $BFFF
        tax
        tay
        ldz     #%10000011                              ; … en kernel in $E000—$FFFF
        jsr     $038d
        map
        eom
        cli
        jsr     main
        sei
        lda     mmu_save                                ; Herstel de geheugenconfiguratie
        sta     $01                                     ; … maar herstel de geheugenindeling NIET!
        lda     d030_save                               ; … We worden nog steeds uitgevoerd, en de kernel
        sta     $d030                                   ; … zal dat afhandelen
        lda     #1                                      ; Hervat het aanroepen van BASIC in de IRQ-verwerker van de kernel
        tsb     $1104                                   ; … zodra de kernel de I-vlag wist
        ldx     sp_save                                 ; Herstel de stapelpointer
        txs
        rts


d030_save:      !skip 1
mmu_save:       !skip 1
sp_save:        !skip 1
