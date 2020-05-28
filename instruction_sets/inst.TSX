Transfer Stack Pointer High Register into the X Register
X $\leftarrow$ SPH
N+Z+
This instruction loads the X Register with the contents of the Stack Pointer High (SPL)
Register.

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result is negative, else it will be cleared.
\item The Z flag will be set if the result is zero, else it will be cleared.
\end{itemize}