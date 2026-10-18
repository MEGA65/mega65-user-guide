Set Decimal Flag
D $\leftarrow$ 1
D+
Deze instructie zet de decimalenvlag. Rekenkundige bewerkingen maken vanaf nu gebruik van de BCD-modus (Binary Coded Decimal).

OPMERKING: de interrupt-handler van de C64 wist de decimalenvlag niet, waardoor het gevaarlijk is de decimalenvlag te zetten zonder eerst de interruptblokkeervlag te zetten.

\subsubsection*{Neveneffecten}
\begin{itemize}
\item De D-vlag wordt gezet.
\end{itemize}
