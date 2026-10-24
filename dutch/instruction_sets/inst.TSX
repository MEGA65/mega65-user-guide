Transfer Stack Pointer Low Register into the X Register
X $\leftarrow$ SPL
N+Z+
Deze instructie laadt het X-register met de inhoud van het Stack Pointer Low (SPL)-register.

\subsubsection*{Neveneffecten}
\begin{itemize}
\item De N-vlag wordt gezet als het resultaat negatief is, dat wil zeggen als het meest significante bit gezet is; anders wordt hij gewist.\item De Z-vlag wordt gezet als het resultaat nul is; anders wordt hij gewist.
\end{itemize}
