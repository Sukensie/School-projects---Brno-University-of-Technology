Implementační dokumentace k 2. úloze do IPP 2021/2022
Jméno a příjmení: Tomáš Souček
Login: xsouce15
---
Tato dokumentace popisuje interpret XML reprezentace IPPcode22, který je implementován v souboru `interpret.py` a testovací rámec v souboru  `test.php`

# Interpret
Skript načítá ze standartního vstupu XML reprezentaci IPPcode22 a na základě daných pravidel jazyka interpretuje a vygeneruje výstup. V případě, že se interpretace nepodaří, ukončí se program se specifickou návratovou hodnotou.

Skript může být spuštěn s parametry
- `--help` -  který zobrazí krátkou informaci o využití programu
- `--source=file` -  volitelný argument. Pokud není zadaný, načítá se vstup ze standartního vstupového kanálu. Pokud je zadaný načítá se vstup ze souboru
- `--input=file` -  volitelný argument. Pokud není zadaný, vstupy pro funkce se načítají ze  standartního vstupového kanálu. Pokud je zadaný načítají se ze souboru

Interpret byl implementován pomocí objektového přístupu s využitím návrhové vzoru továrny.
Byly vytvořeny následující objekty:
- `Argument` - argument obsahující typ a hodnotu
- `Instruction` - instrukce obsahující pořadí, operační kód, list argumentů a funkci na přidání argumentu
- `Frame` - rámec obsahující typ a slovník hodnot, funkci na přidání hodnoty a kontrolní funkci na prázdnost rámce
- `Stack` - zásobník obsahující rámce a funkce na přidání, odebrání, kontrolu prázdnosti a zobrazení vrchního rámce

### Návrhový vzor OOP
V projektu bylo implementováno rozšíření NVI. Pro objektový přístup byl použit návrhový vzor továrna. Uvnitř třídy je inicializátor s parametrem self a následně se v kódu vytváří objekty pomocí volání kontruktoru této třídy. Továrna byla využita převážně z toho důvodu, že není potřeba provázanost jednotlivých objektů a jedná se o poměrně jednoduše implementovatelný vzor. Za nevýhodu tohoto vzoru může být považováno například větší množství používaných tříd a složitější abstrakce.

### Pomocné funkce
- **checkProgramArgs()**
Zjišťuje, jestli byly zadané argumenty `input` a `source`. Pokud ano, přepne načítání vstupu na zadaný soubor

- **checkNumArguments(args, $number)**
Kontroluje správný počet argumetnů dané instrukce. V případě, že má instrukce více nebo méně argumentů, ukončí program s návratovou hodnotou 10


- **checkInt(string)**
Kontroluje jestli je zadaný řetězec číslo. Inspirováno z https://appdividend.com/2021/03/23/how-to-check-if-string-is-integer-in-python/

- **checkBool(arg)**
Podle typu a hodnoty argumentu kontroluje, zda se jedná o boolean

- **checkString(arg)**
Podle typu argumentu kontroluje, zda se jedná o string

- **checkFrame(string)**
Na základě stringu určuje, zda je daný rámec definován

- **getFrame(string, shutdown)**
Vrací požadovaný rámec, pokud je definován. Pokud není rámec vrátí, že se jedná o konstantu, nebo nil. V případě, že shutdown je nastaven na 1 a string není rámec, ukončí skript s hodnotou 55

- **checkVarExistence(frame, var, shutdown)**
Kontroluje, zda existuje proměnná v daném rámci. Pokud ne a shutdown je nastaven na 1, ukončí skript s hodnotou 54

- **escapeToChars(inputString)**
Najde všechny escape sekvence ve tvaru \xyz a změní je na písmena

- **getElement(arg)**
Vrátí konkrétní proměnnou (její typ a hodnotu)

### Hlavní funkcionalita
Na začátku běhu programu se načtou argumenty a pokud je potřeba změní se vstupy. Následně dojde k parsování XML struktury IPPcode22. Pokud není struktura v pořádku (chybné operační kódy, chybějící hlavička, ...), program se ukončí se specifickou hodnotou.
Pokud je ovšem sktruktura XML správná má program v tuhle chvíli načtené všechny instrukce do listu a proběhne jejich seřazení tak, aby se vykonaly ve správném pořadí (inspirace z https://wiki.python.org/moin/HowTo/Sorting#Sortingbykeys). Následně se provede kontrola, zda se mezi instrukcemi nevyskytuje nějaký label. Pokud ano, tak se všechny uloží do slovníku ve tvaru `název: order`

Hlavní část programu je implementován uvnitř smyčky, která prochází seřazený list instrukcí a podle operačního kódu zavolá příslušnou funkci. Výjimku tvoří instrukce pro řízení chodu programu. V případě instrukce `LABEL` se nic neprovede, jelikož labely už jsou uchovány před počátkem hlavní smyčky. U zbylých řídících funkcí, **vyjma EXIT**, se výsledek volání funkcí ukládá do indexové proměnné a tím se mění tok programu.

Pokud se na vstupu objeví funkce s neznámým operačním kódem, je program ukončen pomocí hodnoty 32.


# Testovací rámec
Testovací PHP skript, který vygeneruje HTML5 reprezentaci výsledků testů nad interpretem nebo parserem s možností filtrování podle výsledku.

Skript může být spuštěn s parametry
- `--help` -  zobrazí krátkou informaci o využití programu
-  `--directory=path` - cesta ke složce s testy
-  `--recursive` - rekurzivní prohledávání složek s testy
-  `--parse-script=path` - cesta k parseru
-  `--int-script=path` - cesta k interpretu
-  `--parse-only` - testování proběhne pouze pro parser
-  `--int-only` - testování proběhne pouze pro interpret
-  `--jexampath=path` - cesta k souboru jexamxml.jar
- `--noclean` -  zamezí implicitnímu mazání dočasného výstupního souboru

### Pomocné funkce
- **findTests($target, &$tests, $recursive)**
Nalezení testů v cestě uložené v proměnné `$target` a nahrání na správný index podle koncovky souboru do pole `$tests`. Pokud se nejedná o soubor, ale o složku a je povolené rekurzivní vyhledávání, pak dojde k rekurzivnímu zavolání funkce.
Pole `$tests` se skládá ze 4 indexů (src, in, out, rc) s tím, že každý z těchto indexů ještě implementuje své pole.

- **addMissingFile(&$tests, $extension, $i)**
Vytvoření chybějících .`in, .out, .rc` souborů na základě jména `.src` souboru. Pokud dochází k vytvoření `.rc` souboru, pak se do něj dle zadání vepíše 0.

- **fileCheck($file)**
Kontrola, zda zadaný soubor existuje a je přístupný. Pokud ne, vyhodí chybu 41

- **htmlHead($arguments)**
Vykreslení HTML hlavičky a nastavení CSS stylů

- **htmlEnd()**
Ukončení HTML zápisu a vykreslení javascriptového skriptu na filtrování, který na základě HTML třídy zneviditelní jednotlivé řádky.


### Hlavní funkcionalita
Na začátku běhu programu se načtou argumenty a nastaví se na výchozí hodnoty. Následně dojde k vykreslení HTML hlavičky obsahující také CSS styly. Po vykreslení se provede nalezení jednotlivých testů a nahrání jejich názvů do asociativního pole `$tests`. Pak program přechází do hlavní smyčky.

V hlavní části programu se pro každý `.src` soubor zkontroluje, zda byly nalezeny i ostatní pomocné soubory (`.in, .out, .rc`) a pokud nebyly, dojde k jejich vytvoření. Poté se zjistí očekávaný návratový kód a podle zadaných parametrů se spustí požadovaný testovaný skript. Po provedení všech testů se ještě smažou dočasné soubory, není-li parametrem `noclean` specifikováno jinak. Na konec se provede procentuální výpočet správnosti testů a vykreslí se javascriptový skript na filtrování výsledků a ukončí se HTML zápis. 
