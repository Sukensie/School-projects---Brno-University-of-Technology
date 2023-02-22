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
require '_DIR_' . "/../math_lib.php";

/**
 * Tests all basic calculator functions
 * @author David Kocman xkocma08
 */
class Test extends \PHPUnit\Framework\TestCase
{
    private $kalkulacka;

    /**
     * Test case for adding two numbers
     */
    public function testAdd()
    {
        $kalkulacka = new Calculator();
        //cela cisla
        $this->assertEquals(2, $kalkulacka->Sum(1,1));
        $this->assertEquals(90, $kalkulacka->Sum(40,50));
        $this->assertEquals(-20, $kalkulacka->Sum(30,-50));
        $this->assertEquals(20, $kalkulacka->Sum(-40,60));
        //desetinna cisla
        $this->assertEqualsWithDelta(6.6, $kalkulacka->Sum(1.6, 5), 0.1);
        $this->assertEqualsWithDelta(6.6, $kalkulacka->Sum(5, 1.6), 0.1);
        $this->assertEqualsWithDelta(0.6, $kalkulacka->Sum(0.3, 0.3), 0.1);
        $this->assertEqualsWithDelta(0.4878, $kalkulacka->Sum(0.125, 0.3628), 0.0001);
        $this->assertEqualsWithDelta(2/3, $kalkulacka->Sum(1/3, 1/3), 0.01);
        $this->assertEqualsWithDelta(3/3, $kalkulacka->Sum(2/3, 1/3), 0.01);
        $this->assertEqualsWithDelta(1/3, $kalkulacka->Sum(2/3, -1/3), 0.01);
    }
    /**
     * Test case for subtracting two numbers
     */
    public function testSub()
    {
        $kalkulacka = new Calculator();
        //cela cisla
        $this->assertEquals(0, $kalkulacka->Subtract(1,1));
        $this->assertEquals(-10, $kalkulacka->Subtract(40,50));
        $this->assertEquals(80, $kalkulacka->Subtract(30,-50));
        $this->assertEquals(-100, $kalkulacka->Subtract(-40,60));
        //desetinna cisla
        $this->assertEqualsWithDelta(-3.4, $kalkulacka->Subtract(1.6, 5), 0.1);
        $this->assertEqualsWithDelta(3.4, $kalkulacka->Subtract(5, 1.6), 0.1);
        $this->assertEqualsWithDelta(0, $kalkulacka->Subtract(0.3, 0.3), 0.1);
        $this->assertEqualsWithDelta(-0.2378, $kalkulacka->Subtract(0.125, 0.3628), 0.0001);
        $this->assertEqualsWithDelta(0, $kalkulacka->Subtract(1/3, 1/3), 0.1);
        $this->assertEqualsWithDelta(1/3, $kalkulacka->Subtract(2/3, 1/3), 0.01);
        $this->assertEqualsWithDelta(3/3, $kalkulacka->Subtract(2/3, -1/3), 0.01);
        $this->assertEqualsWithDelta(0.9, $kalkulacka->Subtract(0.6, -0.3), 0.1);
    }
    /**
     * Test case for multiplying two numbers
     */
    public function testMultiply()
    {
        $kalkulacka = new Calculator();
        //cela cisla
        $this->assertEquals(2, $kalkulacka->Multiply(1,2));
        $this->assertEquals(20, $kalkulacka->Multiply(2,10));
        $this->assertEquals(180, $kalkulacka->Multiply(15,12));
        $this->assertEquals(36, $kalkulacka->Multiply(18,2));
        $this->assertEquals(-32, $kalkulacka->Multiply(16,-2));
        $this->assertEquals(-80, $kalkulacka->Multiply(-8,10));
        $this->assertEquals(180, $kalkulacka->Multiply(-9,-20));
        //desetinna cisla
        $this->assertEqualsWithDelta(0.1, $kalkulacka->Multiply(0.1, 1), 0.1);
        $this->assertEqualsWithDelta(0, $kalkulacka->Multiply(0.5, 0), 0.1);
        $this->assertEqualsWithDelta(1/9, $kalkulacka->Multiply(1/3, 1/3), 0.01);
        $this->assertEqualsWithDelta(0.2048, $kalkulacka->Multiply(0.256, 0.8), 0.0001);
        $this->assertEqualsWithDelta(0.363, $kalkulacka->Multiply(0.55, 0.66), 0.001);
    }
    /**
     * Test case for dividing two numbers
     */
    public function testDivide()
    {
        $kalkulacka = new Calculator();
        //cela cisla
        $error = false;
        $this->assertEquals(2, $kalkulacka->Divide(10,5, $error));
        $this->assertEquals(4, $kalkulacka->Divide(-12,-3, $error));
        $this->assertEquals(4, $kalkulacka->Divide(8,2, $error));
        $this->assertEquals(-2, $kalkulacka->Divide(-8,4, $error));
        //cela cisla se zbytkem
        $this->assertEqualsWithDelta(2.5, $kalkulacka->Divide(20, 8, $error), 0.1);
        $this->assertEqualsWithDelta(6.5, $kalkulacka->Divide(13, 2,$error), 0.1);
        //desetinna cisla
        $this->assertEqualsWithDelta(1, $kalkulacka->Divide(0.5, 0.5, $error), 0.1);
        $this->assertEqualsWithDelta(2.6, $kalkulacka->Divide(0.26, 0.1, $error), 0.1);
    }
    /**
     * Test case for division exception
     */
    public function testDivideException()
    {
        $kalkulacka = new Calculator();
        $error = false;
        //exception->deleni nulou
        $this->assertTrue($kalkulacka->Divide(10, 0, $error));
    }
    /**
     * Test case for factorial
     */
    public function testFactorial()
    {
        $kalkulacka = new Calculator();
        $error170 = false;
        $error0 = false;
        $errorEmpty = false;
        $this->assertEquals(1, $kalkulacka->Factorial(0, $error170, $error0, $errorEmpty));
        $this->assertEquals(120, $kalkulacka->Factorial(5, $error170, $error0, $errorEmpty));
    }
    /**
     * Test case for factorial exception is factorial is less than 0
     */
    public function testFactorialException()
    {
        $kalkulacka = new Calculator();
        $error170 = false;
        $error0 = false;
        $errorEmpty = false;
        //zaporny faktorial
        $this->assertTrue($kalkulacka->Factorial(-1, $error170, $error0, $errorEmpty));
    }
    /**
     * Test case for factorial exception is factorial is greater than 170
     */
    public function testFactorialException2()
    {
        $kalkulacka = new Calculator();
        $error170 = false;
        $error0 = false;
        $errorEmpty = false;
        //faktorial vetsi nez 170
        $this->assertTrue($kalkulacka->Factorial(171, $error170, $error0, $errorEmpty));
    }
    /**
     * Test case for power
     */
    public function testPowerOf()
    {
        $kalkulacka = new Calculator();
        $errorEmpty = false;
        //cela cisla
        $this->assertEquals(1, $kalkulacka->Power(2,0, $errorEmpty));
        $this->assertEquals(8, $kalkulacka->Power(2,3, $errorEmpty));
        $this->assertEquals(4, $kalkulacka->Power(2,2 , $errorEmpty));
        $this->assertEquals(4, $kalkulacka->Power(-2,2,$errorEmpty));
        $this->assertEquals(1, $kalkulacka->Power(-2,0,$errorEmpty));
        $this->assertEquals(-27, $kalkulacka->Power(-3,3,$errorEmpty));
        $this->assertEqualsWithDelta(0.25, $kalkulacka->Power(2,-2,$errorEmpty), 0.01);
        $this->assertEqualsWithDelta(0.125, $kalkulacka->Power(2,-3,$errorEmpty), 0.001);
        //cela cisla se zbytkem
        $this->assertEqualsWithDelta(0.04, $kalkulacka->Power(0.2, 2,$errorEmpty), 0.01);
    }
    /**
     * Test case for any root
     */
    public function testRoot()
    {
        $kalkulacka = new Calculator();
        $error = false;
        $errorEmpty = false;
        //cela cisla
        $this->assertEquals(2, $kalkulacka->Root(4,2, $error,$errorEmpty));
        $this->assertEquals(2, $kalkulacka->Root(8,3, $error,$errorEmpty));
        $this->assertEquals(-3, $kalkulacka->Root(-27,3, $error,$errorEmpty));
        $this->assertEquals(0.5, $kalkulacka->Root(4,-2, $error,$errorEmpty));
        $this->assertEquals(5, $kalkulacka->Root(3125,5,$error,$errorEmpty));
        $this->assertEquals(2, $kalkulacka->Root(64,6, $error,$errorEmpty));
    }
    /**
     * Test case for even root exception
     */
    public function testRtException()
    {
        $kalkulacka = new Calculator();
        $error = false;
        $errorEmpty = false;
        $this->assertTrue($kalkulacka->Root(-4,2,$error, $errorEmpty));
    }
    /**
     * Test case for absolute value
     */
    public function testAbs()
    {
        $kalkulacka = new Calculator();
        $errorEmpty = false;
        $this->assertEquals(58, $kalkulacka->Absolute(58, $errorEmpty));
        $this->assertEquals(12, $kalkulacka->Absolute(-12,$errorEmpty));
        
    }
    /**
     * Test case for strings of numbers
     */
    public function testString(){
        $kalkulacka = new Calculator();
        $error = false;
        $error170 = false;
        $error0 = false;
        $errorEmpty = false;
        //test vyrazu
        $this->assertEquals(10, $kalkulacka->Absolute($kalkulacka->Sum($kalkulacka->Multiply(-10, 20), 190),$errorEmpty));
        $this->assertEquals(2, $kalkulacka->Root($kalkulacka->Subtract($kalkulacka->Multiply(2, $kalkulacka->Absolute(-3,$errorEmpty)),2),2, $error,$errorEmpty));
        $this->assertEquals(13, $kalkulacka->Sum(10, $kalkulacka->Root($kalkulacka->Divide(108,4, $error),3,$error,$errorEmpty)));
        $this->assertEquals(15511210043330985984000000, $kalkulacka->Factorial($kalkulacka->Power(5, 2,$errorEmpty), $error170, $error0,$errorEmpty));
        $this->assertEquals(4, $kalkulacka->Power($kalkulacka->Root($kalkulacka->Sum(2, 2), 2, $error,$errorEmpty),2,$errorEmpty));
    }
}
