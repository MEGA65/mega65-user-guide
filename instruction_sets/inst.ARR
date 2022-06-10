Binary AND and Rotate Right (unintended instruction)
A $\leftarrow$ A \binand{} M, A $\leftarrow$ A$\gg$1, C $\leftrightarrow$ A(7)
N+Z+C+M+V+
This instruction shifts the Accumulator one bit right after
performing a binary AND of the Accumulator and the immediate mode argument $M$.
Bit 7 is exchanged with the carry.

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, i.e. has it's most significant bit set, otherwise it will be cleared.
\item The Z flag will be set if the result is zero, otherwise it will be cleared.
\item The V flag will be apparently be affected in some way.
\item The C flag will be set if bit 7 of the value was set, prior to being shifted.
\end{itemize}
