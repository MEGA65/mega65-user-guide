# Termenblad voor de Nederlandse vertaling (MEGA65-handleiding)

Kernregels en woordenlijst voor de vertaling. De volledige stijlgids staat in
`dutch/styleguide.md` (lees die wanneer je twijfelt).

## Kernregels

- **U-vorm, formeel, zakelijk.** Spreek de lezer aan met "u". "Wij" alleen voor het MEGA65-team.
- **Actief schrijven** boven lijdende vorm.
- **één** (met accent) wanneer het aantal bedoeld wordt; **een** als lidwoord.
- **Aanhalingstekens**: behoud de TeX-vorm ``...'' in de vertaling (die rendert als "..." vanwege het lettertype). Leestekens (komma, punt) staan **buiten** de aanhalingstekens.
- **Zinshoofdletters in koppen** (alleen eerste woord + eigennamen), geen title-case.
- **Verkleinwoorden toevoegen** waar natuurlijk (muispijltje, programmaatje).
- **Positiewerkwoorden** toevoegen waar context dat vraagt (het venster gaat open, de cursor verschijnt).
- **Germanismen vermijden**; Germaanse variant heeft voorkeur (bladzijde i.p.v. pagina, bestand i.p.v. file, toepassing i.p.v. applicatie, aanvaarden i.p.v. accepteren).
- **Lidwoorden**: de computer, de SD-kaart, de core, de bitstream, de FPGA, de ROM, de cursor, de map, de instructie, de prompt (mannelijk; "de `READY.`-prompt"); het geheugen, het register, het programma, het scherm, het bestand, het hulpprogramma, het menu.
- **Geen komma vóór coördinerend "en"**: "U kunt het zelf proberen en u zult..." (niet "proberen, en u zult...").
- **Informele zinsopener vermijden**: begin een zin niet met "Maar", "Daarom" of "Daarmee" (Taalunie: "maar" aan zinsbegin is toegestaan, maar minder geschikt voor formele schrijftaal). Gebruik "echter" achter het eerste zinsdeel, "evenwel", of herschrijf de zin.
- **welke** in formele bijzinnen na een komma ("een syntaxisfout, welke u vertelt..."); "die/dat" in korte, onmisbare bijzinnen.
- **indien** in formele voorwaardelijke zinnen; "wanneer" voor tijdrelaties.
- **VVT van "vergeten"**: "was ... vergeten" (niet "had ... vergeten").
- **Decimale punt** blijft het scheidingsteken (2.05, niet 2,05).

## Wat NIET vertaald wordt

- Schermteksten en uitvoer (`READY.`, `SYNTAX ERROR`, `HELLO COMPUTER`), dus ook alles binnen `\screentextwide{...}` / `\screentext{...}` / `\stw{...}`.
- Code binnen `screencode`-blokken en `\verb`.
- Eigennamen en merken: MEGA65, BASIC 65, Commodore 64 (C64), KERNAL, VIC-II, VIC-III, VIC-IV, SID, SpritEd, Freezer, Hypervisor, Matrix Mode, Xemu.
- Toetsnamen in `\specialkey{...}` / `\megakey{...}` / `\widekey{...}` (RETURN, SHIFT, CTRL, enz.).
- BASIC-commando's en -functies: PRINT, GOSUB, POKE, PEEK, SYS, LOAD, RUN, LIST, INPUT, LET, IF, THEN, GOTO, FOR, NEXT, RND, CHR$, STR$, INT, enz.
- Afkortingen: DMA, FPGA, I/O, JTAG, IP, UART, VNC.
- Bestandsnamen, extensies en URL's.
- Vaste termen waar een Nederlands alternatief juist verwarrend is: core, chip RAM, colour RAM, bitstream, sprite, firmware, opcode, operand, PETSCII.

## Woordenlijst (vaktermen)

| Engels | Nederlands |
|---|---|
| address | adres |
| address space | adresruimte |
| accumulator register | accumulatorregister |
| addressing mode | adresseermethode |
| assembler | assembler (het programma); assembleren (werkwoord) |
| assembly language | assembleertaal |
| bank (memory bank) | bank / geheugenbank |
| bitmap | bitmap |
| border colour | randkleur |
| CRT (cathode ray tube) | beeldbuis (in samenstellingen: beeldbuismonitor) |
| byte | byte |
| carry flag | carry-vlag |
| character | teken |
| character set | tekenset |
| chip RAM | chip-RAM |
| cursor | cursor |
| control code | Controlcodes (de) (boekbrede term; "controlcode" enkelvoud) |
| cursor position | cursorpositie |
| device | apparaat |
| direct mode | directe modus |
| directory | map (in de interface) / directory (technisch) |
| disk | schijf |
| disk drive | schijfstation |
| disk image | schijfimage |
| display | beeldscherm (als apparaat); scherm (op het scherm) |
| DMA (Direct Memory Access) | DMA (uitschrijving blijft Engels) |
| draw state | tekenstatus |
| emulator | emulator (meervoud: emulatoren) |
| escape sequence | escape-sequentie (meervoud: escape-sequenties) |
| flashing | flashen |
| flash memory | flashgeheugen |
| floppy disk | diskette (niet "floppy-schijf"); diskettes (meervoud) |
| font | lettertype |
| FPGA | FPGA |
| full-size | op vol formaat (niet vertalen als "full-size") |
| glyph | glief / glyph |
| high byte | hoge byte |
| IEC serial bus | seriële IEC-bus (de Commodore-bus is een seriële variant van de IEC-bus, niet omgekeerd) |
| I/O register | I/O-register |
| interrupt | interrupt |
| loader | loader |
| machine code | machinetaal (standaardisatie: nooit "machinecode"; gesloten
  samenstellingen: machinetaalmonitor, machinetaalprogramma, machinetaal-
  instructie, machinetaalsubroutine, machinetaalroutines; "assembleertaal"
  blijft gereserveerd voor assembly-brontekst) |
| main board / mainboard | moederbord |
| managed drive | beheerd station |
| memory | geheugen |
| memory map | geheugenindeling (niet "geheugenkaart") |
| modifier key | modificatietoets |
| monitor | monitor (het programma) |
| network listening | netwerkluisteren |
| nibble | nibble |
| opcode | opcode |
| operand | operand |
| page | pagina (in geheugencontext) |
| palette | palet / kleurenpalet |
| peripheral (device) | randapparaat (niet "perifeer apparaat") |
| peripheral bus | randapparatuurbus |
| PETSCII | PETSCII |
| pixel | pixel |
| power light | stroomlampje |
| program | programma |
| prompt | prompt ("de `READY.`-prompt"; "prompt" is mannelijk) |
| raster interrupt | rasterinterrupt |
| register | register |
| ROM | ROM |
| screen | scherm |
| screen code | schermcode |
| screen editor | schermeditor |
| sprite | sprite |
| SD card | SD-kaart |
| supporter | steuner ("Supporters & Donors" → "Steuners & donateurs") |
| tokenise | tokeniseren |
| utility | hulpprogramma |
| zero page | zero page / nulpagina |

## Valse vrienden

| Engels | Vertaal NIET als | Correct |
|---|---|---|
| actually | actueel | eigenlijk, in feite |
| eventually | eventueel | uiteindelijk |
| control | controleren (verifiëren) | bedienen, besturen, aansturen |
| current | actueel | huidig, lopend |
| data | data (enkelvoud) | gegevens |
| disc | (wordt in de bron gebruikt voor optische schijf) | blijft "schijf" |
| library | bibliotheek | bibliotheek / programmabibliotheek |
| memory | herinnering | geheugen |
| sensible | sensibel | verstandig, redelijk |
| to address (a problem) | adresseren | aankaarten, behandelen |

## Keuzewoorden (kies consequent)

| Engels | Aanbevolen |
|---|---|
| application | toepassing |
| border | rand |
| directory / folder | map (in de interface); directory (technisch) |
| display | beeldscherm / scherm |
| file | bestand |
| flag (setting) | vlag |
| icon | pictogram |
| option | optie |
| setting | instelling |
| to download / upload | downloaden / uploaden |
| to key in | intypen |
| to load | laden / inladen |
| to run (a program) | uitvoeren |
| to save | opslaan |
| to switch off / on | uitschakelen / inschakelen (formeler); uitzetten / aanzetten (losser) |
| utility | hulpprogramma |
| window | venster |

## Specifieke begrippen uit het vertaalde hoofdstuk (beginninginbasic)
- **variable** → variabele (de)
- **numeric variable** → numerieke variabele
- **string variable** → stringvariabele
- **string** → string / tekenreeks (kies één; string is ingeburgerd)
- **keyword** → sleutelwoord / keyword (kies één; "sleutelwoord" heeft de voorkeur)
- **program listing** → programmalijst / listing
- **error message** → foutmelding
- **syntax error** → syntaxisfout (fout vanwege onjuiste syntaxis); de schermtekst `SYNTAX ERROR` blijft onvertaald
- **question mark character** → vraagteken
- **dollar sign character** → dollarteken
- **relational operator** → relationele operator (meervoud: operatoren, niet "operators")
- **less than / greater than** → kleiner dan / groter dan
- **loop** → lus
- **times tables** → tafels van vermenigvuldiging
- **random number** → willekeurig getal
- **guess the number** → raad het getal
- **screen background / border** → schermachtergrond / rand
- **to rub out** → uitwissen / wegpoetsen
- **to print** → afdrukken (op het scherm: weergeven, tonen; PRINT blijft de commandonaam)
- **to type** → typen / intypen
- **to press** (een toets) → indrukken / op ... drukken
- **to hold down** (een toets) → ingedrukt houden
- **to switch on** → inschakelen / aanzetten
- **signed integer** → geheel getal (gehele getallen zijn per definitie getekend; het woord "gesigneerd" wordt niet gebruikt in het Nederlands voor het teken van getallen)
- **unsigned integer** → natuurlijk getal (niet-negatieve gehele getallen)
- **two's complement** → tweecomplement
- **byte** → byte (woord voor woord behouden)
- **integer-array** → integer-array (type-naam, wordt niet vertaald)

## Kruisverwijzingen (sectietitels in andere hoofdstukken)

Titels die vanuit meerdere hoofdstukken geciteerd worden, moeten overal gelijk
vertaald worden. Afspraken tot nu toe:

| Engels (sectie) | Hoofdstuk | Nederlands |
|---|---|---|
| Introducing SD Cards | `configuring` | Kennismaken met SD-kaarten |
| Configuration Utility | `configuring` | Configuratiehulpprogramma |
| Control codes | `appendix-screeneditorkeys` | Controlcodes |
| Determining the Versions of Things | `cores-and-flashing` (sec:versions) | De versies van de onderdelen bepalen |
| Supporters & Donors | `appendix-donors` | Steuners & donateurs |

## Beslissingen (genomen, niet opnieuw ter discussie stellen)

- **Bijlage, niet aanhangsel.** Voor verwijzingen naar de appendixen wordt
  consequent "bijlage" gebruikt, omdat `\appendixname` in
  `dutch/common-header-dutch.tex` als "Bijlage" rendert ("zie bijlage \vref{...}").
- **Controlcodes** is de boekbrede term voor "control codes" (geen
  "Stuurcodes" of "besturingscodes"). "Controlcodes" is gedefinieerd in
  `gettingstarted` en is ook de sectietitel van `appendix-screeneditorkeys`.
  Zowel "besturingscodes" als "Stuurcodes" zijn gecorrigeerd naar
  "Controlcodes".
- **Kleurnamen appendix-system-palette:** de zestien C64-kleuren (rij 0-15)
  worden vertaald (Donkergrijs, Middengrijs, Lichtgroen, Lichtblauw,
  Lichtgrijs, enz.); de namen van de alternatieve set (rij 16-31, zoals
  "Guru Meditation", "Plum Sauce", "Hot Tamales") blijven bewust Engels
  (popcultuur-verwijzingen die een vertaling zouden verliezen).
- **Stroomlampje** is de vaste term voor "power light" (consequent in
  `transferring-files` en `using-disks`); niet vervangen.
- **Steuners** voor "Supporters" (hoofdstuktitel "Steuners & donateurs").
- **Modificatietoets** is de vaste term voor "modifier key" (gangbaar in de
  Nederlandse computerwereld).
- **Nederlandse namen voor boeken.** De Engelstalige boektitels worden in de
  vertaalde tekst consequent vertaald:

  | Engels | Nederlands |
  |---|---|
  | The Mega65 User's Guide | De Mega65-gebruikershandleiding |
  | The Mega65 BASIC 65 Reference | De Mega65 naslaggids BASIC65 |
  | The Mega65 Chipset Reference | De Mega65-chipset naslaggids |
  | The Mega65 Developer's Guide | De Mega65 programmeergids |
  | The Mega65 Complete Compendium | Alles over de Mega65 |
  | The Mega65 Book | Het grote Mega65-boek |

- **"Zoals" bij niet-restrictieve voorbeelden krijgt komma's.** In het Nederlands
 worden niet-restrictieve voorbeelden (die een bredere categorie illustreren)
 door komma's afgescheiden: "MEGA65-voorzieningen, zoals het Freezer-menu,
 zijn niet beschikbaar" (niet: "MEGA65-voorzieningen zoals het Freezer-menu
 zijn…"). Dit verschilt van het Engels, waar "such as" vaak zonder komma staat.
- **Floppy disk** → "diskette" (niet "floppy-schijf"; het woord "diskette" is
  ingeburgerd in het Nederlands). Zie woordenlijst.
- **Full-size** → "op vol formaat" (niet vertalen als "full-size").

- **Grenscontrole(n)** is de vaste term voor "bounds check(s)" / "screen bounds
  checks" (niet "grenzencontrole"): "Er wordt geen grenscontrole uitgevoerd …" /
  "Er worden geen grenscontroles (op de schermgrenzen) uitgevoerd".
- **Tekencel** voor "character cell" (niet "tekenveld"/"veld").
- **Paletbank** voor "palette bank" (en "ingemapte paletbank" voor
  "maped-in"; "in te mappen" voor "bank-in", niet "in te banken").
- **Attribuut** (en "attributenmodus", "tekstattributen", "…-attribuut") is de
  vaste term voor "attribute"; gebruik niet "kenmerken". "Onderstrepingsattribuut"
  is de afgeleide vorm van "onderstrepen".

## Assembly Reference (instructieset)

Richtlijnen voor de assembler-instructietabellen en -beschrijvingen
(`appendix-instructionset`, `instructionset-*`, `opcodetable-*`):

- **Instructie-mnemonics blijven Engels** (`ADC`, `LDA`, `STA`, `BRK`, …) en
  evenzo hun Engelse opschrift in de opschriftenregel van de tabel, bijv.
  `\bf{ADC : Add with carry}` → ook "Add with carry" blijft Engels. Pas dat
  opschrift NIET aan.
- **Instructie-beschrijvingen wél vertalen**: de lopende tekst die uitlegt wat
  de instructie doet ("This instruction adds the argument and the Carry Flag to
  the contents of the Accumulator.") wordt vrij in het Nederlands vertaald.
- **Side effects** → **Neveneffecten**. Bijv. "The N flag will be set if the
  result is negative…" → "De N-vlag wordt gezet als het resultaat negatief is…"
  De vlaggenletters (N, Z, I, C, D, V, E) blijven zoals gegeven.
- **Tabelkoppen wél vertalen** (binnen `\bf{...}`):
  Addressing Mode → Adresseermethode, Assembly → Assembly, Code → Code,
  Bytes → Bytes, Cycles → Cycles.
- **Voetnoot onder de tabel wél vertalen**: "Add one cycle if indexing crosses a
  page boundary." → "Voeg één cyclus toe als indexering een paginagrens
  overschrijdt."
- **Data-tabelrijen en opschriften zijn pass-segmenten**: die komen als zodanig
  in de opdracht (niet-vertaalbaar); de vertaler laat ze ongemoeid.

Woordenschat:

- **Accumulator Register** → accumulatorregister (vaste term).
- **carry flag** → carry-vlag.
- **immediate (8bit)** → immediate (8-bits) / direct (8-bits); kies "immediate"
  consequent en behoud de technische term.
- **zero-page** → zero page (technische term, zie woordenlijst).

Tekstverwerking / cryptografie (vastgesteld tijdens `text-processing`):

- **statement** → statement (het {\bf PRINT}-statement; gevestigd leenwoord in
  dit boek, niet vertalen).
- **string** → tekenreeks; vaste samenstelling conform woordenlijst:
  **string variable** → stringvariabele.
- **plaintext** → klare tekst (twee woorden; nooit "klartekst"); **ciphertext** →
  cijfertekst; **cipher** → cijfer
  (ROT13-cijfer, Atbasj-cijfer, Caesar-cijfer).
- **encrypt/decrypt** → versleutelen/ontsleutelen; **encryption/decryption
  function** → versleutelings-/ontsleutelingsfunctie (niet "versleutelfunctie").
  De termen *encryptie*/*decryptie* komen alleen voor waar de bron ze als
  begrip introduceert.
- **native** → native (onvertaald; vaste term in "native toetsenbordmatrix",
  "native-modus", "native resolutie").
- **scan code** → scancode; **data direction register** → gegevensrichtingsregister.

- **jump table** → sprongtabel (KERNAL-sprongtabel; vastgesteld bij de
  vertaling van `appendix-kernal-jumptable`).
