//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     TOMÁŠ SOUČEK <xsouce15@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author TOMÁŠ SOUČEK
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//


class EmptyTree : public ::testing::Test
{
    protected:
        BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test
{
    protected:
        virtual void SetUp() {
            int values[] = { 10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55 };

            for(int i = 0; i < 14; ++i)
                tree.InsertNode(values[i]);
        }

        BinaryTree tree;
};


TEST_F(EmptyTree, InsertNode)
{
    auto result = tree.InsertNode(23);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second->key,23);
    auto result2 = tree.InsertNode(23);
    EXPECT_FALSE(result2.first);
    EXPECT_EQ(result.second, result2.second);
}

TEST_F(EmptyTree, DeleteNode)
{
    auto number = tree.InsertNode(23);
    EXPECT_TRUE(tree.DeleteNode(23));
    EXPECT_FALSE(tree.DeleteNode(18));
}

TEST_F(EmptyTree, FindNode)
{
    auto number = tree.InsertNode(66);
    EXPECT_EQ(tree.FindNode(66), number.second);
    EXPECT_EQ(tree.FindNode(81), nullptr);
}

TEST_F(NonEmptyTree, InsertNode)
{
     auto result = tree.InsertNode(23);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second->key,23);
    auto result2 = tree.InsertNode(90);
    EXPECT_FALSE(result2.first);

}

TEST_F(NonEmptyTree, DeleteNode)
{
    //testuj hodnoty které jsou vloženy při inicializaci
    EXPECT_TRUE(tree.DeleteNode(90));
    EXPECT_FALSE(tree.DeleteNode(18));
}

TEST_F(NonEmptyTree, FindNode)
{
    //testuj hodnoty které jsou vloženy při inicializaci
    EXPECT_TRUE(tree.FindNode(85));
    EXPECT_FALSE(tree.FindNode(66));
}


TEST(TreeAxioms, Axiom1)
{
    BinaryTree tree;
    int values[] = { 10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55 };

    for(int i = 0; i < 14; ++i)
        tree.InsertNode(values[i]);

    

    std::vector<Node_t *> vector {};
    tree.GetLeafNodes(vector);

     for(auto node : vector)
     {
         EXPECT_EQ(node->color, BLACK);
     }   
}

TEST(TreeAxioms, Axiom2)
{
    BinaryTree tree;
    int values[] = { 10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55 };

    for(int i = 0; i < 14; ++i)
        tree.InsertNode(values[i]);

    
    std::vector<Node_t *> vector {};
    tree.GetAllNodes(vector);

     for(auto node : vector)
     {
            if(node->color == RED)
            {
                EXPECT_EQ(node->pLeft->color, BLACK);
                EXPECT_EQ(node->pRight->color, BLACK);
            }
     }
    
}

/*** Konec souboru black_box_tests.cpp ***/
