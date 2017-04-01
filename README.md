# KOTEK
KOTEK to bezstratny(lub stratny) prosty algorytm kompresji plików graficznych (przeznaczony do fotografii).
W założeniach powinien cechować się kompresją mocniejszą o kilka % w porównaniu do zoptymalizowanego PNG (w oparciu o kodowanie huffmana).
Opis algorymu: algorytm jest podzielony na trzy niezależne etapy:

kompresja:
-kodowanie różnic (deltaEnc)
-kodowanie sekwencji:RLE (rleEnc)
-kodowanie entropijne:huffman (huffEnc)

dekompresja:
-huffDec,rleDec,deltaDec.

Przedstawiona tutaj implementacja jest uproszczona, przez co brakuje jej do oczekiwanego stopnia kompresji.
