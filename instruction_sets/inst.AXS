AND and Subtract (undocumented instruction)
X $\leftarrow$ ( A $AND$ X ) - V
N+Z+M+
This instruction loads the X Register  with the binary
AND of the Accumulator Register and X Register, less the
immediate argument.

NOTE: The subtraction effect in this instruction is due to 
CMP \index{CMP}, not \index{SBC}. Thus the Negative Flag
is set according to the function of CMP, not SBC.  That is,
the carry flag is not used in the calculation.

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, else it will be cleared.
\item The Z flag will be set if the result is zero, else it will be cleared.
\item The C flag will be set if the result is zero or positive, else it will be cleared.
\end{itemize}
