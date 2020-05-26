Arithmetic Shift Left
A $\leftarrow$ A$<<$1 $or$ M $\leftarrow$ M$<<$1
N+Z+C+M+
This instruction shifts either the Accumulator or contents
of the provided memory location one bit left.  Bit 0 will be
set to zero, and the bit 7 will be shifted out into the Carry Flag

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, i.e., if bit 7 is set after the operation, else it will be cleared.
\item The Z flag will be set if the result is zero, else it will be cleared.
\item The C flag will be set if bit 7 of the value was set, prior to being shifted.
\end{itemize}
