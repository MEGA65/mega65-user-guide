Set A, X and SPL Register With Useless Value (unintended instruction)
SP, A, X $\leftarrow$ SP \binand{} M
N+,Z+
OPMERKING: dit monster van een instructie is, afgezien van het feit dat het elk denkbaar nuttig doel ontbeert, onstabiel op veel 6502-processoren en dient daarom ook om die reden te worden vermeden, indien u niet al was afgeschrikt.

\subsubsection*{Neveneffecten}
\begin{itemize}
\item De N-vlag wordt gezet als het resultaat negatief is, d.w.z. als het meest significante bit gezet is; anders wordt hij gewist.\item De Z-vlag wordt gezet als het resultaat nul is, anders wordt hij gewist.\item Een gevoel van holle voldoening, wanneer u daadwerkelijk een nuttig doel voor deze instructie ontdekt.
\end{itemize}
