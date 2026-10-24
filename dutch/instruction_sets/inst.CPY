Compare Y Register
N,C,Z $\Leftarrow$ $[$Y $-$ M$]$
N+Z+C+
Deze instructie voert Y $-$ M uit en zet de processorvlaggen dienovereenkomstig, maar wijzigt de inhoud van het accumulatorregister niet.

\subsubsection*{Neveneffecten}
\begin{itemize}
\item De N-vlag wordt gezet als het resultaat van Y $-$ M negatief is, d.w.z. als het meest significante bit gezet is; anders wordt deze gewist.\item De C-vlag wordt gezet als het resultaat van Y $-$ M nul of positief is, d.w.z. als Y niet kleiner is dan M; anders wordt deze gewist.\item De Z-vlag wordt gezet als het resultaat van Y $-$ M nul is; anders wordt deze gewist.
\end{itemize}
