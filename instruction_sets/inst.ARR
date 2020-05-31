Binary AND and Rotate Right (unintended instruction)
A $\leftarrow$ (A $AND$ Value) $>>$1, C $\leftarrow$ A(0)
N+Z+C+M+
This instruction shifts the Accumulator one bit right after
performing a binary AND of the Accumulator and the immediate mode argument.
Bit 7 will be set to zero, and the bit 0 will be shifted out into the Carry Flag

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, i.e., if bit 7 is set after the operation, else it will be cleared.
\item The Z flag will be set if the result is zero, else it will be cleared.
\item The V flag will be apparently be affected in some way.
\item The C flag will be set if bit 0 of the value was set, prior to being shifted.
\end{itemize}
