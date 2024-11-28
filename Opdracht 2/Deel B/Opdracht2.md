# Opdracht 1
***Qing Scholten***

### Deel A
1. Het programma wordt binnen een seconde helemaal uitgevoerd.

2. Het duurt nu een tijdje voordat er na "start" iets geprint wordt.

3. Er is geen verschil merkbaar tussen de versie met en de versie zonder usleep(10) in de zetInBuf() methode.

4. Het programma geraakt in een deadlock. Beide threads blijven hangen in de while loop. De Sensor blijft hangen in de zetInBuf() while loop en de Verwerker blijft hangen in de haalUitBuf() while loop. Dit komt doordat beide threads de teller van de buffer tegelijkertijd gebruiken. Hierdoor gaan wijzigingen bij de een verloren bij de ander. De Verwerker blijft hangen omdat het denkt dat de buffer leeg is en blijft, omdat de bewerking van de Sensor van de teller verloren gaat. De Sensor blijft hangen in de while loop zodra het de buffer heeft volgestopt en de Verwerker geen dingen meer uit de buffer haalt waardoor de teller op 10 blijft staan.

### Deel B
1. -

2. - 

3. -

4. Ja de antwoorden zijn nog steeds hetzelfde


### Deel C
-