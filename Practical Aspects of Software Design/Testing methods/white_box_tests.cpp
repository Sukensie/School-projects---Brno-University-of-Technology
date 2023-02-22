//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     TOMÁŠ SOUČEK <xsouce15@stud.fit.vutbr.cz>
// $Date:       $2021-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author TOMÁŠ SOUČEK
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

class matice : public ::testing::Test
{
    protected:
        Matrix m {};
};


TEST(basicTest,matrixConstrucotr)
{
    Matrix x{};
    EXPECT_EQ(x.get(0,0), 0);
    EXPECT_ANY_THROW(x.get(1,1));

    EXPECT_ANY_THROW(Matrix x1 (0,0));
    EXPECT_ANY_THROW(Matrix x1 (0.8,0.5));
}

TEST(basicTest,matrixSet)
{
    Matrix x{2,2};
    EXPECT_FALSE( x.set(3,3,10));
    EXPECT_TRUE( x.set(1,1,-8));

    std::vector<std::vector <double>> vector =  {{20, 60,15},{85, 15,19},{16,17,55}};
    
    Matrix x1{3,3};
    x1.set(vector);

    EXPECT_EQ(x1.get(0,0),20);

    Matrix x2{1,1};
    EXPECT_FALSE(x2.set(vector));

}

TEST(basicTest,solveEquation)
{
   Matrix x1{3,2};

   std::vector<std::vector <double>> vector =  {{20, 60,15},{85, 15,19},{16,17,55}};
   x1.set(vector);
   std::vector <double> vysledky =  {1,2,3};

   EXPECT_ANY_THROW( x1.solveEquation(vysledky));

   Matrix x2{2,3};
   std::vector<std::vector <double>> vector2 =  {{20, 60,15},{85, 15,19}};
   x2.set(vector2);
   std::vector <double> vysledky2 =  {1,2,3};
   EXPECT_ANY_THROW( x2.solveEquation(vysledky));

   Matrix x3{3,3};
   std::vector<std::vector <double>> vector3 =  {{1, 4, 6},{8, 10,12},{14,16,18}};
   x3.set(vector3);
   std::vector <double> vysledky3 =  {1,2,3};

   std::vector <double> final = x3.solveEquation(vysledky3);
   EXPECT_EQ(final.at(0), 0);

   //nulový determinant
   Matrix x4{3,3};
   std::vector<std::vector <double>> vector4 =  {{2, 4, 6},{8, 10,12},{14,16,18}};
   x4.set(vector4);
   std::vector <double> vysledky4 =  {1,2,3};

   EXPECT_ANY_THROW(x4.solveEquation(vysledky4));

   //determinant 1
   Matrix x5{1,1};
   x5.set(0,0,1);
   std::vector <double> vysledky5 = {1};

   std::vector <double> final2 = x5.solveEquation(vysledky5);
   EXPECT_EQ(vysledky5.at(0),1);

   //matice větší než 3x3
   Matrix x6{5,5};
   std::vector<std::vector <double>> vector6 =  {{1, 4, 6,8,8},{8, 10,12,8,8},{14,16,18,7,7},{20,22,24,3,3},{26,28,30,5,7}};
   x6.set(vector6);
   std::vector <double> vysledky6 =  {4,2,3,4,5};

   std::vector <double> final3 = x6.solveEquation(vysledky6);
   EXPECT_EQ(final3.at(0), -3);

}

TEST(basicTest,matrixGet)
{
    Matrix x{2,2};
    x.set(0,0,7);
    EXPECT_EQ(x.get(0,0), 7);
    EXPECT_ANY_THROW(x.get(6,2));

     //x.~Matrix();
}

TEST(basicTest,matrixCompare)
{
    Matrix x{2,2};
    Matrix x1{2,2};
    x.set(0,0,7);
    x1.set(0,0,7);
    EXPECT_TRUE(x==x1);

    Matrix x2{2,2};
    x2.set(0,0,9);
    EXPECT_FALSE(x==x2);

    Matrix x3{3,3};
    x3.set(0,0,9);
    EXPECT_ANY_THROW(x==x3);
}

TEST(basicTest,matrixAdd)
{
    Matrix x{1,1};
    Matrix x1{1,1};
    x.set(0,0,7);
    x1.set(0,0,7);
    Matrix x3{1,1};
    x3.set(0,0,14);
    EXPECT_EQ(x.operator+(x1),x3);


    Matrix x2{3,3};
    x2.set(1,1,9);
    EXPECT_ANY_THROW(x.operator+(x2));
}
TEST(basicTest,matrixMultiply)
{
    Matrix x{2,2};
    x.set(0,0,2);
    x.set(0,1,9);
    x=x.operator*(3);
    EXPECT_EQ(x.get(0,0),6);
    EXPECT_EQ(x.get(0,1),27);

    Matrix x1{3,3};
    EXPECT_ANY_THROW(x.operator*(x1));

    Matrix x2{2,2};
    x2.set(0,0,2);
    x2.set(1,1,9);

    x.set(0,0,2);
    x.set(1,1,9);

    x2 = x2.operator*(x);
    EXPECT_EQ(x2.get(1,1),81);


}

TEST(basicTest,matrixTranspose)
{
    Matrix x{2,2};
    x.set(0,0,1);
    x.set(0,1,2);
    x.set(1,0,4);
    x.set(1,1,8);

    x = x.transpose();
    EXPECT_EQ(x.get(0,1),4);
}

TEST(basicTest,matrixInverse)
{
    Matrix x{4,4};
    EXPECT_ANY_THROW(x.inverse());

    Matrix x1 {2,2};
    x1.set(0,0,1);
    x1.set(0,1,2);
    x1.set(1,0,3);
    x1.set(1,1,4);
    x1 = x1.inverse();
    EXPECT_EQ(x1.get(0,0), -2);
    
   //nulový determinant
   Matrix x2 {3,3};
   x2.set(1,1,1);
   EXPECT_ANY_THROW(x2 =x2.inverse());
   
   //funkční inverzní
   Matrix x3 {3,3};
    x3.set(0,0,1);
    x3.set(1,1,2);
    x3.set(2,2,1);
    x3 = x3.inverse();
    EXPECT_EQ(x3.get(0,0), 1);
}


/*** Konec souboru white_box_tests.cpp ***/
