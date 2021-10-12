# MEGA65 Style Guide

In order for the suite of MEGA65 books to be easier to read and understand,
it's important that the English in the manuals is well-written and
consistent. To keep the MEGA65 style guide short, the team has chosen
to rely on the [BBC News Style Guide](https://www.bbc.co.uk/newsstyleguide/)
as much as possible for the basics. For computer specific terminology and spelling, the
[Microsoft Style Guide](https://docs.microsoft.com/en-us/style-guide/welcome/) was used. And finally,
the [Commodore 64 User Manual](http://www.zimmers.net/anonftp/pub/cbm/c64/manuals/C64_User_Manual_1984_2nd_Edition.pdf) 
was used, as we're trying to replicate the look and feel of it.

The intended audience for people reading (and contributing to) MEGA65 literature are people who are comfortable using 
a computer, and have good English skills.

If you're thinking of contributing, you should be comfortable writing about electronics and computing in general.

When writing content for the MEGA65 books, the intended audience in terms of readers are people who know their way 
around a computer, but don't assume they know how to program a computer or know how/why certain things work. The 
manuals should be as user-friendly as possible, but also be
[concise](https://docs.microsoft.com/en-us/style-guide/word-choice/use-simple-words-concise-sentences).

## Rules
Below are some simple MEGA65 styling rules:

* First and foremost, 
  [British English spelling](https://www.bbc.co.uk/newsstyleguide/grammar-spelling-punctuation#spelling) is used.
    * Organise, not organize.
    * Colours, not colors.

* **Clean as you go!** If you're changing certain properties/using commands of LaTeX to have your tables/images/text 
  appear in a particular way, remember to reset anything you have changed back to the default. These especially include
  `setlength`.  

* If you're stuck on when to use commas and hyphens refer to the 
  [Microsoft style guide](https://docs.microsoft.com/en-us/style-guide/punctuation/commas).

* You, not we. When describing steps a user can make in order to perform try to use
  the word "you", instead of "we". When describing the intention(s) of the MEGA65 team,
  we can be used. Examples:
    * You can try typing in RUN to see what happens next.
    * We believe that the MEGA65 is the best computer in the world.
    * However, to keep the manual more user-friendly, the programming sections may use "we". This is also more in-line 
      with the C64 manual.

* Can, not could. Similar to rule #2, and it's more polite.

* Like/wish, not want. It's more polite:
    * You may like to read more about the style guide here.
    * You may wish to change the program above to see what happens.

* Such as, similarly, not like. Please try not to get this confused with the previous item! When describing that 
  something is similar to something else, don't use the word like (too casual).
  * The external microSD slot works similarly to the internal SD card slot. Not `external microSD slot works like the 
    internal SD card slot`.

* Switch on, not power up or turn on.
    * When you switch on your MEGA65, the following screen will appear.

* Will, not should. Saying that the MEGA65 _should_ do something implies that it might
  not do it. Saying will is more confident.

* Refer to VIC and SID with capitals.

* When adding a note, use `NOTE:`, not `Note:`

* There's a dedicated LaTeX tag used for signifying keys on the MEGA65 keyboard. So instead of using text such 
  as `<RETURN>`,   use the `megakey` or `specialkey` tag, for example: `\specialkey{RETURN}`, `megakey{X}`.
  * Only letter, number, function, and symbol keys should be using the `\megakey` tag. Everything else should 
    use `\specialkey`, for the MEGA key use `\megasymbolkey`. `RESTORE` needs to be `\widekey{RESTORE}`.

* When pointing out on-screen text, the `screentext` tag should be used. Example: `\screentext{HELLO WORLD}`. 
  Note that upper case letters were used, as that's what the MEGA65 uses by default.

* When quoting keywords, tokens, or any other values whilst documenting BASIC, use plain double quotes ("). However, 
  when quoting elsewhere (for example, "homebrew") please use ``homebrew'' syntax instead.

* If you're documenting BASIC, ensure that every mention of a BASIC command is in bold and uppercase. For example,
  __DATA__, __DIR__, __PRINT__. Same goes for file types __PRG__,__USR__,__REL__,__SEQ__,__DEL__.

* When using tables, please center them on the page by using `\begin{center}` and `\end{center}`. Ensure column 
  headings **are in bold**. Separate everything with **a single horizontal line only**. This keeps them in-line with 
  the 
  C64 User Manual. An example of a table is:
```text
\begin{center}
    \begin{tabular}{|l|l|l|l|}
        \hline
            {\textbf{Boot Mode}} & {\textbf{ROM version}} & {\textbf{BASIC}} & {\textbf{C64-mode}} \\
        \hline
            MEGA65    & 92xxxx      & BASIC 65 & {\bf GO 64} \\
            C65       & 91xxxx      & BASIC 10 & {\bf GO 64} \\
        \hline
    \end{tabular}
\end{center}

```


## A-Z of commonly used words and phrases
Some commonly used words, and their preferences (in alphabetical order):

* Adapter, not adaptor. See 
  [here](https://english.stackexchange.com/questions/22537/which-is-the-proper-spelling-adapter-or-adaptor)
  for more information. 

* Brackets is preferred over the word `parentheses`.

* Command, see Keyword.

* Disk, not disc. See [here](https://en.wikipedia.org/wiki/Spelling_of_disc#UK_vs._US) for more information.

* Input/Output. Please use slashes here, same goes for the initials. i.e: I/O, not IO.

* Keyword. When documenting BASIC, there's no need to add the word `keyword` before a BASIC keyword, as they
  should already be in bold (`{\bf LOAD`), so the fact that it's a keyword is already implied.
  * Enter `{\bf LOAD}`, not enter the keyword LOAD. 

* Low and high bytes. Please refrain from using hyphens for `low byte` and `high byte`.

* MEGA65, not MEGA 65.

* Program, not programme. For computer programs, "program" is the generally accepted
  spelling. This is also consistent with the C64 User Manual.

* Programmer, not coder (C64 manual doesn't use the term, and it's not very formal).

* SD card/microSD and not sd-card.

* Trademarks and Registered notation. Refrain from using `(tm)`, `(R)`, etc. There are special sequences you can use for
  ™ and ®, namely `\texttrademark` and `\textregistered`.

* 8-bit, not 8 bit. When referring to multiple bits, no hyphen is necessary. Example:
  * The C64 is an 8-bit computer with a 16-bit address space.
  * A byte consists of 8 bits.
  * A half byte (4 bits) should be referred to as a `nibble`. Please refrain from using half-byte, nybl, nybble, etc.
  * More information on the use on hyphens in general is available in the 
    [Microsoft style guide](https://docs.microsoft.com/en-us/style-guide/punctuation/dashes-hyphens/hyphens)


## Screenshots
When adding screenshots of the MEGA65 display, please crop title bars and shadows (when emulated).
For screenshots used in the BASIC appendix, please use a black background and border colour, and ensure     
you leave a few pixels spare on all sides. This is so you can round off the corners. To do this in 
[GIMP](https://www.gimp.org/):
  * Open or paste your screenshot as a new image.
  * Press CTRL/Command+A to select all, or click Select > All.
  * Click Select > Rounded Rectangle.
  * Choose a sensible percentage so the corners are similar to the LaTeX/text based code snippets. Normally the 
    percentage is from 5-15%.
  * Press CTRL/Command + I, or click Select > Invert.
  * Press Delete, or click Edit > Clear.
  * Export the image as a PNG.

More information can be found [here](https://alvinalexander.com/design/how-to-create-rounded-rectangle-in-gimp/).
