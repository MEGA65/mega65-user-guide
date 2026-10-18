Arithmetic Shift Left Memory or Accumulator
A $\leftarrow$ A$\ll$1 or M $\leftarrow$ M$\ll$1
N+Z+C+M+
Deze instructie verschuift het accumulatorregister of de inhoud van de opgegeven geheugenlocatie één bit naar links.  Bit 0 wordt op nul gezet en bit 7 wordt verschoven naar de carry-vlag.

\subsubsection*{Neveneffecten}
\begin{itemize}
\item De N-vlag wordt gezet als het resultaat negatief is, d.w.z. als het meest significante bit is ingesteld, anders wordt de vlag gewist.\item De Z-vlag wordt gezet als het resultaat nul is, anders wordt de vlag gewist.\item De C-vlag wordt gezet als bit 7 van de waarde was ingesteld, voor het verschuiven.
\end{itemize}
