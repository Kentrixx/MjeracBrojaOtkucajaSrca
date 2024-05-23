# O projektu

Projekat "Mjerač broja otkucaja srca" je osmišljen tako da se pomoću senzora mjere otkucaji srca osobe. Mjerenje se vrši svake sekunde, pri čemu se na LCD displeju ispisuje izmjerena vrijednost. Prema ovoj vrijednosti se računa trajanje jednog otkucaja srca osobe. Neposredno nakon toga, na prednjoj strani kućišta pali se LED dioda i buzzer zvučnik unutar kućišta svira određeni ton. Oba ova događaja traju koliko traje jedan otkucaj srca, nakon čega se LED dioda gasi, kao i ton na buzzeru.

Matrix displej, koji se nalazi na dnu prednje strane kućišta, je podijeljen tako da svaka njegova kolona predstavlja jedan period uzorkovanja vrijednosti sa senzora za otkucaje srca, pri čemu kolona skroz desno predstavlja t(n) tj. upravo izmjerenu i ujedno najnoviju vrijednost, dok kolona skroz lijevo predstavlja t(n-31) tj. vrijednost izmjerenu prije 31 sekundu. Displej je također podijeljen tako da svaki njegov red predstavlja određen opseg otkucaja srca. Donji red sadrži vrijednosti koje su manje od 60, drugi red odozdo sadrži vrijednosti između 60 i 70 otkucaja po minuti, treći one između 70 i 80 i sve tako do osmog reda tj. gornjeg koji sadrži vrijednosti iznad 120 otkucaja po minuti. Zahvaljujući ovome, moguće je nacrtati graf koji oslikaje aktivnost srca osobe u zadnjih tridesetak sekundi.

Nakon što se nova vrijednost učita, biva joj dodijeljeno mjesto u prvoj koloni desno na matrix displeju, a sve prijašnje vrijednosti se pomjeraju za jednu kolonu desno. Red u kojem će se nalaziti zavisi od toga kojem opsegu otkucaja srca pripada. Nakon što joj je dodijeljena potpuna pozicija, mikrokontroler čeka da prođe tačno jedna sekunda od posljednjeg uzorkovanja, kako bi mogao započeti sa novim. 

# Spisak korištene opreme

Arduino Uno R3

LED dioda

Buzzer zvučnik

I2C LCD 1602 displej

8x32 MAX7219 matrix displej

HW-827 puls senzor za otkucaje srca

Žice

Breadboard

3D printano kućište
