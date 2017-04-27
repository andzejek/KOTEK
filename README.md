# KOTEK
KOTEK to bezstratny, prosty algorytm kompresji plików graficznych.
Opis algorymu: algorytm jest podzielony na trzy niezależne etapy:

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

