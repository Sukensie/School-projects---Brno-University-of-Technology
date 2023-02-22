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
 * File: calc_script.js\n
 * Date: 25.4 2021\n
 * Last change: 25.4.2021\n
 * Description: File containing javacript used for this project\n
 * @author David Kocman xkocma08\n
 *         Tomáš Souček xsouce15\n
 *         Přemek Janda xjanda28\n
 *         Adam Cologna xcolog00\n
 * @version 1.0
 */

/**
 * Positions cursor inside certain HTML element 
 * @param tag HTML element inside which will be cursor positioned
 * 
 */
function position_cursor(tag) 
{        				
    // Creates range object
    let setpos = document.createRange();
    
    // Creates object for selection
    let set = window.getSelection();
    
    // Set start position of range
    setpos.setStart(tag.childNodes[0], 0);
    
    // Collapse range within its boundary points
    // Returns boolean
    setpos.collapse(true);
    
    // Remove all ranges set
    set.removeAllRanges();
    
    // Add range with respect to range object.
    set.addRange(setpos);
    
    // Set cursor on focus
    tag.focus();
}

/**
 * Waits until copy button is pressed or hovered over.
 * If hovered over calculator, shows copy icon button.
 * If hovered over copy button, changes its color.
 * If copy button is pressed and result isn´t empty, displays message and copies result inside clipboard.
 */
function copy_result ()
{
    //load copy button inside variable
    let copy = document.getElementById('copy');
    
    //togle copy button visibility
    calculator.addEventListener("mouseover", function()
    {
        copy.style.opacity = "1";
    });
    calculator.addEventListener("mouseleave", function()
    {
        copy.style.opacity = "0";
    });		

    //show copied message and copy result into clipboard
    let copied = document.getElementById('copied');
    copy.addEventListener('click', function(event)
    {
        //copy result into clipboard if result is calculated
        if(equation.innerHTML != "")
        {
            copied.style.opacity = "1"; //display "copied"  message
            setTimeout(function(){copied.style.opacity = "0";}, 500);//show message only for 0.5 s

            let text = document.querySelector('.input-container').innerHTML;
            navigator.clipboard.writeText(text);//write result into clipboard
        }
    });
    
}


//disable calculator rendering without JS
let calculator = document.querySelector('.calculator');
document.querySelector('.full-container').classList.remove('none');



let btn = document.querySelectorAll('.btn');
let input_container = document.querySelector('.input-container');
let equation = document.querySelector('.equation');


//catch every keypress
document.addEventListener('keydown', function (e) 
{
    //delete previous calculation as soon as user starts writing again
    if(equation.innerHTML != "")
    {
        equation.innerHTML = "";
        input_container.innerHTML = "";
    }
    
    //if user starts writing change cursor positon to input container
    if(document.activeElement == "[object HTMLBodyElement]")
    {
       position_cursor(input_container);
    }


    let regex = /[0-9*--+,!/]/g;

    //if insert is active (clicked inside)
    if(document.activeElement == "[object HTMLTextAreaElement]")
    {	
        //while typing, innerHTML of textarea doesn´t update, only it´s value updates. This eventListener fixes it in order to send correct info to server
        document.activeElement.addEventListener('input', function()
        {
            document.activeElement.innerHTML = document.activeElement.value;
        });

        //disable writing letters
        if(!e.key.match(regex) && (e.keyCode < 36 || e.keyCode > 41) && e.key != "Delete" && e.key != "Backspace" && e.key !="Tab")
        {
            e.preventDefault();
        }


        if(document.activeElement.innerHTML.trim().length < 1 && (e.key == "Delete" || e.key == "Backspace"))
        {
            if(document.activeElement.className.indexOf('insert') != -1)
            {
               //loop through all insert items and delete those empty ones
               let insert_array = document.querySelectorAll('.input-container .insert');
               for(let i = 0; i < insert_array.length; i++)
               {
                   if(insert_array[i].innerHTML.trim().length < 1)
                   {
                       insert_array[i].remove();
                   }
               }
            }             
        }
    }

    //disable writing letters inside calculator
    if(document.activeElement.className == "input-container")
    {
        if(e.keyCode >= 65 && e.keyCode <= 91 && !(e.ctrlKey))
        {
            e.preventDefault();
        }
    }

    //print pressed key into input if it matches regex above and user is not clicked into input form. keycode <112,123> stands for function keys to prevent printing them
    if(e.key.match(regex) && document.activeElement.className != "input-container" && document.activeElement != "[object HTMLTextAreaElement]"  && !(e.keyCode>=112 && e.keyCode <= 123))
    {
        if(e.key == "*")
        {
            input_container.innerHTML += "×";
        }
        else
        {
            input_container.innerHTML += e.key;
        }
        
    }
    //send input to server
    if(e.key == "=" || e.key == "Enter")
    {
        window.location.href = "?input=" + input_container.innerHTML;
    }


    //delete whole insert element
    let span_container = document.querySelectorAll('.input-container span.wrap');
    for(let i = 0; i < span_container.length; i++)
    {   
        if(span_container[i].children.length == 0)
        {
            span_container[i].remove();
        }
        else
        {
            //special condition for pair functions (requiring 2 parametrs)
            if((span_container[i].children[0].id.includes("power") || span_container[i].children[0].id.includes("sqrt")))
            {
                if(span_container[i].children.length < 2)
                {
                    span_container[i].remove();
                }
            }
        }
    } 
});



copy_result();



//event handler for buttons
for(let i = 0; i < btn.length; i++)
{
    btn[i].addEventListener('click', function()
    {
        //delete previous calculation as soon as user starts writing again
        if(equation.innerHTML != "")
        {
            equation.innerHTML = "";
            input_container.innerHTML = "";
        }

        let write = true;//if current button should be written into input container
        let current = this.innerHTML;
        
        
        if(this.value == "CE")
        {
            write = false;
            input_container.innerHTML = "";
            equation.innerHTML = "";
        }
        if(this.value == "=")
        {
            write = false;
            window.location.href = "?input=" + input_container.innerHTML;
        }
        if(this.value == "backspace")
        {   
            if(input_container.innerHTML.slice(-1) == '>')
            {
                input_container.lastChild.remove();
            }
            else
            {
                input_container.innerHTML= input_container.innerHTML.trim().slice(0, -1);
            }
            write = false;
        }        


        if(write == true)
        {
            input_container.innerHTML += current;
                      
            		
            //textareas are disabled for writing inside buttons, this loop removes the "disabled" attribute for all "insert" textareas inside input container
            let insert_arr = document.querySelectorAll('.insert');
           
            for(let i = 0; i < insert_arr.length; i++)
            {
                insert_arr[i].removeAttribute('disabled');
            }
        }	

        //this section of code ensures that after insert box is printed, the cursor jumps inside the newest one
        if(this.value == "absolute")
        {
            let input = document.querySelectorAll('.input-container #absolute');
            position_cursor(input[input.length-1]);
        }
        if(this.value == "power")
        {
            let input = document.querySelectorAll('.input-container #power');           
            position_cursor(input[input.length-1]);
        }
        if(this.value == "sqrt")
        {
            let input = document.querySelectorAll('.input-container #sqrt-y');
            position_cursor(input[input.length-1]);		
        }
    });
}


//edit insert elements
document.querySelector('.input-container').addEventListener('input', function()
{
    let insert_array = document.querySelectorAll('.input-container .insert');//get all insert elements and store them into array
    for(let i = 0; i < insert_array.length; i++)
    {
        insert_array[i].value = insert_array[i].value.trim(); //get rid of previous necessary whitespaces that are included in HTML

        insert_array[i].style.width = (insert_array[i].value.length*0.66)+"em";//chnage width of textarea according to number of letters

        //change inserted element´s background color and toggle border
        if(insert_array[i].value.length > 0)
        {
            insert_array[i].classList.add("color");
        }
        else
        {
            insert_array[i].classList.remove("color");
        }
    }
});
