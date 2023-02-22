<?php

//Odstraní vše za # do konce řádku
function deleteComments($line)
{
    $commentPositon = strpos($line, "#");
    if($commentPositon !== FALSE) //musí být !== aby byl bool false, když je v commentPosition 0, tak se z toho dělal false a to jsem nechtěl (pozice 0 je validní pro odstranění celého řádku začínající na #)
    {
        $line = substr($line, 0, $commentPositon);
    }
    return $line;
}

//zkontroluje, jestli je daný počet argumentů instrukce správný
function checkNumArguments($array, $number)
{
    if(count($array) > $number+1)
    {
        fwrite(STDERR, "nesprávný počet argumentů pro funkci ".$array[0]."\n");
        exit(23);
    }
}

//pomocí regulárního výrazu zkontroluje validitu zaslaného argumentu. V případě nevalidního, ukončí běh programu
function getArgumentType($array, $position, $expected)
{
    switch ($expected)
    {
        case "var":
            if(preg_match("/^(LF|TF|GF)@[áčďéěíňóřšťůúýžÁČĎÉĚÍŇÓŘŠŤŮÚÝŽa-zA-Z_\-\$&;\/%\*\!\?][áčďéěíňóřšťůúýžÁČĎÉĚÍŇÓŘŠŤŮÚÝŽa-zA-Z_\-\$&;\/%\*\!\?0-9]*$/",$array[$position]))
            {
                return "var";
            }
            break;

        case "symb":
            if(preg_match("/^(int|bool|string|nil|LF|TF|GF)@[áčďéěíňóřšťůúýžÁČĎÉĚÍŇÓŘŠŤŮÚÝŽa-zA-Z_\-\$&;\/@%\*\!\?0-9(nil)\.\\\\]*$/",$array[$position]))
            {
                return "symb";
            }
            break;

        case "label":
            if(preg_match("/^[áčďéěíňóřšťůúýžÁČĎÉĚÍŇÓŘŠŤŮÚÝŽa-zA-Z_\-\$&;\/%\*\!\?][áčďéěíňóřšťůúýžÁČĎÉĚÍŇÓŘŠŤŮÚÝŽa-zA-Z_\-\$&;\/\*\!\?0-9]*$/",$array[$position]))
            {
                return "label";
            }
            break;
    }

    fwrite(STDERR,"špatně zadaný argument\n");
    exit(23);
    
}

//Argument <symb> může přijmout proměnnou, nebo konstantu. Funkce rozhdodne pomocí reg. výrazu o co se jedná a podle toho vytiskne argument na stdout. Používám u <symb> argumentů
function varOrSymb($array, $argNum)
{
    if(preg_match("/^(LF|TF|GF)@/",$array[$argNum])) //pokud narazíš na LF, TF, GF, pak je to var... jinak je to symb
    {
        echo "        <arg".$argNum." type=\"var\">".$array[$argNum]."</arg".$argNum.">\n";
        return;
    }
    $symbVal = explode("@",$array[$argNum],2); //rozděl jen do prvního výskytu @
    echo "        <arg".$argNum." type=\"".$symbVal[0]."\">".$symbVal[1]."</arg".$argNum.">\n";
}

//zobrazí informace o skriptu
function printHelp()
{
    echo "Skript vezme vstup v jazyce IPPcode22 ze standartního vstupu a vypíše na standartní výstup XML reprezentaci programu\n\n";
    echo "Skript je možno doplnit o následující parametry:\n\n--help\tzobrazí nápovědu\n";
    exit(0);
}

ini_set('display_errors', 'stderr');

if($argc > 1)
{
    if($argv[1] == "--help")
    {
        printHelp();
    }
    else
    {
        exit(10);
    }
}


echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"; //povinná hlavička

$order = 1;
$header = false;


while($line = fgets(STDIN))
{
    $argNum = 1;
    $type = "";
    $line = trim($line);
    $line = deleteComments($line);

    //kontrola prázdného řádku po odstranění komentářů, pokud prázdný pokračuje skript načtením nového řádku
    if(strlen($line) < 1)
    {
        continue;
    }

    //oprava specialníách XML znaků
    $line = str_replace("&","&amp;",$line);
    $line = str_replace("<","&lt;",$line);
    $line = str_replace(">","&gt;",$line);
    $line = str_replace("'","&apos;",$line);
    $line = str_replace("\"","&quot;",$line);

    //nahrazení více mezer na jednu, aby s tím nebyl problém v explode()
    $line = preg_replace('/\s+/', ' ', $line);


    //rozdělení řádku na instrukci a argumenty
    $array = explode(" ", trim($line));


    //kontrola hlavičky ippcode22
    if(!$header)
    {
        if(strtoupper($array[0]) == strtoupper(".IPPcode22"))
        {
            $header = true;
            echo "<program language=\"IPPcode22\">\n"; 
            continue;
        }
        else
        {
            fwrite(STDERR, "chybná nebo chybějící hlavička\n");
            exit(21);
        }
    }

   

    echo "    <instruction order=\"".$order."\" opcode=\"".$array[0]."\">\n";


    //rozdělení instrukcí podle očekávaných argumentů
    switch(strtoupper($array[0]))
    {
        //<var>
        case "DEFVAR":
        case "POPS":
            checkNumArguments($array,1);

            $type = getArgumentType($array, $argNum, "var");
            echo "        <arg".$argNum." type=\"var\">".$array[$argNum]."</arg".$argNum.">\n";
            break;
       
            
        //<label>
        case "CALL":
        case "LABEL":
        case "JUMP":
            checkNumArguments($array,1);

            $type = getArgumentType($array, $argNum, "label");
            echo "        <arg".$argNum." type=\"label\">".$array[$argNum]."</arg".$argNum.">\n";
            break;


        //<symb>
        case "PUSHS":
        case "WRITE":
        case "EXIT":
        case "DPRINT":
            checkNumArguments($array,1);
            $type = getArgumentType($array, $argNum, "symb");
            varOrSymb($array, $argNum);
            break;


        /* ------------- 2 ARGUMENTY ------------------*/

        //<var> <type>
        case "READ":
            checkNumArguments($array,2);

            $type = getArgumentType($array, $argNum, "var");
            echo "        <arg".$argNum." type=\"var\">".$array[$argNum]."</arg".$argNum.">\n";
            $argNum++;

            //kontrola jestli <type> nabývá správných hodnot
            if(!preg_match("/^(int|string|bool)$/",$array[$argNum]))
            {
                fwrite(STDERR, "špatný typ argumentu");
                exit(23);
            }
            echo "        <arg".$argNum." type=\"type\">".$array[$argNum]."</arg".$argNum.">\n";
            break;


        //<var> <symb>
        case "MOVE":
        case "INT2CHAR":
        case "STRLEN":
        case "TYPE":
        case "NOT":
            checkNumArguments($array,2);

            $type = getArgumentType($array, $argNum, "var");
            echo "        <arg".$argNum." type=\"var\">".$array[$argNum]."</arg".$argNum.">\n";
            $argNum++;

            $type = getArgumentType($array, $argNum, "symb");
            varOrSymb($array, $argNum);
            break;


        /* ------------- 3 ARGUMENTY ------------------*/

        //<var> <symb> <symb>
        case "ADD":
        case "SUB":
        case "MUL":
        case "IDIV":
        case "LT":
        case "GT":
        case "EQ":
        case "AND":
        case "OR":
        case "STRI2INT":
        case "CONCAT":
        case "GETCHAR":
        case "SETCHAR":
            checkNumArguments($array,3);

            $type = getArgumentType($array, $argNum, "var");
            echo "        <arg".$argNum." type=\"var\">".$array[$argNum]."</arg".$argNum.">\n";
            $argNum++;

            $type = getArgumentType($array, $argNum, "symb");
            varOrSymb($array, $argNum);
            $argNum++;

            $type = getArgumentType($array, $argNum, "symb");
            varOrSymb($array, $argNum);
            break;


        //<label> <symb> <symb>
        case "JUMPIFEQ": 
        case "JUMPIFNEQ":   
            checkNumArguments($array,3);

            $type = getArgumentType($array, $argNum, "label");
            echo "        <arg".$argNum." type=\"label\">".$array[$argNum]."</arg".$argNum.">\n";
            $argNum++;

            $type = getArgumentType($array, $argNum, "symb");
            varOrSymb($array, $argNum);
            $argNum++;

            $type = getArgumentType($array, $argNum, "symb");
            varOrSymb($array, $argNum);
            break;


        //bez argumentů
        case "CREATEFRAME":
        case "PUSHFRAME":
        case "POPFRAME":
        case "RETURN":
        case "BREAK":
            checkNumArguments($array, 0);
            break;
        
        default:
            fwrite(STDERR,"chybný operační kód\n");
            exit(22);
    }
    
    echo "    </instruction>\n";
    $order++;
}

echo "</program>\n";
?>