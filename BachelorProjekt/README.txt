Hvis i vil køre vores main-metode, hvor du får en lille bid af, hvordan programmet fungerer, kan i køre:

python3 main.py

Vi havde problemer med at få visualiseringen til at virke i linux, hvorfor vi brugte Windows PowerShell.
Vi kunne nemt køre det der efter at have importeret networkx samt matplotlib.

I får en del valgmuligheder, når I kører programmet, hvor I får muligheden for at køre transformationen,
for de givede regler, som vi skriver om i rapporten. 
Vi giver kun valget mellem at transformere glykolaldehyd, formaldehyd, kombinationen af de to eller en testgraf.
I kan dog ændre på disse i source-koden eller tilføje andre grafer, hvis i er interesseret i dette, på de simple måder, som beskrives i rapporten.
Hvis i vil visualisere graferne eller vil se knuderne/kanterne på dem, får i valget for det.

Hvis I har lyst til at køre nogle af de andre metoder, såsom 'transformation2', der transformerer outputtet
af alle de grafer vi får af at køre de fire regler igennem kombinationen af to formaldehyd- samt ét glykolaldehyd-molekyle, kan i blot
uddokumenterer 'main-metoden' og køre disse funktioner.

Vores køretidsfunktion virker ikke i main-funktionen, kun hvis man kører koden direkte i filen.
Dette er pga. den tæller med, når useren indtaster sit input, hvilket er irrelevant.
Hvis i vil teste køretider, skal i blot køre funktionerne i filen og ikke i main-metoden.


