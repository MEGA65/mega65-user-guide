Increment Memory or Accumulator
A $\leftarrow$ A + 1 $or$ M $\leftarrow$ M + 1
N+Z+M+
This instruction increments the Accumulator Register or indicated
memory location.
\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, i.e. has it's most significant bit set, otherwise it will be cleared.
\item The Z flag will be set if the result is zero, otherwise it will be cleared.
\end{itemize}