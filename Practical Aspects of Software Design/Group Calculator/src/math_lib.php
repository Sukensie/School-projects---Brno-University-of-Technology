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
 * Date: 9.4 2021\n
 * Last change: 26.4.2021\n
 * Description: A team project Calculator.\n
 * @author David Kocman xkocma08\n
 *         Tomáš Souček xsouce15\n
 *         Přemek Janda xjanda28\n
 *         Adam Cologna xcolog00\n
 * @version 1.0
 */

/**
 * Class description: Includes all basic arithmetical functions plus some extra
 * 
 * @author Přemek Janda xjanda28
 * @author David Kocman xkocma08\n
 * license: GNU GPLv3 
 */
class Calculator{

    /**
     * Divides two numbers.
     * @param int|float $x First variable
     * @param int|float $y Second variable
     * @param bool $errorDiv if second argument is 0, detects an error
     * @return int|float $x divided by $y 
     * @return bool an error indicator.
     */
    function Divide($x, $y, &$errorDiv) {
        if ($y == 0){
            $errorDiv = true;
            return $errorDiv;
        }
	    return $x / $y;
    }

    /**
     * Factorial of a number.
     * @param int|float $x First variable
     * @param bool $errorFact170 if argument is above 170, detects an error
     * @param bool $errorFact0 if second argument is below 0, detects an error
     * @param bool $errorEmpty an error indicator if string is empty
     * @return int|float Factorial of a number
     * @return bool an error indicator.
     */
    function Factorial($x, &$errorFact170, &$errorFact0, &$errorEmpty) {
        $w = 1;
        if ($x === null){
            $errorEmpty = true;
            return;
        }
        if ($x > 170){
            $errorFact170 = true;
            return $errorFact170;
        }
        if ($x < 0){
            $errorFact0 = true;
            return $errorFact0;
        }
        if ($x==1 || $x == 0)
            return 1;
        else {
            //do untill variable $i is equal to $x
            for ($i = 1; $i <= $x; $i++){
                $w = $w * $i;
            }
        }
        return $w;
    }

    /**
     * Multiplies two numbers.
     * @param int|float $x First variable
     * @param int|float $y Second variable
     * @return int|float $x multiplied by $y
     */
    function Multiply($x, $y) {
        return ($x * $y);
    }

    /**
     * Subtracts two numbers.
     * @param int|float $x First variable
     * @param int|float $y Second variable
     * @return int|float $x subtracted by $y
     */
    function Subtract($x, $y) {
        return ($x - $y);
    }

    /**
     * Adds two numbers.
     * @param int|float $x First variable
     * @param int|float $y Second variable
     * @return int|float $x added to $y
     */
    function Sum($x, $y) {
        return ($x + $y);
    }

    /**
     * Absolute value of a number.
     * @param int|float $x First variable
     * @param bool $errorEmpty an error indicator if string is empty
     * @return int|float Absolute value of $x
     * @return void if argument is null
     */
    function Absolute($x, &$errorEmpty) {
        if ($x === null) {
            $errorEmpty = true;
            return;
        }
        return abs($x);
    }

    /**
     * General power of a number.
     * @param int|float $x First variable, the base
     * @param int|float $y Second variable, the exponent
     * @param bool $errorEmpty an error indicator if string is empty
     * @return int|float $x to the power od $y
     * @return void if both arguments are null
     */
    function Power($x, $y, &$errorEmpty) {
        if (($x === null) || ($y === null)) {
            $errorEmpty = true;
            return;
        }
        return pow($x, $y);
    }

    /**
     * Root of a number.
     * @param int|float $x First variable, the base
     * @param int|float $y Second variable, the root
     * @param bool $errorRoot If a negative variable is rooted by even second variable, detects an error
     * @param bool $errorEmpty If the string is empty, detects an error
     * @return int|float $x rooted by $y
     * @return bool an error indicator
     * @return int zero if both arguments are null
     */
    function Root($x, $y, &$errorRoot, &$errorEmpty) {
        if (($x === null) || ($y === null)) {
            $errorEmpty = true;
            return;
        }
        if (($x < 0) && ($y % 2 == 0)){
            $errorRoot = true;
            return $errorRoot;
        }
        if ($x < 0){
            $x = $x * (-1);
            return pow($x, 1/$y)*(-1);
        } else {
            return pow($x,1/$y);
        }
    }
}
