Compare Y Register

N+Z+C+
This instruction performs Y $-$ M, and sets the processor flags accordingly,
but does not modify the contents of the Accumulator Register.

\subsubsection*{Side effects}
\begin{itemize}
\item The N flag will be set if the result of Y $-$ M is negative, i.e. bit 7 is set in the result, else it will be cleared.
\item The C flag will be set if the result of Y $-$ M is zero or positive, i.e., if Y is not less than M, else it will be cleared.
\item The Z flag will be set if the result of Y $-$ M is zero, else it will be cleared.
\end{itemize}
