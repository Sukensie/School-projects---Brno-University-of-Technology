Dokumentace k 1. úloze do IPK 2021/2022  
Jméno a příjmení: Tomáš Souček  
Login: xsouce15  
---
Tahle dokumentace popisuje server v jazyce C komunikující prostřednictvím HTTP. Server naslouchá na zadaném portu a na standartní výstup vrací na základě zadané url požadované informace.

### Spouštění serveru
1) Přeložení zdrojového kódu v souboru `server.c` pomocí příkazu `make`  
2) `./hinfosvc port` - nutnost zadat číslo portu (0-65535), na kterém bude server naslouchat


### Ukázkové příklady funkcionality
`./hinfosvc 12345 &` - spuštění serveru na pozadí s portem 12345

**Získání doménového jména**  
Zasláním HTTP requestu `GET http://servername:12345/hostname`  
merlin.fit.vutbr.cz  

**Získání informací o CPU**  
Zasláním HTTP requestu `GET http://servername:12345/cpu-name`  
Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz  

**Aktuální zátěž procesoru**  
Zasláním HTTP requestu `GET http://servername:12345/load`  
37.33%