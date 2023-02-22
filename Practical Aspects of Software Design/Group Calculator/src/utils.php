<?php
/*
A basic calculator created as a project for school by Kerbal Team. IVS L2021.
    Copyright (C) 2021 Kerbal Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    In case of malfunctions, do not hesitate to contact any of us.
    xjanda28@stud.fit.vutbr.cz, xkocma08@stud.fit.vutbr.cz, 
    xsouce15@stud.fit.vutbr.cz, xcolog00@stud.fit.vutbr.cz


*/

/**
 * @mainpage 
 * Project name: IVS-2. projekt\n
 * File: utils.php\n
 * Date: 9.4 2021\n
 * Last change: 9.4.2021\n
 * Description: Calculator utility functions.\n
 * @author David Kocman xkocma08\n
 *         Tomáš Souček xsouce15\n
 *         Přemek Janda xjanda28\n
 *         Adam Cologna xcolog00\n
 * @version 1.0
 */

/**
 * Class description: Calculator utilities used in main app.
 * 
 * @author Přemek Janda xjanda28
 * @author David Kocman xkocma08\n
 * license: GNU GPLv3 
 */
class Utilities{

    //  - - - - - - - - - - - - - - - - - - - - - //
    //  - - - - -  print history memory - - - - - //
    //  - - - - - - - - - - - - - - - - - - - - - //
    /**
     * Prints sorted history of calculators intermediate results
     * @param array<string,int> $arr Array storing intermediate results
     * @param int $count Pointer on top of history memory array
     * @param int $max Maximum number of elements in history memory
     */
    function print_hist_array ($arr, $count, $max) {
        for ($i = $count + 1; $i <= $count + $max; $i++) {
            if(empty($arr[$i%$max])) continue;
            echo "<p class=\"history\">".$arr[$i%$max]['assignment']." = ".$arr[$i%$max]['output']."</p>";
        }
    }
    
    
    //  - - - - - - - - - - - - - - - - - - - - - //
    //  - - - - string parsing functions - - - - - //
    //  - - - - - - - - - - - - - - - - - - - - - //
    /**
     * Finds out if input is a number or a character.
    * @param string $char The compared character
    * @return bool true in case character is a number or a dot, false if otherwise
    */
    function is_num($char) {
        return (is_numeric($char) || $char == ".") ? true : false;
    }
    
    /**
     * Increments pointer to the next character in a while loop.
    * @param int $pos position of the pointer in string
    * @param string $num string to store the data in
    * @param string $str string which is gone through
    */
    function inc(&$pos, &$num, $str) {
        $pos++;
        $num .= $str[$pos];
    }
    
    /**
     * Decrements pointer to the previous character in a while loop.
    * @param int $pos position of the pointer in string
    * @param string $num string to store the data in
    * @param string $str string which is gone through
    */
    function dec(&$pos, &$num, $str) {
        $pos--;
        $num = $str[$pos].$num;
    }
    
    /**
     * Finds out if haystack contains a needle from operators_arr
    * @param string $haystack String in which the needle is searched
    * @param string $operators_arr Array of operators with which the needle is compared to
    * @param string $needle The found operator
    * @param int $char_pos The position of current character in while and starting point
    */
    function find_in_str ($haystack, $operators_arr, &$needle, $char_pos) {
        while ($char_pos < strlen($haystack)) {
            if ($haystack[$char_pos] == $operators_arr[0]) {
                $needle = $operators_arr[0];
                return $char_pos;
            }
            if ($haystack[$char_pos] == $operators_arr[1]) {
                $needle = $operators_arr[1];
                return $char_pos;
            }
            $char_pos++;
        }
        return false;
    }
    
    /**
     * Function for the search of basic arithmetic operators such as + or *
    * @param string $str The string in which the operators are searched in
    * @return string String replaced with the result of the relevant arithmetic operator
    */
    function Basic_operators (&$str) {
        // setting up operatiors with priority
        $operators = array("x", "/");
        $errorDiv = false;
        $errors = false;
    
        $calculator = new Calculator();
        $utils = new Utilities();
        
        for ($i = $char_pos = 0; $i < 2; $i++) {
            while (($start_pos = $end_pos = $utils->find_in_str($str, $operators, $key, $char_pos)) !== false) {
                $num1 = $num2 = "";
    
                // insert numbers into 'num1' left side operand
                while ($start_pos > 0 && ($utils->is_num($str[$start_pos - 1]) || ($str[$start_pos - 1] == "-"))) {
                    $utils->dec($start_pos, $num1, $str);
                    if ($str[$start_pos] == "-") break;
                }
    
                // sign after main operation sign
                if ($end_pos < strlen($str) - 1)
                {
                    $sgn_bool = ($str[$end_pos + 1] == "-" || $str[$end_pos + 1] == "+") ? true : false;
                }
            
    
                // insert numbers into 'num2' right side operand
                while ($end_pos < strlen($str) - 1 && ($utils->is_num($str[$end_pos + 1]) || $sgn_bool)) {
                    $utils->inc($end_pos, $num2, $str);
                    $sgn_bool = false;
                }
    
                // in case of empty operand
                if (empty($num1)) $num1 = 0;
                if (empty($num2)) $num2 = 0;
                if ($char_pos == 0) $char_pos++;
    
                // in case of both negative values insert '+' sign before result
                $sgn = ($num1 < 0 && $num2 < 0) ? "+" : "";
                
                // choosing right function to execute
                switch ($key) {
                    case 'x': $str = substr_replace($str, $sgn.$calculator->Multiply($num1, $num2) , $start_pos, $end_pos - $start_pos + 1); break;
                    case '/': 
                        $str = substr_replace($str, $sgn.$calculator->Divide($num1, $num2, $errorDiv), $start_pos, $end_pos - $start_pos + 1); 
                        if($errorDiv == true){
                            $str = 'Division by 0!';
                            $errors = true;
                            return $str;
                        }
                        break;
                    case '+': $str = substr_replace($str, $sgn.$calculator->Sum($num1, $num2), $start_pos, $end_pos - $start_pos + 1); break;
                    case '-': $str = substr_replace($str, $sgn.$calculator->Subtract($num1, $num2), $start_pos, $end_pos - $start_pos + 1); break;
                }
    
            }
            // change of operators for second loop
            $operators = array("+", "-");
        }
        
        // trim unnecessary '+' sign
        if(!empty($str))
        {
            $str[0] = ($str[0] == "+") ? " " : $str[0];
        }
        
        return trim($str);
    }
}


//  - - - - - - - - - - - - - - - - - - - - - //
//  - - - - -  parsing input string - - - - - //
//  - - - - - - - - - - - - - - - - - - - - - //

// getting rid of url special character '+', '×' and white space ' '
$char_rmv  = array("+", "%C3%97", "%E2%88%9A", "%E2%88%92", "%20", "%3C/span%3E", "%3Cspanclass=%22wrap%22%3E");
$char_rplc = array("%2B", "x", "v", "-", "", "", "");
$input  = urldecode(str_replace($char_rmv, $char_rplc, substr($_SERVER['QUERY_STRING'], 6)));
$output = ""; $i = 0;
$utils = new Utilities();

// parse input string and remove html entities
while ($i < strlen($input)) {
    switch ($input[$i]) {
        // escape html tags of square root and power
        case '<':
            $end = 0; $bracket = "";	// auxiliary variables
            while ($end < 2) {
                switch ($input[$i]) {
                    case '>':
                        // adding appropriate bracket
                        if ($end) { $output .= $bracket; $i--; }
                        // values inside tag
                        else { while ($input[++$i] != "<") { $output .= ($input[$i] == ",") ? "." : $input[$i];} }
                        $end++; break;

                    // selecting bracket to be inserted
                    case '_': $output .= "("; $bracket = ")"; break;
                    case '^': $output .= "["; $bracket = "]"; break;
                }
                $i++;
            }
            break;

        // insert characters into final string
        default:
            // convert to US floating point
            if ($input[$i] == ",") { $output .= "."; }
            else { $output .= $input[$i]; }
        break;
    }
    $i++;
}

// adding power sign
$output = str_replace(")[", ")^[", $output);

// adding implicit multiplication signs between brackets
$brackets_types_left  = array("[", "(");
$brackets_types_right = array("]", ")", "!");
$search_arr  = array( "][",  ")(",  ")|",  "|(",  "]|",  "|[",  "](",  "!|");
$replace_arr = array( "]x[", ")x(", ")x|", "|x(", "]x|", "|x[", "]x(", "!x|");
$output = str_replace($search_arr, $replace_arr, $output);

// simplifying signs
$search_arr  = array("--", "+-", "+-", "*");
$replace_arr = array("+",  "-",  "-",  "x");
$output = str_replace($search_arr, $replace_arr, $output);
$output = str_replace($search_arr, $replace_arr, $output);


// adding implicit multiplication signs between number and bracket
for ($i = 0; $i < strlen($output) - 1; $i++) {
    // LEFT-side bracket
    // 42(  ->  42x(
    if (is_numeric($output[$i]) && in_array($output[$i + 1], $brackets_types_left))
        $output = substr_replace($output, $output[$i]."x".$output[$i + 1], $i, 2);

    // RIGHT-side bracket (including factorial '!')
    // )42  ->  )x42
    if (is_numeric($output[$i + 1]) && in_array($output[$i], $brackets_types_right))
        $output = substr_replace($output, $output[$i]."x".$output[$i + 1], $i, 2);

    if ($output[$i] == "|") {
        // if possible insert multiplication sign BEFORE '|' bracket
        // 42|  ->  42x|
        if ($i > 0 && is_numeric($output[$i - 1])) {
            $output = substr_replace($output, $output[$i - 1]."x".$output[$i], $i - 1, 2);
            $i++;
        }

            // skiping inside of asbolute value
            do { $i++; } while ($output[$i] != "|" && $i < strlen($output));


        // if possible insert multiplication sign AFTER '|' bracket
        // |42  ->  |x42
        if ($i < strlen($output) && is_numeric($output[$i + 1]))  {
            $output = substr_replace($output, $output[$i]."x".$output[$i + 1], $i, 2);
            $i++;
        }
    }
}

// load intermediate calculation into calculator's memory
$result = str_replace("x", "×", str_replace("v", "√", str_replace("-", "−", $output)));
$input = urldecode(str_replace("+", "%2B", substr($_SERVER['QUERY_STRING'], 6)));


// - - - - - - - - - - - - - - - - - - - - - - - - - //
// - - - - -  special functions evaluation - - - - - //
// - - - - - - - - - - - - - - - - - - - - - - - - - //

// declaration of the Calculator class
$calculator = new Calculator(); 
$errorFact170 = false;
$errorFact0 = false;
$errorRoot = false;
$errorEmpty = false;
$errors = false;


// factorial
while (($char_pos = $start_pos = strpos($output, "!")) !== false) {
    $number = "";
    while ($utils->is_num($output[$char_pos - 1]) && $char_pos > 0)
        $utils->dec($char_pos, $number, $output);

    $output = substr_replace($output, $calculator->Factorial($number, $errorFact170, $errorFact0, $errorEmpty), $char_pos, $start_pos - $char_pos + 1);
    if ($errorFact170 == true){
        $output = 'Highest factorial is 170!';
        $errors = true;
        break;
    } elseif ($errorFact0 == true) {
        $output = 'Negative factorial!';
        $errors = true; 
        break;
    } elseif ($errorEmpty == true) {
        $output = 'Empty field!';
        $errors = true; 
        break;
    }
}

// absolute value
while (($char_pos = $end_pos = strpos($output, "|")) !== false) {
    $number = "";

    while ($end_pos < strlen($output) - 1 && $output[$end_pos + 1] != "|")
        $utils->inc($end_pos, $number, $output);

    $output = substr_replace($output, $calculator->Absolute($utils->Basic_operators($number), $errorEmpty), $char_pos, $end_pos - $char_pos + 2);
    if ($errorEmpty == true) {
        $output = 'Empty field!';
        $errors = true; 
        break;
    }
    if (++$end_pos >= strlen($output)) break;
}

// power
while (($start_pos = $end_pos = strpos($output, "^")) !== false) {
    $num1 = $num2 = "";
    $start_pos--; $end_pos++;

    // left side
    while ($start_pos > 0 && $output[$start_pos - 1] != "(")
        $utils->dec($start_pos, $num1, $output);

    // right side
    while ($end_pos < strlen($output) - 1 && $output[$end_pos + 1] != "]")
        $utils->inc($end_pos, $num2, $output);

    $output = substr_replace($output, $calculator->Power($utils->Basic_operators($num1), $utils->Basic_operators($num2), $errorEmpty), $start_pos - 1, $end_pos - $start_pos + 3);
    if ($errorEmpty == true) {
        $output = 'Empty field!';
        $errors = true; 
        break;
    }
}

// root function
while (($start_pos = $end_pos = strpos($output, "v")) !== false) {
    $num1 = $num2 = "";
    $start_pos--; $end_pos++;

    // left side
    while ($start_pos > 0 && $output[$start_pos - 1] != "[")
        $utils->dec($start_pos, $num1, $output);

    // right side
    while ($end_pos < strlen($output) - 1 && $output[$end_pos + 1] != ")")
        $utils->inc($end_pos, $num2, $output);

    $output = substr_replace($output, $calculator->Root($utils->Basic_operators($num2), $utils->Basic_operators($num1), $errorRoot, $errorEmpty), $start_pos - 1, $end_pos - $start_pos + 3);
    if ($errorRoot == true) {
        $output = "Even root can't be of a negative number!";
        $errors = true;
        break;
    } elseif ($errorEmpty == true) {
        $output = 'Empty field!';
        $errors = true; 
        break;
    }
}

//  - - - - - - - - - - - - - - - - - - - - - - - - - - - //
//  - - - - - final basic operations evaluation - - - - - //
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - //

$utils->Basic_operators($output);
$output = str_replace("-", "−", $output);

?>
