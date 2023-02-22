# Seminář C# Team 0023
## Cíl projektu
Cílem je vytvořit aplikaci, která bude umožňovat jejím uživatelů realizovat spolujízdy. 

## Autoři
- Přemek Janda
- David Kocman
- Tomáš Souček
- Radomír Bábek
- Adam Cologna

## Jednotlivá odevzdání

### 1. fáze
V úvodní části se zaměříme na ER diagram, k jehož vytvoření došlo během týmovému meetingu, kde všichni členové debatovali o jeho návrhu.
Kódová implementace obsahující všechny potřebné vlastnosti a vazby entitních tříd. 
Nezbytnou součástí plánování je i tvorba wireframu, která se taktéž řešila na týmové poradě.

### 2. fáze
Zde dochází k napojení datových tříd pomocí Entity Frameworku na databázi.
Vznikne Repository vrstva, kde se uskuteční zapouzdření databázové entity a fasády.
Klíčová část celého procesu je vytvoření automatizovaných testů (unit testy / integrační testy) a jejich spouštění pomocí Azure Devops Pipelines.
Entity framework se dělí na Business Logic Layer a Data Access Layer.

### 3. fáze
Poslední část projektu se soustředí na implementaci finální WPF aplikace. Dochází k vytvoření frontendové (zobrazení předpřipravených dat) i backendové části aplikace, která se napojí na datové modely, které jsou zapouzdřené za vrstvou fasád. Implementace proběhla s cílem vytvoření intuitivního a lehce použivatelného finálního produktu.
