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

// Příkaz pro spuštění výpočetu výběrové směrodatné odchylky:
// I. bez zadaného soubou
//     php profiling.php
//
// II. se zadaným souborem např. data.txt
//     php profiling.php < data.txt
//
// Note: Program není nutno překládat, pouze spustit z terminálů

include 'math_lib.php';

$calculator = new Calculator();

// reading file from stdin
if ($argc == 1)
$file = fopen("php://stdin", "r");
else if ($argc == 2)
$file = fopen($argv[1], "r");

$num = "";          // current loaded number
$nums = array();    // array to store data in
$num_count = 0;     // number of 
$blank_err = false; // for error handling purposes (invalid in this context)

// count all necessary values
while (($char = fgetc($file)) || true) {
    if (($char == " " || $char == "\n" || $char == "\t") && ($num = trim($num)) != "") {
        array_push($nums, $num);
        $num_count = $calculator->Sum($num_count, 1);
        $num  = "";
    } else { 
        $num = $num.$char;
        // read last value in case of num'EOF'
        if (feof($file) && ($num = trim($num)) != "") {
            array_push($nums, $num);
            $num_count = $calculator->Sum($num_count, 1);
        }
    }

    if (feof($file)) 
        break;
}

// count total sum of all numbers
$sum_nums = 0;
foreach ($nums as $val) 
    $sum_nums = $calculator->Sum($sum_nums, $val);
// average value
$artm_avg = $calculator->Divide($sum_nums, $num_count, $blank_err);

// compute inside of brackets
$vso = 0;
foreach ($nums as $val)
    $vso += $calculator->Power($calculator->Subtract($val, $artm_avg), 2, $blank_err);

// final deviation evaluation
$vso = $calculator->Root($calculator->Divide($vso, $num_count - 1, $blank_err), 2, $blank_err, $blank_err);

echo $vso;

fclose($file);