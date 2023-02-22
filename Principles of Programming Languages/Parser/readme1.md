Implementační dokumentace k 1. úloze do IPP 2021/2022
Jméno a příjmení: Tomáš Souček
Login: xsouce15
---
Tahle dokumentace popisuje analyzátor kódu IPPcode22, který je implementován v souboru `parse.php`. 
Program načte ze standartního vstupu kód v jazyce IPPcode22 a na standartní výstup vytiskne jeho XML reprezentaci. V případě, že se nepodaří lexikální nebo syntaktická analáza vstupu, ukončí se program se specifickou návratovou hodnotou.

Program může být spuštěn s argumentem `--help`, který zobrazí krátkou informaci o programu.

## Pomocné funkce
- **deleteComments($line)**
Slouží k odstranění komentářů. Najde pozici prvního výskytu # a uchová pouze obsah řádku do této pozice. Pokud se na daném řádku nevyskytuje žádný #, neprovede nic.

- **checkNumArguments($array, $number)**
Kontroluje správný počet argumetnů dané instrukce. V případě, že má instrukce více nebo méně argumentů, ukončí program s návratovou hodnotou 23


- **getArgumentType($array, $position, $expected)**
Pomocí regulárního výrazu kontroluje, zda obsah argumentu odpovídá očekávanému typu. Pokud ne, ukončí program s návratovou hodnotou 23

- **varOrSymb($array, $argNum)**
Funkce rozhodne pomocí regulárního výrazu, zda se jedná o konstantu nebo proměnnou. Podle výsledku vytiskne na standartní výstup XML reprezentaci argumentu. Používám u všech funkcí, které mají argument `<symb>`. Samotná funkce `getArgumentType` nedokáže rozpoznat, jestli je daný symbol proměnná, nebo konstanta. 

## Hlavní funkcionalita
Celý program je implementován uvnitř smyčky, která načítá řádky do skončení souboru. Po načtení řádku se osekají bílé znaky před i po obsahu řádku a odstraní se komentáře. V případě, že řádek zůstal po těchto úpravách prázdný, přistoupí se k dalšímu a obsah smyčky se vykonává od začátku znovu. 
Pokud není prázdný dojde k escapování speciálních XML znaků (&, <, >, ', ") tak, aby nedošlo k poškození obsahu řádku. Následně se řádek rozdělí na instrukce a argumenty pomocí příkazu `explode(" ", trim($line))`, který rozdělí řádek do pole podle delimetru znaku mezery. Aby se zamezilo špatnému rozdělení např. když je na řádku mezi argumenty více mezer, dojde ještě před samotným rozdělením k nahrazení vice mezer za jednu pomocí regulárního výrazu.

V první iteraci se zkontroluje, zda zdrojový soubor obsahuje hlavičku IPPcode22. Pokud neobsahuje ukončí se program s hodnotou 21. Následně se pokračuje již k výpisu instrukcí a kontrole argumentů.

Instrukce jsem si rozdělil do skupin podle typu a počtu očekávaných argumentů. Dovolilo mi to tak psát čitelnější a neopakující se kód. Pro každou instrukci kontroluji počet jejích argumentů, zjištuji typ argumentu a následně argument vypisuji na standartní výstup. Po validním vypsání všech argumentů se vypíše i ukončující polovina instrukce. U funkce `READ` jsem provedl kontrolu druhého argumentu bez použití `getArgumentType`, jelikož se jedná o jedinou funkci očekávající argument typu type. 

Pokud se na vstupu objeví funkce s neznámým operačním kódem, je program ukončen pomocí hodnoty 22.