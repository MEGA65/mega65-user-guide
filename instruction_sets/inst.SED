Set Decimal Flag
D $\leftarrow$ 1

This instruction sets the Decimal Flag.
Binary arithmetic will now use Binary-Coded Decimal (BCD) mode.

NOTE: The C64's interrupt handler does not clear the Decimal Flag,
which makes it dangerous to set the Decimal Flag without first
setting the Interrupt Disable Flag.

\subsubsection*{Side effects}
\begin{itemize}
\item The D flag is set.
\end{itemize}