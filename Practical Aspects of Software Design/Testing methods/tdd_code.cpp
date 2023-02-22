//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     TOMÁŠ SOUČEK <xsouce15@stud.fit.vutbr.cz>
// $Date:       $2021-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author TOMÁŠ SOUČEK
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{

}

PriorityQueue::~PriorityQueue()
{
    Element_t * current  = m_pHead;
    Element_t * next;

    while(current != NULL)
    {
        next = current->pNext;

        delete(current);
        current = next;
    }   

    
}

void PriorityQueue::Insert(int value)
{
    if(m_pHead == NULL)
    {
        m_pHead = new Element_t{
            .pNext = nullptr,
            .value = value
        };
    }
    else
    {
        Element_t * current = m_pHead;
        Element_t * previous = current;

        bool konec = false;

        //vložení největšího prvku na začátek
        if(current->value <= value)
        {
            Element_t * inserted = new Element_t{
                .pNext = m_pHead->pNext,
                .value = value
            };

            m_pHead->pNext = inserted;

            int help = m_pHead->value;
            m_pHead->value = inserted->value;
            inserted->value = help;
            return;
        }
   
        while(current != nullptr)//loop pokud mám vložit prvek někam doprostřed
        {
           if(value >= current->value)//vložení prvku
            {
                //1)
                Element_t * inserted = new Element_t{
                    .pNext = current,
                    .value = value
                 };
            

                //2)
                previous->pNext = inserted;
                konec = true;
                return;
            }
          
                if(current->pNext != nullptr)
                {
                    previous = current;//než se posunu na další prvek, chci si zapamatovat současný...krokování o krok zpět
                     current = current->pNext;//posun na nový prvek
                }
                else
                {
                    break;
                }
        }

        if(konec == false)
        {
            current->pNext = new Element_t{ //vložení nejmenšího prvku na konec listu
                .pNext = nullptr,
                .value = value
            };
        }
    }
}

bool PriorityQueue::Remove(int value)
{
    //pokud je list prázdný
    if(m_pHead == NULL)
    {
        return false;
    }

    //pokud je jediný prvek
    if(m_pHead->pNext == nullptr)
    {
        delete(m_pHead);
        m_pHead = NULL;
        return true;
       
    }
    Element_t *current = m_pHead;
    Element_t *previous = current;
    while(current != nullptr)
    {
        if(value == current->value)
        {
            //pokud je hledaný prvek největší
            if(value == m_pHead->value)
            {
                m_pHead = NULL;
                m_pHead = current->pNext;
                return true;
            }
            previous->pNext = current->pNext;//spojí list tak aby nevznikla díra

            delete(current);         

            return true;
        }
        previous = current;
        current = current->pNext;
    }
   
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    Element_t* help = m_pHead;

    while (help != NULL)
    {
        if (value == help->value)
        {
            return help;
        }
        if(help != NULL)
        {
            help = help->pNext;
        }
    }
    return NULL;
}

size_t PriorityQueue::Length()
{
	size_t length = 0;
    Element_t* help = m_pHead;
    if(m_pHead == NULL)
    {
        return length;
    }
  
    while (help->pNext != NULL)
    {
        length++;
        help = help->pNext;
    }
    if(help != NULL)
    {
        length++;
    }
    return length;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/
