Transfer Stack Pointer Low Register into the X Register
X $\leftarrow$ SPL
N+Z+
This instruction loads the X Register with the contents of the Stack Pointer Low (SPL)
Register.

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, i.e. has it's most significant bit set, otherwise it will be cleared.
\item The Z flag will be set if the result is zero, otherwise it will be cleared.
\end{itemize}