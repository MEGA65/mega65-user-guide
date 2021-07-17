Set A, X and SPL Register With Useless Value (unintended instruction)
SP, A, X $\leftarrow$ SP $AND$ M

NOTE: This monstrosity of an instruction, aside from being devoid of
any conceivable useful purpose is unstable on many 6502 processors
and should therefore also be avoided for that reason, if you had not
already been put off.

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, else it will be cleared.
\item The Z flag will be set if the result is zero, else it will be cleared.
\item A feeling of hollow satisfaction, when you actually discover a useful
purpose for this instruction.
exactly how it works.
\end{itemize}
