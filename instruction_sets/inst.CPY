Compare Y Register
N,C,Z $\Leftarrow$ $[$Y $-$ M$]$
N+Z+C+
This instruction performs Y $-$ M, and sets the processor flags accordingly,
but does not modify the contents of the Accumulator Register.

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result of Y $-$ M is negative, i.e. has it's most significant bit set, otherwise it will be cleared.
\item The C flag will be set if the result of Y $-$ M is zero or positive, i.e., if Y is not less than M, otherwise it will be cleared.
\item The Z flag will be set if the result of Y $-$ M is zero, otherwise it will be cleared.
\end{itemize}
