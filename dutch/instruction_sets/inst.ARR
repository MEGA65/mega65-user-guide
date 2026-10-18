Binary AND and Rotate Right (unintended instruction)
A $\leftarrow$ A \binand{} M, A $\leftarrow$ A$\gg$1, C $\leftrightarrow$ A(7)
N+Z+C+M+V+
Deze instructie verschuift het accumulatorregister één bit naar rechts na het uitvoeren van een bitgewijze EN-operatie van het accumulatorregister en het argument $M$ in de immediate-addreringswijze. Bit 7 wordt uitgewisseld met de carry-vlag.

\subsubsection*{Neveneffecten}
\begin{itemize}
\item De N-vlag wordt gezet als het resultaat negatief is, d.w.z. als het meest significante bit is ingesteld, anders wordt de vlag gewist.\item De Z-vlag wordt gezet als het resultaat nul is, anders wordt de vlag gewist.\item De V-vlag wordt op een of andere manier beïnvloed.\item De C-vlag wordt gezet als bit 7 van de waarde was ingesteld, voor het verschuiven.
\end{itemize}
