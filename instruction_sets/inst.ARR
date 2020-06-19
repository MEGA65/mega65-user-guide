Binary AND and Rotate Right (unintended instruction)
A $\leftarrow$ (A $AND$ Value) $>>$1, C $\leftarrow$ A(7)
N+Z+C+M+V+
This instruction shifts the Accumulator one bit right after
performing a binary AND of the Accumulator and the immediate mode argument.
Bit 7 is exchanged with the carry.

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, i.e., if bit 7 is set after the operation, else it will be cleared.
\item The Z flag will be set if the result is zero, else it will be cleared.
\item The V flag will be apparently be affected in some way.
\item The C flag will be set if bit 7 of the value was set, prior to being shifted.
\end{itemize}
