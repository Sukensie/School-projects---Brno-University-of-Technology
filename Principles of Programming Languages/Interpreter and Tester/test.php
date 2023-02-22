<?php

//ukončí html výpisu a vytvoří funkcionalitu filrtrování
function htmlEnd()
{
    ?>
    </tbody>
    </table>
    <script>
        let array = document.querySelector("table tbody").children;
        let btn = document.querySelector("#filter-btn");
        btn.addEventListener('click', function()
        {
            let filter = document.querySelector("#filter").value;
          
            for(let i = 0; i < array.length; i++)
            {
                if(array[i].lastChild.children[0].className != filter && filter != "all")
                {
                    array[i].style.display = "none";
                }
                else
                {
                    array[i].style.display = "table-row";
                }
            }       
        });
        
      
    </script>
    </body>
    </html>
    <?php
}

//nalezení testů a vložení do pole $tests
function findTests($target, &$tests, $recursive)
{
    $files = glob( $target . '*', GLOB_MARK ); //GLOB_MARK adds a slash to directories returned
    foreach( $files as $file )
    {
        $pathInfo = pathinfo($file);
        if($pathInfo["basename"] == "jexamxml")
        {
            continue;
        }
        if(is_dir($file))
        {
            if($recursive) //kdybych neudělal vnořené podmínky, tak ve chvíli, kdy bych neměl rekurzivní hledání, tak by mi to do $tests dalo i složky z původní složky
            {
                findTests($file, $tests, $recursive);
            }  
        }
        else
        {   
            if(array_key_exists("extension", $pathInfo))
            {
                $index = $pathInfo["extension"];
                if($index == "src" ||$index == "in" || $index == "out" ||$index == "rc")
                {
                    array_push($tests[$index], $file);
                }
            }
        } 
    }
    
}

//vytvoří chybějící rc, in nebo out soubor
function addMissingFile(&$tests, $extension, $i)
{   
    $srcFile = pathinfo($tests["src"][$i]);

    //musí se provést kontrola zda index existuje, aby nedocházelo ke špatnému přístupu do paměti
    $missingFile = array("filename" => "");
    if(array_key_exists($i,$tests[$extension]))
    {
        $missingFile = pathinfo($tests[$extension][$i]);
    }
    
    
    
    //pokud na stejném indexu uvnitř jednotlivých typů testů nejsou soubory se stejným jménem jako v src typu, pak chybí ostatní soubory
    if($srcFile["filename"] != $missingFile["filename"])
    {
        $newFile = $srcFile["dirname"]."/".$srcFile["filename"].".".$extension;
        $tmpRcFile = fopen($newFile, "w");

        if($extension == "rc")
        {
            fwrite($tmpRcFile, "0");
        }
        fclose($tmpRcFile);

        array_splice($tests[$extension], $i, 0, $newFile);  //vložení do pole již existujících testovacích souborů
    }
}

//kontroluje, zda zadaný soubor existuje a je přístupný. Pokud ne, vyhodí chybu 41
function fileCheck($file)
{
    if(!file_exists($file))
    {
        fwrite(STDERR, "zadaný soubor ".strval($file)." neexistuje, nebo není přístupný\n");
        exit(41);
    }
}

//vykreslení html a nastavení css
function htmlHead($arguments)
{
    ?>
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>IPP test script</title>
        <style>
            body
            {
                font-family: 'Segoe UI', sens-serif;
                margin: 0;
            }
            table
            {
                width: 60%;
                min-width: 1000px;
                margin: 1em auto;
                box-shadow: 0 0 10px #ccc;
                border-collapse: collapse;
            }
            table thead
            {
                background: #00AAE3;
                color: #fff;
            }
            table td, table th
            {
                text-align: left;
                padding: 1.3em 1em;
            }
            table tr
            {
                border-bottom: 1px solid #ededed;
            }
            table td:last-child, table th:last-child
            {
                text-align: right;
            }
            .success
            {
                color: #28a745;
            }
            .fail
            {
                color: #dc3545;
            }
            .container
            {
                display: flex;
                width: 60%;
                margin: 1em auto;
                gap: 1em;
                min-width: 1000px;
            }
            .sub-container
            {
                box-shadow: 0 0 5px #ccc;
                padding: 1em 2em;
                width: 50%;
            }
            #filter-btn
            {
                padding: 1em 2em;
                background: #00AAE3;
                color: white;
                font-weight: bold;
            }
            select
            {
                padding: 1em;
            }
            .orange
            {
                color: #ffc107;
            }
            .result-container
            {
                box-shadow: 0 0 5px #ccc;
                padding: 1em 2em;
                width: 60%;
                display: flex;
                margin: 1em auto;
                box-sizing: border-box; 
                align-items: center;
                justify-content: space-between;
                min-width: 1000px;
            }
            .result-container span
            {
                font-weight: bold;
                font-size: 2em;
            }
            header
            {
                font-size: 14px;
                display: flex;
                justify-content: space-between;
                padding: 0 2em;
                margin-bottom: 3em;
                background: #00AAE3;
                color:white;
            }
        </style>
    </head>
    <body>
        <header>
            <h1>IPP 2022 Testovací rámec</h1>
            <p>Tomáš Souček<br>xsouce15</p>
        </header>
        <div class="container">
            <div class="sub-container">
                <h2>Nastavení testů</h2>
                <ul>
                <?php
                    foreach (array_keys($arguments) as $option)
                    {
                        print("<li>".$option."</li>");
                    }
                ?>
                <ul>
            </div>

            <div class="sub-container">
                <h2>Filtrování testů</h2>
                <select id="filter">
                    <option value="all">Všechny</option>
                    <option value="success">Pouze úspěšné</option>
                    <option value="fail">Pouze neúspěšné</option>
                </select>
                <button id="filter-btn">Filtrovat</button>
            </div>
        </div>


        <table>
            <thead>
                <tr><th>ID</th><th>Cesta k testu</th><th>Poznámka</th><th>Úspěšnost</th></tr>
            </thead>
            <tbody>
<?php
}

//získání argumentů z příkazové řádky
$arguments = getopt("", ["help", "directory:", "recursive", "parse-script:", "int-script:", "parse-only", "int-only", "jexampath:", "noclean"]);


//defaultní nastavení argumentů
$recursive = array_key_exists("recursive", $arguments); //pokud v poli nalezne klíč vrátí true, jinak vrátí false
$parseOnly = array_key_exists("parse-only", $arguments);
$intOnly  = array_key_exists("int-only", $arguments);
$noClean = array_key_exists("noclean", $arguments);


if($parseOnly && ($intOnly || array_key_exists("int-script", $arguments)))
{
    fwrite(STDERR, "parse-only se nesmí kombinovat s int-only nebo int-script\n");
    exit(10);
}
if($intOnly && ($parseOnly || array_key_exists("parse-script", $arguments)))
{
    fwrite(STDERR, "int-only se nesmí kombinovat s parse-only nebo parse-script\n");
    exit(10);
}

$directory = "./";
$parseScript = "parse.php";
$intScript = "interpret.py";
$jexamPath = "/pub/courses/ipp/jexamxml/";




if(array_key_exists("help", $arguments)) 
{
    if(count($arguments) > 1)
    {
        fwrite(STDERR, "help nesmí být zavolán v kombinaci s dalšími argumenty\n");
        exit(10);
    }
    echo "TBD";
    exit(0);
}
if(array_key_exists("directory", $arguments))
{   
    fileCheck($arguments["directory"]);
    $directory = $arguments["directory"];
    $directory .= "/"; //nutné pro otevření dané složky
}
if(array_key_exists("parse-script", $arguments))
{
    fileCheck($arguments["parse-script"]);
    $parseScript = $arguments["parse-script"];
}
if(array_key_exists("int-script", $arguments))
{
    fileCheck($arguments["int-script"]);
    $intScript = $arguments["int-script"];
}
if(array_key_exists("jexampath", $arguments))
{
    fileCheck($arguments["jexampath"]);
    $jexamPath = $arguments["jexampath"];
}


htmlHead($arguments);


$tests = array("src" => array(), "in" => array(), "out" => array(), "rc" => array());

findTests($directory, $tests, $recursive);
 
$succCount = 0;

//procházení jednotlivých testů
for($i = 0; $i < count($tests["src"]); $i++)
{
    //přidání chybějících souborů, je-li potřeba
    addMissingFile($tests, "rc", $i);
    addMissingFile($tests, "in", $i);
    addMissingFile($tests, "out", $i);

    //získání očekávaného návratového kódu
    $expectedCode = intval(trim(file_get_contents($tests["rc"][$i])));

    //nastavení cesty k dočasnému výstupnímu souboru
    $pathInfo = pathinfo($tests["src"][$i]);
    $tmpOutputFile = $pathInfo["dirname"]."/".$pathInfo["filename"]."_tmp.out";

    //parser 
    if(!$intOnly)
    {
        exec("php ".$parseScript. " < ".$tests["src"][$i]. "> ".$tmpOutputFile, $output, $parserCode); //provedení skriptu
        if($parserCode != $expectedCode)
        {
            //špatný návratový kód = neúspěšný test
            if($parseOnly)
            {
                print("
                <tr>
                    <td>".$i."</td><td>".strtok($tests["src"][$i],".")."</td><td>");
    
                print("<strong>Očekáváno ".$expectedCode.", obrdrženo ".$parserCode."</strong></td><td><span class=\"fail\">Neúspěch</span></td></tr>"); 
            }
        }
        else
        {
            //správný návratový kód

            if($parserCode == 0)
            {
                //pokud byl návratový kód 0, musí se provést kontrola stejnosti souborů
                exec("java -jar ".$jexamPath." ".$tmpOutputFile." ".$tests["out"][$i]." xml_diff.xml /D", $output, $jexamCode);

                if($jexamCode == 0)
                {
                    //soubory jsou identické
                    if($parseOnly)
                    {
                        print("
                        <tr>
                            <td>".$i."</td><td>".strtok($tests["src"][$i],".")."</td><td>");
                        print("</td><td><span class=\"success\">Úspěch</span></td></tr>");
                        $succCount++;
                    }
                }
                else
                {
                    //soubory nejsou identícké
                    if($parseOnly)
                    {
                        print("
                        <tr>
                            <td>".$i."</td><td>".strtok($tests["src"][$i],".")."</td><td>");
    
                        print("<strong>Výstupní soubory nejsou identické</strong></td><td><span class=\"fail\">Neúspěch</span></td></tr>");
                    }

                }
            }
            else
            {
                //správný návratový kód
                if($parseOnly)
                {
                    print("
                    <tr>
                        <td>".$i."</td><td>".strtok($tests["src"][$i],".")."</td><td>");
                    print("</td><td><span class=\"success\">Úspěch</span></td></tr>");
                    $succCount++;
                }
            }
            
        }
    }

    //interpret
    if(!$parseOnly)
    {   
        //nastavení vstupního souboru pro interpret. Pokud se má provádět test parseru i interpretu, pak je vstupní soubor pro interpret výstupní soubor parseru
        if($intOnly)
        {
            $tmpOutputFile = $tests["src"][$i];
        }
        exec("python3 ".$intScript." --source=".$tmpOutputFile. " --input=".$tests["in"][$i]. "> ".$tmpOutputFile."2", $output, $intCode);
        if($intCode != $expectedCode)
        {
            //špatný návratový kód = neúspěšný test
            print("
            <tr>
                <td>".$i."</td><td>".strtok($tests["src"][$i],".")."</td><td>");

            print("<strong>Očekáváno ".$expectedCode.", obrdrženo ".$intCode."</strong></td><td><span class=\"fail\">Neúspěch</span></td></tr>");
        }
        else
        {
            //správný návratový kód

            if($intCode == 0)
            {
                //pokud byl návratový kód 0, musí se provést kontrola stejnosti souborů
                $diff = shell_exec("diff ".$tmpOutputFile."2 ".$tests["out"][$i]);
                if($diff == "")
                {
                    //soubory jsou identícké
                    print("
                    <tr>
                        <td>".$i."</td><td>".strtok($tests["src"][$i],".")."</td><td>");
                    print("</td><td><span class=\"success\">Úspěch</span></td></tr>");
                    $succCount++;
                }
                else
                {
                    //soubory nejsou identícké
                    print("
                    <tr>
                        <td>".$i."</td><td>".strtok($tests["src"][$i],".")."</td><td>");

                    print("<strong>Výstupní soubory nejsou identické</strong></td><td><span class=\"fail\">Neúspěch</span></td></tr>");
                }
            }
            else
            {
                print("
                <tr>
                    <td>".$i."</td><td>".strtok($tests["src"][$i],".")."</td><td>");
                print("</td><td><span class=\"success\">Úspěch</span></td></tr>");
                $succCount++;
            }
        }
    }
    //pokud nebyl zadán noclean argument, provede se smazání dočasných souborů
    if(!$noClean)
    {
        if(file_exists($tmpOutputFile))
        {
            unlink($tmpOutputFile);
        }
        if(file_exists($tmpOutputFile."2"))
        {
            unlink($tmpOutputFile."2");
        }
    }
}



//výpočet a nastavení barvy úspěšnosti
$successRate = intval($succCount/count($tests["src"]) * 100);
if($successRate > 80)
{
    $color = "success";
}
else if($successRate < 80 && $successRate > 40)
{
    $color = "orange";
}
else
{
    $color = "fail";
}
print("<div class=\"result-container\">");
print("<p><b style=\"font-size: 1.5em;display:block;margin-bottom: 1em;\">Výsledky testů</b>Úspěšných: ".$succCount."<br>Neúspěšných: ".(count($tests["src"])-$succCount)."<br>Celkem: ".count($tests["src"])."</p>");
print("<span class=\"".$color."\">".$successRate." %</span>");
print("</div>");
htmlEnd();
?>

