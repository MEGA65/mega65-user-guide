# MEGA65 Style Guide

In order for the suite of MEGA65 books to be easier to read and understand,
it's important that the English in the manuals is well-written and
consistent. To keep the MEGA65 style guide short, the team has chosen
to rely on the [BBC News Style Guide](https://www.bbc.co.uk/newsstyleguide/)
as much as possible for the basics. For computer specific terminology and spelling,
the [Commodore 64 User Manual](http://www.zimmers.net/anonftp/pub/cbm/c64/manuals/C64_User_Manual_1984_2nd_Edition.pdf) was used for inspiration.

The intended audience for people reading (and contributing to) MEGA65 literature are people who are comfortable using a computer, and have good English skills.

If you're thinking of contributing, you should be comfortable writing about electronics and computing in general.

When writing content for the MEGA65 books, the intended audience in terms of readers are people who know they're way around a computer, but don't assume they know how to program a computer or know how/why certain things work. The manuals should be as user-friendly as possible, but also be concise.

Below are some simple MEGA65 rules:

* First and foremost, British English spelling is used.
    * Organise, not organize.
    * Colours, not colors.

* You, not we. When describing steps a user can make in order to perform try to use
  the word "you", instead of "we". When describing the intention(s) of the MEGA65 team,
  we can be used. Examples:
    * You can try typing in RUN to see what happens next.
    * We believe that the MEGA65 is the best computer in the world.
    * However, to keep the manual more user-friendly, the programming sections may use "we". This is also more in-line with the       C64 manual.

* Can, not could. Similar to rule #2, and it's more polite.

* Like/wish, not want. It's more polite:
    * You may like to read more about the style guide here.
    * You may wish to change the program above to see what happens.

* Turn on, not power up or switch on. This is how it's worded in the C64 User Manual.
    * When you turn on your MEGA65, the following screen will appear.

* Will, not should. Saying that the MEGA65 _should_ do something implies that it might
  not do it. Saying will is more confident.

* Refer to VIC and SID with capitals.

* When adding a note, use `NOTE:`, not `Note:`

* There's a dedicated LaTeX tag used for signifying keys on the MEGA65 keyboard. So instead of using text such as `<RETURN>`,   use the `megakey` tag, for example: `\megakey{RETURN}`.

* When pointing out on-screen text, the `screentext` tag should be used. Example: `\screentext{HELLO WORLD}`. Note that upper case letters were used, as that's what the MEGA65 uses by default.

* When quoting keywords, tokens, or any other values whilst documenting BASIC, use plain double quotes ("). However, 
  when quoting elsewhere (for example, "home-brew") please use ``home-brew'' syntax instead.


Some commonly used words, and their preferences:

* Adapter, not adaptor. See [here](https://english.stackexchange.com/questions/22537/which-is-the-proper-spelling-adapter-or-adaptor) for more information. 

* Brackets is preferred over parentheses.

* Disk, not disc. See [here](https://en.wikipedia.org/wiki/Spelling_of_disc#UK_vs._US) for more information.

* MEGA65, not MEGA 65.

* Program, not programme. For computer programs, "program" is the generally accepted
  spelling. This is also consistent with the C64 User Manual.

* Programmer, not coder (C64 manual doesn't use the term, and it's not very formal).

* SD card/microSD and not sd-card.
