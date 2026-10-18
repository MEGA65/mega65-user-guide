
Stijlgids voor de Nederlandse vertaling

Deze stijlgids is een leidraad voor het maken van de Nederlandse vertaling van de handleiding van de Mega65. Aanwijzingen in deze stijlgids hebben voorrang op de stijlgids in de hoofddirectory voor het Engelstalige origineel, doch deze "hoofdstijlgids" blijft leidend voor onderwerpen die niet in deze Nederlandse stijlgids worden behandeld.

Voorstellen en besluiten: een aantal onderdelen in deze gids zijn gemarkeerd als **voorstel**. Beslissingen over terminologie horen in de woordenlijst (hoofdstuk 7) thuis; wijzig de lijst niet stilzwijgend, maar neem wijzigingen op in het termenregister (hoofdstuk 8.3).

# Algemene principes

De doelgroep en de toon van de oorspronkelijke handleiding blijven overeind: hobbyisten, van wisselende technische kennis, aangesproken in zakelijk formeel Nederlands (zie hoofdstuk 4). Het vertaalde document moet net zo leesbaar, consistent en onderhoudbaar zijn als het origineel. Vertaal niet woord voor woord, maar breng de *betekenis* van het origineel over.

# Vertaalstrategie

## Vrije vertaling

We willen een vrije vertaling naar gebruikelijk Nederlands. Dus geen vertaling die zo brongetrouw mogelijk probeert te blijven. Een zin of zelfs hele alinea mag opnieuw geformuleerd worden.

Regels die daarbij horen:

- **Eerst de boodschap, dan de vorm.** Begrijp eerst wat de zin zegt; schrijf het vervolgens op zoals een Nederlandse schrijver het zou schrijven.
- **Doe geen moeite om grappen of woordspelingen te behouden** wanneer die in het Nederlands geen effect hebben. Laat ze weg of vervang ze door iets dat past.
- **Laat het origineel niet doorschemeren in constructies.** Vermijd Engelse zinsbouw ("de MEGA65 heeft..." in plaats van "the MEGA65 has..."; juiste Nederlandse volgorde en samentrekkingen gebruiken).
- **Wanneer de bron dubbelzinnig of onduidelijk is**, kies de meest waarschijnlijke lezing en voeg eventueel een voetnoot toe voor de vertaler/redacteur.

## Wat wij niet vertalen

Bepaalde elementen worden letterlijk overgenomen en **niet** vertaald:

- **Schermteksten en programmauitvoer**, zoals het `READY.`-prompt, het `OK.`-prompt, menu-regels die op het scherm verschijnen en BASIC-foutmeldingen. Dit is letterlijke uitvoer die de gebruiker op het scherm ziet; een vertaling zou de lezer in verwarring brengen. Zie ook 6.3.
- **Code-voorbeelden**: BASIC 65-, assembler- en C-listing worden letterlijk overgenomen. Keywords en namen van commando's worden nooit vertaald. Commentaar binnen code-voorbeelden mag vertaald worden, mits consequent in het hele boek.
- **Bestandsnamen en bestandsextensies**: `MEGA65.D81`, `.PRG`, `mega65_ftp` en dergelijke.
- **Eigennamen en merken**: zie 6.1.
- **Toetsnamen**: zie 6.2.

# Spelling

De Nederlandse vertaling wordt geschreven volgens de Witte Spelling. De Witte Spelling geeft op een aantal onderwerpen meer vrijheid in de spelling van woorden dan de Groene Spelling. Ook binnen de Groene Spelling bestaat op enkele punten vrijheid. We zullen een aantal keuzen maken voor de spelling van de handleiding.

## Meervoud op N heeft de voorkeur

Indien van een woord zowel een meervoud op S als een meervoud op N bestaat, verkiezen we de vorm met N:
- Keuzen, niet keuzes
- Typen, niet types
- Waarden, niet waardes
- Symbolen, niet symboles

S toch gebruiken bij:
- Registers (in de betekenis van registercellen; het meervoud van "register" is in het Nederlands echter gewoonlijk ook "registers")

## Meervoud bepaalt tussen-N

We kiezen voor de oude tussen-N-regel waarbij het getal van het eerste woord bepaalt of een tussen-N wordt geschreven:
- Driefasenaansluiting, niet driefaseaansluiting
- Koekepan, niet koekenpan
- Ideeënloos, niet ideeëloos

## Vernederlandste schrijfwijzen

De binnen de Witte Spelling toegestane vernederlandste spellingen zoals kado, paëlla en sjiek hebben de voorkeur boven etymologische spellingen als cadeau, paella en chique.

## Schrijfwijze van getallen

Het Nederlands gebruikt een komma als decimaal scheidingsteken. De Mega65 gebruikt, als Engelstalige computer, de decimale punt als scheidingsteken. Om de lezer niet onnodig te verwarren zullen we in de handleiding de decimale punt gebruiken als decimaal scheidingsteken. We zullen geen scheidingsteken toepassen voor duizendtallen, dit om te voorkomen dat de lezer dit scheidingsteken intikt.

We wijken hier dus af van de Nederlandse spelling!

- 2.05, niet 2,05.
- 893267856, niet 893,267,856.

In doorlopende tekst boven een bepaald getal (bijvoorbeeld "de 256 mogelijkheden van een byte") wordt het getal gewoon in woorden of cijfers geschreven volgens de regels in 4.13.

## Accenttekens: "één" versus "een"

In technische tekst is het accentteken op "één" van belang om verwarring te voorkomen:
- **één** wanneer het aantal bedoeld wordt: "één byte", "één van de acht cores"
- **een** wanneer het lidwoord bedoeld wordt: "een byte", "een van de acht cores"

Pas dit consequent toe op alle woorden waar weglating van het accent dubbelzinnig maakt. Schrijf ook "één" in "één en andermaal"-achtige samenstellingen waar nodig.

### Accenttekens op hoofdletters

Accenttekens (trema, accent aigu, etc.) worden ook op hoofdletters geplaatst waar
nodig. Bijvoorbeeld: NIET GE**Ï**MPLEMENTEERD, GE**Ï**NSPIREERD, GE**Ë**XPLIOTEERD.

Bron: de Taalunie staat dit toe; zie ook Onze Taal
(https://onzetaal.nl/taalloket/accenten-op-hoofdletters).

**Uitzondering:** het accent op een beginhoofdletter van een zelfstandig naamwoord
vervalt wanneer de woordvorm dat vereist (Taalunie-regel): schrijf bijv.
"Eén" (niet "Één") aan het begin van een zin, maar "één" in de loop
van de zin.

## Lidwoorden en geslacht

Bij leenwoorden en eigennamen moeten wij kiezen voor "de" of "het". Voorstel:

- **de**: de MEGA65, de computer, de SD-kaart, de bitstream, de FPGA, de ROM, de core, de cursor, de interface, de map, de directory, de instructie, de status, de configuratie, de optie, de fout, de kleur, de cursor-toets
- **het**: het geheugen, het register, het programma, het scherm, het beeldscherm, het bestand, het besturingssysteem, het bestandssysteem, het hulpprogramma, het menu, het invoerveld, het bestandstype, het RAM(-geheugen)

Een gemakkelijke vuistregel: kies het lidwoord dat het meest gangbaar is in Nederlandse computertijdschriften en handleidingen. Bij twijfel: "de" voor hardware-onderdelen en apparaten, "het" voor concepten, systemen en bestanden. Leg afwijkende keuzen vast in de woordenlijst.

## Samenstellingen en koppelteken

Nederlands schrijft samenstellingen over het algemeen aan elkaar. Schrijf Engelse woorden in samenstellingen met een koppelteken wanneer de combinatie anders onleesbaar wordt of een klinkerbotsing ontstaat:

- SD-kaart, niet sdkaart
- Freezer-menu, niet Freezer menu (het Engelse "Freezer menu" wordt een samenstelling in het Nederlands)
- I/O-register, niet IOregister
- hulpprogrammamenu (aan elkaar; het is een gewone Nederlandse samenstelling)

Bij eigennamen en initialismen (MEGA65, I/O, VIC) altijd een koppelteken in de samenstelling: "MEGA65-computer", "VIC-IV-chip", "I/O-register".

# Schrijfstijl

## U-vorm en formele schrijftaal

Spreek de lezer met u aan en schrijf zakelijk formeel. Gebruik formele schrijftaal:
- welke/hetwelk in plaats van die/dat. "De computer, welke..." NIET: "De computer, die..."
- derhalve in plaats van daarom. "De computer zal derhalve..."
- Gebruik in beginsel "maar" om een tegenstelling aan te duiden, maar indien een zin een grote hoeveelheid komma's zou krijgen, gebruik dan een constructie met "evenwel".
- Geen komma vóór coördinerend "en": "U kunt het zelf proberen en u zult..." NIET: "U kunt het zelf proberen, en u zult..."
- Begin een zin niet met "Maar", "Daarom" of "Daarmee". Volgens de Nederlandse
  Taalunie (taaladvies.net) is "maar" aan het begin van een zin toegestaan, maar
  minder geschikt voor zakelijke schrijftaal. Gebruik in plaats daarvan "echter"
  (achter het eerste zinsdeel), "evenwel" of herschrijf de zin:
  "We kunnen de computer echter ook een lijst van commando's laten onthouden."
  NIET: "Maar we kunnen de computer ook een lijst van commando's laten onthouden."
- "indien" voor voorwaardelijke zinnen in formele schrijftaal: "Indien u dat doet, ziet u..." (informeler: "Wanneer u dat doet, ..."; "wanneer" blijft de voorkeur voor tijdrelaties).
- NIET: Het programma, dat u zojuist geschreven heeft, wordt uitgevoerd, maar, omdat een deling door nul optreedt, breekt het af in regel 50.
- WEL: Het programma dat u zojuist geschreven heeft, wordt uitgevoerd. Het zal evenwel afgebroken worden in regel 50, omdat een deling door nul optreedt.

Let op: in dit voorbeeld wordt "dat" gebruikt, terwijl de regel hierboven "welke/hetwelk" voorschrijft. De regel voor "welke/hetwelk" geldt vooral voor *beperkende* bijzinnen die verwarring kunnen opleveren; in eenvoudige, korte bijzinnen is "die/dat" natuurlijker Nederlands. Gebruik "die/dat" wanneer de bijzin onmisbaar is en geen leesverwarring geeft; gebruik "welke/hetwelk" wanneer een extra komma nodig is of wanneer het antecedent een voorzetselconstituent is.

## Wij-vorm

De auteur spreekt vanuit de "wij-vorm": "Wij raden u aan...", niet "Ik raad u aan...". "Wij" verwijst naar het MEGA65-team (of de auteurs), niet naar de lezer. Zie ook 4.15 (de lezer is "u").

## Voorzichtige formuleringen

Gebruik voorzichtige formuleringen "het zou kunnen voorkomen dat...": De auteurs spreken als ingenieurs die beperkingen erkennen, het is geen marketingtaal.

Voorbeeld:
- "Het zou kunnen voorkomen dat de verbinding verbroken wordt."
- "In sommige situaties kan dit tot een fout leiden."

## De computer als actieve entiteit

Gebruik een persoonsverbeelding van de computer: "de computer wacht", "de computer rekent voor u uit". De computer als actieve entiteit. Zie ook het positiewerkwoorden (4.9) en de werkwoordstijden (4.11).

## De stijl moet informeren

De stijl moet informeren, niet verkopen of overtuigen. Het is zakelijk, formeel en informatief: geen marketingtaal, geen overbodige superlatieven, geen ironie die de lezer in verwarring brengt. De lezer moet zich kunnen concentreren op de inhoud.

## Germaanse versus Romaanse woorden

Het Nederlands heeft in sommige opzichten een dubbel woordenboek: Van veel woorden bestaat zowel een variant van Germaanse als Romaanse oorsprong. Het Engels kent vaak maar één variant en de vertaler zal moeten kiezen.

Indien beide vormen gangbaar zijn verkiezen we de Germaanse variant.

- Bladzijde, niet pagina.
- Aanhangsel, niet appendix
- Inhoudsopgave, niet index
- Oorsprong, niet origine.
- Aankomen, niet arriveren
- Beginsel, niet principe
- Oorspronkelijk, niet origineel
- Volstrekt, niet absoluut
- Plotseling, niet abrupt
- Verbeteren, niet corrigeren
- Bestand, niet file (in de betekenis van gegevensbestand)
- Toepassing, niet applicatie
- Instelling, niet setting
- Weergeven, niet displayen
- Geheugen, niet memory
- Aanvaarden, niet accepteren

In een aantal gevallen is de Romaanse variant zo gebruikelijk dat de Germaanse variant raar kan klinken, in deze gevallen verkiezen we de Romaanse variant:
- Alinea, niet paragraaf
- Conclusie, niet gevolgtrekking
- Configuratie, niet instellingstructuur
- Terminal, niet eindstation

## Lengte van zinnen

Het Nederlands heeft meer mogelijkheden voor het maken van lange zinnen dan het Engels. Onderzoek of losse zinnen gecombineerd kunnen worden tot een enkele zin. Indien een enkele zin de relatie tussen beide deelzinnen verduidelijkt, heeft de enkele zin de voorkeur.

## Verkleinwoorden

Het Engels kent geen verkleinwoorden, bij vertaling naar het Nederlands zal de vertaler deze creatief moeten toevoegen. Woorden als "small" en "little" in een Engelse zin zijn aanwijzingen dat in de Nederlandse vertaling een verkleinwoord gebruikt kan worden, maar dit is niet voldoende.

Onderzoek of de context informatie biedt of een verkleinwoord gebruikt kan worden: Indien de handleiding een programma van een paar regels bespreekt is "programmaatje" wellicht een betere aanduiding dan "programma".

Kijk ook naar de gewoonte in de Nederlandse taal: "mouse pointer" vertalen met "muispijltje", omdat het gebruikelijk is voor dit woord om een verkleinwoord te gebruiken.

## Positiewerkwoorden

Het Engels kent geen positiewerkwoorden, bij vertaling naar het Nederlands zal de vertaler deze creatief moeten toevoegen. Zoek of de context informatie geeft over de positie van een voorwerp. Zo niet, gebruik de positie die voor dat voorwerp het meest gebruikelijk is.

Voorbeelden:
- "The book is on the table" → "Het boek ligt op tafel." (liggen is gebruikelijke positie)
- "The window opens" → "Het venster gaat open" (geen "staat open", tenzij de toestand bedoeld is)
- "The screen is blank" → "Het scherm blijft leeg"
- "The cursor appears" → "De cursor verschijnt" of "De cursor komt te staan"

Kies tussen een *toestand* (staan, liggen, hangen, zitten) en een *overgang* (gaan staan, verschijnen, opkomen) aan de hand van de context.

## Actief schrijven; vermijd de lijdende vorm

In formele zakelijke tekst bestaat de neiging de lijdende vorm te gebruiken. Wij geven de voorkeur aan actieve zinnen wanneer dat de leesbaarheid verbetert:

- NIET: "Het bestand wordt door het programma opgeslagen."
- WEL: "Het programma slaat het bestand op."

De lijdende vorm blijft toegestaan wanneer het onderwerp onbekend of onbelangrijk is: "Het bestand wordt opgeslagen wanneer de computer wordt uitgeschakeld."

## Werkwoordstijden

Gebruik standaard Nederlandse werkwoordstijden. Net als in het Engelstalige origineel geldt:

- **Tegenwoordige tijd** voor de mogelijkheden van (vintage) computers. Vintage computers worden nog steeds door hobbyisten gebruikt en behouden hun technische eigenschappen: "De Commodore 64 heeft 64 kilobyte geheugen." (niet "had")
- **Verleden tijd** voor gebeurtenissen in het verleden: "De Commodore 64 werd in 1982 uitgebracht door Commodore International."
- De voltooid verleden tijd van "vergeten" gebruikt "was", niet "had": "De computer was de invoerregel alweer vergeten." NIET: "De computer had de invoerregel alweer vergeten."

## Moeten, kunnen, hoeven

Wees precies met modale werkwoorden, want zij drukken verplichting en mogelijkheid uit:

- **moeten**: verplichting of noodzaak. "U moet de computer uitschakelen voordat u de kaart vervangt."
- **kunnen**: mogelijkheid of vaardigheid. "U kunt de kleur aanpassen."
- **hoeven**: afwezigheid van noodzaak. "U hoeft het bestand niet eerst te verwijderen." (niet "U moet niet...")

De Engelse bron gebruikt "can", "may" en "must"; vertaal "must" consequent met "moeten" en "can" met "kunnen" (zie 4.15).

## Getallen in doorlopende tekst

- Spreek aantallen kleiner dan tien uit: "Houd de toets één seconde ingedrukt." "De computer piept drie keer."
- Voor hoeveelheden: cijfer + eenheid, zie 3.4.
- De decimale punt is het scheidingsteken (zie 3.4); in doorlopende tekst mag een bedrag als "2.05" geschreven worden.

## De lezer en de auteurs

- De lezer is "u". Zie 4.1.
- "Wij" alleen voor het MEGA65-team of de auteurs, en spaarzaam: "Wij raden u aan de nieuwste firmware te installeren."
- Voor een optionele activiteit van de lezer: "kan", niet "zou kunnen": "U kunt de kleur aanpassen." "Zou kunnen" drukt onzekerheid uit; gebruik het alleen voor mogelijke, ongewisse gebeurtenissen (zie 4.3).
- Vermijd "moeten" ten opzichte van de lezer wanneer het een advies is: gebruik de gebiedende wijs ("Schakel de computer in") of een suggestie ("U kunt de computer inschakelen").

# Typografie en leestekens

## Aanhalingstekens

De bron gebruikt in doorlopende tekst de klassieke TeX-aanhalingstekens (``...''). **Besluit (PoC, vanwege het lettertype):** het hoofdlettertype MegaGlacial bevat het Nederlandse openingsteken „ (U+201E) niet, en geen van de in de repo aanwezige fonts biedt het behalve Inconsolata (het code-lettertype). Daarom behouden wij in de Nederlandse vertaling de klassieke TeX-vorm `` ... '', die XeTeX als de ronde aanhalingstekens " ... " (U+201C/U+201D) weergeeft. Dit is tevens visueel consistent met het Engelstalige origineel.

- De computer ``welke ... heet'', ...
- Bij een citaat binnen een citaat: wissel naar `'...'`.

De regels voor de Nederlandse interpunctie (leestekens buiten het aanhalingsteken, zie 5.2) blijven ongewijzigd.

Uitzonderingen:
- **Schermteksten en code** gebruiken te allen tijde de ASCII-dubbele aanhalingstekens (") zoals in de bron, omdat dit letterlijke uitvoer is: `LOAD "*",8,1`
- **Als het lettertype ooit een font met „ krijgt**, kunnen wij terugschakelen naar de letterlijke Nederlandse vorm „..."; pas dan ook dit hoofdstuk aan. Het teken „ (U+201E) mag in ieder geval niet als letterlijk teken in de TeX-bron staan zolang het font het niet heeft.

## Leestekens rond aanhalingstekens

In het Nederlands staan leestekens zoals komma en punt **buiten** het aanhalingsteken, anders dan in het Engels:

- NIET: ``Houd de toets ingedrukt," zei de handleiding.
- WEL: ``Houd de toets ingedrukt'', zei de handleiding.

Uitzondering: wanneer het aanhalingsteken zelf een volledige zin is die eindigt met een punt, komt de punt binnen het aanhalingsteken te staan.

## Hoofdletters in koppen

In het Engels gebruiken de koppen title-case (elk belangrijk woord met hoofdletter). Het Nederlands kent geen title-case: schrijf koppen in zinshoofdletters, dus alleen het eerste woord en eigennamen met een hoofdletter:

- NIET: "De VIC-IV-Chip en Zijn Registers"
- WEL: "De VIC-IV-chip en zijn registers"

Eigennamen in koppen behouden hun eigen hoofdletters: "BASIC 65", "KERNAL", "MEGA65".

## Afkortingen

- Vertaal de Engelse afkortingen: "e.g." → "bijv."; "i.e." → "d.w.z."; "etc." → "enz."; "e.g." in een nieuwe zin → "Bijvoorbeeld".
- Schrijf afkortingen met punten: "bijv.", "d.w.z.", "enz.".
- Laat afkortingen die geen Nederlandse tegenhanger hebben (zoals "e.g.") niet in het Nederlands achter. Gebruik de Nederlandse vorm.
- "I/O" blijft "I/O". "PC" blijft "PC". Leg afwijkende keuzen vast in de woordenlijst.
- Bij het eerste gebruik van een afkorting: schrijf de afkorting uit en zet de afkorting tussen haakjes, zoals in de bron: "Direct Memory Access (DMA)". Vertaal alleen de uitgeschreven vorm: "Directe Geheugentoegang (DMA)" of behoud de Engelse uitschrijving? Behoud de Engelse uitschrijving wanneer de afkorting in het Nederlands hetzelfde blijft (DMA, FPGA, I/O, MAC, IP). Vertaal de uitschrijving alleen wanneer het een zuiver Nederlandse term betreft (bijv. "voeding").

## Bezitsvorm

Het Nederlands kent geen apostrof-s zoals het Engels ("the MEGA65's CPU"). Gebruik:

- "de CPU van de MEGA65", niet "de MEGA65's CPU"
- "de toetsenbordindeling van de MEGA65"

# Termen en naamgeving

## Vaste namen die niet vertaald worden

Deze namen blijven ongewijzigd (hoofdlettergebruik overnemen uit de bron):

- **Eigennamen en merken**: MEGA65, BASIC 65, Commodore 64 (C64), Commodore 128 (C128), KERNAL, VIC-II, VIC-III, VIC-IV, SID, SpritEd, Freezer, Hypervisor, Matrix Mode, Xemu, M65Connect, Filehost, SD2IEC, Acme
- **Product- en formaatnamen**: D64, D81, PRG, FAT32, PDF, PNG, JPEG, GIF
- **Registernamen**: BORDERCOL, MAP, MAPH, MAPL, VFAST, CB, en alle namen in de registertabellen
- **Instructienamen** (assembleertaal): LDA, STQ, MAP, EOM, AUG enz. — deze komen ook in doorlopende tekst voor en blijven onvertaald in hoofdletters
- **BASIC-commando's en -functies**: PRINT, GOSUB, POKE, PEEK, SYS, MOUNT, DLOAD enz.
- **Afkortingen**: DMA, FPGA, I/O, JTAG, MAC, IP, UDP, IRQ, NMI, QSPI, UART, VNC, CRT, RGB
- **Monitornamen en -commando's**: MONITOR, STEP, TRACE, WATCHPOINT, BREAKPOINT enz.

Vertaal dus "Raster Rewrite Buffer (RRB)" niet als "Rasterherschrijfbuffer"; behoud de afkorting en Engelse uitschrijving zoals in de bron, of leg een uitzondering vast.

In dien een term uit meerdere Engelse woorden bestaat en dit samengesteld wordt met een Nederlands woord, wordt de groep Engelse woorden tussen aanhalingstekens geplaatst en de samenstelling met een streepje aangegeven:
- Het voorgaande "Raster Rewrite Buffer"-voorbeeld.

## Toetsnamen

Toetsnamen worden niet vertaald, omdat zij op het toetsenbord staan. Gebruik de namen uit de bron, in hoofdletters, via de LaTeX-commando's `\specialkey`, `\megakey`, `\widekey` enz.:

- RETURN, SHIFT, CTRL, ALT, ESC, RESTORE, TAB, HELP
- Samengestelde toetsen: RUN/STOP, CLR/HOME, INST/DEL, NO/SCROLL, CAPS/LOCK, SHIFT/LOCK
- Cursor-toetsen: pijltjes (←, →, ↑, ↓)

Eenmalig bij het eerste gebruik kan de Nederlandse aanduiding tussen haakjes gegeven worden, bijv. "de `RETURN`-toets (Enter)". Voer dit echter consequent door in het hele boek.

## Schermteksten

Alles wat de lezer op het scherm ziet, blijft onvertaald:

- `READY.` en `OK.`-prompts (het lidwoord is "de": "de `READY.`-prompt"; "prompt" is mannelijk)
- Menu-regels en dialoogteksten die in het origineel als schermtekst getoond worden, bijv. `- INTERNAL 3.5" -`, `UNIT #`
- De typfout-opdracht `DELETE EVERYTHING`
- BASIC-foutmeldingen zoals Division By Zero Error

In doorlopende tekst wordt een BASIC-fout *omschreven* in het Nederlands ("Deze handleiding ... eindigt met een "Division By Zero"-fout?). Zie de Engelse stijlgids onder "BASIC errors": schrijf het type met beginhoofdletters gevolgd door het woord "error" (in het Nederlands: "fout"): *Voorstel:* "een Division By Zero"-fout. De letterlijke weergave, zoals `?DIVISION BY ZERO ERROR`, hoort in een `screencode`-blok.

## Code en bestandsnamen

- Code-voorbeelden (BASIC, assembler, C) blijven letterlijk. Keywords en identifiers worden niet vertaald.
- Commentaarregels binnen code kunnen vertaald worden; doe dit consequent en houd het kort.
- Strings binnen programmavoorbeelden kunnen eveneens vertaald worden: PRINT"HELLO WORLD" → PRINT"HALLO WERELD"
- Bestandsnamen, extensies en commando's blijven onvertaald: `MEGA65.D81`, `.PRG`, `mega65_ftp -e -c 'put file.d81' -c 'exit'`.

# Woordenlijst

Deze woordenlijst is een levend document. Termen worden alfabetisch binnen hun categorie gerangschikt. Elke term geeft de aanbevolen Nederlandse vertaling en, waar nuttig, een voorbeeldzin uit de bron. Categorieën:

- **Vaktermen**: termen met een vaste, technische betekenis; hier bestaat één duidelijke vertaling (of een bewuste keuze).
- **Valse vrienden**: woorden die op een Nederlands woord lijken maar anders vertaald moeten worden.
- **Keuzewoorden**: woorden waarvoor meerdere Nederlandse vertalingen gangbaar zijn; hier moet gekozen worden.

## 7.1 Vaktermen

| Engels | Nederlands (voorstel) | Toelichting / voorbeeld |
|---|---|---|
| address | adres | "Elke cel heeft een numeriek adres." |
| address space | adresruimte | "De 28-bits adressen vormen een adresruimte in het bereik 000.0000 -- FFF.FFFF." |
| addressing mode | adresseermode | Gangbaar binnen de Nederlandse informatica |
| assembler | assembler | Gangbare Nederlandse vakterm; "assembleren" voor het werkwoord. |
| assembly language | assembleertaal |  |
| bank (memory bank) | bank / geheugenbank | "Het adres 3.FDA8 is een adres in bank $3." |
| bitmap | bitmap | Blijft. |
| bitstream | bitstream | Blijft; vertaling "bitstroom" is ongebruikelijk. |
| border colour | randkleur | "border" → "rand"; "the screen's border colour" → "de randkleur van het scherm". |
| byte | byte | Blijft. |
| character | teken | Voorkeur voor "teken"; "karakter" gereserveerd voor de betekenis "persoonlijkheid". "A character most often represents a single, primitive element of printable text". |
| character set | tekenset | "Graphics based on character sets". |
| character ROM | character-ROM | Het ROM waarin het patroon van tekens opgeslagen is; zie "character" → "teken". |
| chip RAM | chip-RAM | Blijft (eigennaam-achtig; vastgelegd in de bron). |
| colour RAM | kleurengeheugen (colour-RAM) | Kleurengeheugen is goed bruikbaar, maar vermeld bij een eerste gebruik de Engelse benaming tussen haakjes vanwege de herkenbaarheid: het woord wordt ook onvertaald gebruikt in het Nederlands. |
| core | core | Blijft. "kern" is geen optie: verwarrend met "kernel" en "KERNAL". |
| core selection menu | corekeuzemenu | Kandidaat. Alternatief: "keuzemenu voor cores". |
| cursor | cursor | Blijft. "muisaanwijzer" alleen voor de muiscursor. |
| device | apparaat | "Toestel" en "apparaat" zijn beide correct; kies "apparaat". Uitzondering: Gebruik "device" indien gesproken wordt over een logisch device op de seriële bus met een devicenummer.|
| direct mode | directe modus | Volgt de Engelse stijlgids ("direct mode", niet "immediate mode"). |
| directory | directory | Zie 7.3 Keuzewoorden. |
| disk | schijf | Magnetisch medium. Zie ook "disc" bij 7.2. |
| disk drive | schijfstation | Kandidaat. "diskettestation" alleen voor floppy. |
| disk image | schijfimage / diskimage | Beide komen voor; kies één vorm. |
| display (noun) | beeldscherm | Zie 7.3. |
| DMA (Direct Memory Access) | DMA (Direct Memory Access) | Afkorting blijft; de uitschrijving blijft Engels (zie 5.4). |
| emulator | emulator | Blijft. |
| firmware | firmware | Blijft. |
| flashing | flashen | "a process known as flashing" → "een proces dat flashen heet". Het werkwoord "flashen" is ingeburgerd. |
| font | lettertype | Germanic voorkeur. "font" mag in technische context. |
| FPGA | FPGA | Blijft. |
| Freezer menu | Freezer-menu | "Freezer" blijft; koppelteken in de samenstelling. |
| glyph | glief | Kandidaat; "glyph" is ook gangbaar in typografie. |
| high byte | hoge byte | "het meest significante byte" is de letterlijke betekenis; houd "hoge byte" aan zoals de bron "high byte" gebruikt. Zie ook "low byte". |
| I/O register | I/O-register | Blijft; koppelteken. |
| input/output | invoer/uitvoer | "I/O" blijft als afkorting. |
| interrupt | interrupt | Blijft (gangbare Nederlandse vakterm). |
| joystick | joystick | Blijft. |
| loader | loader | Blijft; "lader" is ongebruikelijk in deze betekenis. |
| machine code | machinecode | Aaneenschrijven. |
| machine language | machinetaal | Historische synoniem (zie Engelse stijlgids). |
| memory | geheugen | Niet "herinnering" (zie 7.2). |
| memory map | geheugenindeling | **Let op**: "geheugenkaart" betekent in het Nederlands een geheugenkaartje (memory card) en is dus een valse vriend. Gebruik "geheugenindeling" of behoud "memory map". |
| mnemonic | mnemoniek | Gangbaar in de assemblercontext: "de mnemoniek van een instructie". |
| monitor | monitor | Blijft voor het programma. Voor het beeldscherm: zie "display". |
| nibble | nibble | Blijft (zie Engelse stijlgids). |
| opcode | opcode | Blijft. |
| operand | operand | Blijft. |
| page | pagina | Uitsluitend in geheugencontext: "pagina", anders "bladzijde". |
| palette | palet | "colour palette" → "kleurenpalet". |
| PETSCII | PETSCII | Blijft. |
| pixel | pixel | Blijft. |
| prompt | prompt | "the READY. prompt" → "de `READY.`-prompt". "Prompt" is mannelijk (Witte en Groene Boekje): "de prompt". |
| program | programma | Blijft; "programme" bestaat niet in het Nederlands. |
| raster interrupt | rasterinterrupt | Aaneenschrijven. |
| register | register | Blijft; "het register". |
| ROM | ROM | Blijft. |
| screen | scherm | "op het scherm". |
| screen code | schermcode | Kandidaat. |
| screen editor | schermeditor | Kandidaat. |
| sprite | sprite | Blijft. |
| SD card | SD-kaart | Koppelteken. |
| tokenise | tokeniseren | Kandidaat. "The BASIC program is tokenised" → "het programma wordt getokeniseerd". |
| utility | hulpprogramma | Germanic voorkeur; zie 7.3. |
| Utility menu | hulpprogrammamenu | Zie 7.3. |
| zero page | zero page | Blijft in 6502-literatuur; "nulpagina" is een kandidaat. |

## 7.2 Valse vrienden

| Engels | Vertaling (niet) | Correct | Voorbeeld uit de bron |
|---|---|---|---|
| actually | actueel | eigenlijk, in feite | "GO64 mode is actually just a temporary re-configuration of the MEGA65." → "De GO64-modus is eigenlijk slechts een tijdelijke herconfiguratie van de MEGA65." |
| eventually | eventueel | uiteindelijk, tenslotte | "eventually you will want to run programs written by others" → "uiteindelijk wilt u programma's uitvoeren die door anderen zijn geschreven". |
| control (verb) | controleren (in de zin van verifiëren) | bedienen, besturen, aansturen | "to control the machine" → "de computer bedienen". "controleren" = nagaan/verifiëren. |
| current | actueel | huidig, lopend | "the current working directory" → "de huidige werkmap". |
| data | data (als enkelvoud) | gegevens | "data stored in its memory" → "gegevens die in het geheugen zijn opgeslagen". |
| disc | disk | (optische) schijf | "disc" is optisch (cd/dvd); "disk" is magnetisch. In de bron: "compact disc". |
| library | bibliotheek (gebouw) | bibliotheek / programmabibliotheek | "the ncurses library" → "de ncurses-bibliotheek". In programmeercontext is "bibliotheek" correct. |
| memory | herinnering | geheugen | "computer memory" → "het geheugen van de computer". |
| monitor | beeldscherm | monitor (programma) | "the MEGA65 monitor" blijft "de monitor". "display" vertaalt u als "beeldscherm". |
| sensible | sensibel | verstandig, redelijk | "any sensible use of the MAP instruction" → "elk redelijk gebruik van de MAP-instructie". |
| simulate | simuleren (nabootsen) | simuleren | Is geen valse vriend, maar: "emulate" en "simulate" uit elkaar houden: emuleren = nabootsen van hardware, simuleren = nabootsen van gedrag. |
| to address (a problem) | adresseren | aankaarten, behandelen | "to address a problem" → "een probleem aankaarten". "adresseren" = van een adres voorzien. |

## 7.3 Keuzewoorden

| Engels | Kandidaten | Aanbeveling (voorstel) |
|---|---|---|
| application | toepassing / applicatie | toepassing (Germanic), tenzij in vaste termen als "applicatie" |
| border | rand / kader / rand van het scherm | rand |
| directory | directory / map | **map** in UI-context ("folders" → "mappen"); **directory** in technische beschrijvingen; schrijf "sub-directory" → "subdirectory" |
| display (noun) | beeldscherm / scherm / display | beeldscherm (als zelfstandig apparaat), scherm (in "op het scherm") |
| file | bestand / file | bestand |
| folder | map / folder | map |
| flag (setting) | vlag / markering | vlag |
| icon | pictogram / icoon | pictogram |
| menu | menu / keuzemenu | menu |
| option | optie / mogelijkheid | optie |
| setting | instelling / setting | instelling |
| to download / upload | downloaden/uploaden / ophalen/verzenden | downloaden/uploaden (ingeburgerd) |
| to key in | intoetsen / intypen / typen | intypen |
| to load | laden / inladen | laden |
| to run (a program) | uitvoeren / draaien | uitvoeren |
| to save | opslaan / bewaren | opslaan |
| to switch off / on | uitschakelen/inschakelen / uitzetten/aanzetten | uitschakelen/inschakelen (formeel), uitzetten/aanzetten in losse, informele passages; houd één vorm per passage |
| utility | hulpprogramma / utility | hulpprogramma; "Configuration Utility" → "configuratiehulpprogramma", "SD Card Utility" → "SD-kaarthulpprogramma", "Utility menu" → "hulpprogrammamenu" |
| window | venster / scherm / window | venster |

# Proces en werkwijze

## Status per hoofdstuk

Een vertaling van deze omvang moet bijgehouden worden. *Voorstel:* houd in een apart statusbestand per hoofdstuk bij:

- **vertaald**: de eerste vertaalslag is klaar
- **nagelezen**: een tweede lezer heeft de tekst gecontroleerd op stijl, leesbaarheid en terminologie
- **technisch gecontroleerd**: iemand met technische kennis heeft gecontroleerd dat adressen, registers en commando's correct zijn overgenomen

## Review-checklist

Gebruik bij het nalazen van een hoofdstuk deze checklist:

- [ ] Zijn alle schermteksten en code-voorbeelden letterlijk overgenomen? (6.3, 6.4)
- [ ] Zijn er Engelse woorden achtergebleven die vertaald hadden moeten worden?
- [ ] Komen er valse vrienden voor? (7.2)
- [ ] Staat "één" met accent waar het aantal bedoeld wordt? (3.5)
- [ ] Zijn de aanhalingstekens in de TeX-vorm ``...'' toegepast (rendert als "..." in verband met het lettertype), met leestekens erbuiten? (5.1, 5.2)
- [ ] Volgen de koppen de zinshoofdletters? (5.3)
- [ ] Zijn de lidwoorden consistent (de/het)? (3.6)
- [ ] Is de lezer consequent "u" en de auteur "wij"? (4.1, 4.15)
- [ ] Worden "moeten", "kunnen" en "hoeven" op de juiste wijze gebruikt? (4.12)
- [ ] Zijn nieuwe termen in de woordenlijst opgenomen? (8.3)

## Termenbeheer

- De woordenlijst (hoofdstuk 7) is leidend. Nieuwe termen worden door de redactie toegevoegd; voeg altijd een datum en een korte toelichting toe.
- Gebruik dezelfde term consequent in het hele boek; vermijd synoniemen voor eenzelfde begrip.
- Wanneer een term in het hoofdstuk zelf anders moet (bijvoorbeeld in een citaat), markeer dat dan.
- Termen die niet in de lijst staan, mogen tijdelijk met de gangbare Nederlandse vertaling worden geschreven, maar moeten worden gemeld zodat ze besproken kunnen worden.

# LaTeX en het bouwen van de vertaling

De vertaling wordt gebouwd met dezelfde gereedschappen als het origineel (latexmk/xelatex, Makefile). Aanwijzingen:

- Voeg voor het Nederlandse document `\usepackage[dutch]{babel}` toe (of stel de taal via de babel-opties in), zodat de Nederlandse afbreekpatronen en leestekens gebruikt worden. Lange Nederlandse samenstellingen breken anders onjuist af.
- Controleer na elke build de afbreekwaarschuwingen (`Overfull \hbox`) in de `.log`; voeg waar nodig `\hyphenation{...}`-regels toe voor lange samenstellingen.
- Gebruik voor aanhalingstekens in doorlopende tekst de klassieke TeX-vorm ``...'' (rendert via de TeX-ligaturen als "..."; het font mist het teken „, zie 5.1). Gebruik geen letterlijk „ in de TeX-bron.
- Houd de vertaling in een eigen map (bijv. `dutch/`) met dezelfde bestandsnamen en dezelfde hoofdstructuur, zodat bestanden een-op-een te vergelijken zijn met het origineel.
- Zorg dat `make all` (of het equivalent voor de vertaling) foutloos doorloopt; bouw regelmatig tussentijds.
