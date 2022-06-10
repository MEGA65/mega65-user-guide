Set A, X and SPL Register With Useless Value (unintended instruction)
SP, A, X $\leftarrow$ SP \binand{} M
N+,Z+
NOTE: This monstrosity of an instruction, aside from being devoid of
any conceivable useful purpose is unstable on many 6502 processors
and should therefore also be avoided for that reason, if you had not
already been put off.

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, i.e. has it's most significant bit set, otherwise it will be cleared.
\item The Z flag will be set if the result is zero, otherwise it will be cleared.
\item A feeling of hollow satisfaction, when you actually discover a useful
purpose for this instruction.
\end{itemize}
