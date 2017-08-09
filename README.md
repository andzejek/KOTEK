# KOTEK
KOTEK to bezstratny, prosty algorytm kompresji plików graficznych.
Opis algorymu: algorytm jest podzielony na trzy niezależne etapy:
(Uproszczona implementacja, w pełnej wersji (niedokończonej) w fotografiach osiągał poziom pngout).
kompresja:
-kodowanie różnic (deltaEnc).
-kodowanie sekwencji:RLE (rleEnc).
-kodowanie entropijne:huffman (huffEnc).

dekompresja:
-huffDec,rleDec,deltaDec.

użycie(po skompilowaniu): 

compression: example.exe -c filein.bmp fileout.kotek
(only R8G8B8 bmp/ppm)

decompression: example.exe -d filein.kotek fileout.bmp 

W założeniach powinien(miał?) być od kilku do kilkunastu procent lepszy od zoptymalizowanego png (bezstratna kompresja fotografii, która nie była uprzednio podanna "JPEGizacji") ale dalej gorszy o te kilka, kilkanaście procent od najlepszych, dostępnych algorytmów.
