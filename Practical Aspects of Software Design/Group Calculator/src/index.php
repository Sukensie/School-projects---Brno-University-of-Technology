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
 * Project name: IVS-2. projekt
 * File: index.php
 * Date: 9.4.2021
 * Last change: 9.4.2021
 * Authors: David Kocman xkocma08
 *          Tomáš Souček xsouce15
 *          Přemek Janda xjanda28
 *          Adam Cologna xcolog00
 * Description: Main body of the calculator. 
 * @version 1.0
 */

// start sessions for storing calculator's history intermediate results
session_start();

include 'math_lib.php';
include 'utils.php';


if (isset($_SESSION['switch'])) {
	if (isset($_GET['switch'])) {
		$_SESSION['switch'] = (!empty($_GET['switch'])) ? 1 : 0;
		$_SESSION['result'] = "";
		header("Location: ".$_SERVER['PHP_SELF']);
		exit();
	}
} else {
	$_SESSION['switch'] = 0;
}

if (isset($_GET['input'])) {
	$_SESSION['output'] = $output;
	$_SESSION['result'] = $result;
	$_SESSION['input'] = $input;
	header("Location: ".$_SERVER['PHP_SELF']);
	exit();
}
?>

<!DOCTYPE html>
<html lang="cs">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<link rel="preconnect" href="https://fonts.gstatic.com">
	<link href="https://fonts.googleapis.com/css2?family=Hepta+Slab:wght@400;600;700&display=swap" rel="stylesheet">
	<link rel="stylesheet" href="https://pro.fontawesome.com/releases/v5.10.0/css/all.css" 
	integrity="sha384-AYmEC3Yw5cVb3ZcuHtOA93w35dYTsvhLPVnYs9eStHfGJvOvKxVfELGroGkvsg+p" crossorigin="anonymous"/>
	<link href="normalize.css">
	<meta name="viewport" content="width=device-width, initial-scale=1">	
	<title>IVS KERBAL CALCULATOR</title>
	<link rel="stylesheet" type="text/css" href="calc_style.css"></link>


	<!-- favicon -->
	<link rel="apple-touch-icon" sizes="180x180" href="img/icon/apple-touch-icon.png">
	<link rel="icon" type="image/png" sizes="32x32" href="img/icon/favicon-32x32.png">
	<link rel="icon" type="image/png" sizes="16x16" href="img/icon/favicon-16x16.png">
	<link rel="manifest" href="img/icon/site.webmanifest">
	<link rel="mask-icon" href="img/icon/safari-pinned-tab.svg" color="#5bbad5">
	<meta name="msapplication-TileColor" content="#da532c">
	<meta name="theme-color" content="#ffffff">
</head>

<body>
	<div class="full-container none">

	<h1>IVS KERBAL CALCULATOR</h1>

	<label class="switch">
	  <input type="checkbox" name="switch" value="1" <?php echo ($_SESSION['switch'] == '1') ? ' checked' : ''; ?> >>
	  <span class="slider round"></span>
	</label>
	<!-- UI of the calculator -->
	<div class="calculator">
		<span id="copy"><i class="far fa-copy"></i></span>
		<div id="copied">
			<span>copied</span>
			<span id="square"></span>
		</div>
		<div class="equation"><?php if (isset($_SESSION['input'])) echo $_SESSION['input']; ?></div>
		<div contentEditable="true" class="input-container"><?php if(isset($_SESSION['output'])) echo $_SESSION['output']; ?></div>
		<div class="btn-container">
			<!-- ROW -->
			<!-- 1st row with absolute and delete buttons -->
			<!-- space inside TEXTAREA is necessary otherwise it would print error saying its not a node-->
			<button class="btn btn-function" value="absolute"><span class="wrap">|<textarea disabled rows="1" id="absolute" class="insert x"> </textarea>|</span></button>
			<button class="btn btn-util" value="CE">CE</button>
			<button class="btn btn-util grow-2" value="backspace">←</button>
			
			<!-- ROW -->
			<!-- 2nd row with root, power, factorial and divide buttons -->
			<button class="btn btn-function" value="sqrt"><span class="wrap"><textarea disabled class="insert sup ^" id="sqrt-y" maxlength="4"> </textarea>√<textarea disabled class="insert _" id="sqrt-x"> </textarea></span></button>
			<button class="btn btn-function" value="power"><span class="wrap"><textarea disabled id="power" class="insert _"> </textarea><textarea disabled id="powerY" class="insert sup ^" maxlength="4"> </textarea></span></button>
			<button class="btn btn-function" value="!">!</button>
			<button class="btn btn-function" value="/">/</button>

			<!-- ROW -->
			<!-- 3rd row with 7, 8, 9 and multiply buttons -->
			<button class="btn" value="7">7</button>	
			<button class="btn" value="8">8</button>
			<button class="btn" value="9">9</button>
			<button class="btn btn-function" value="multiply">×</button>

			<!-- ROW -->
			<!-- 4th row with 4,5,6 and subtract buttons -->
			<button class="btn" value="4">4</button>	
			<button class="btn" value="5">5</button>
			<button class="btn" value="6">6</button>
			<button class="btn btn-function" value="−">−</button>

			<!-- ROW -->
			<!-- 5th row with 1,2,3 and add buttons -->
			<button class="btn" value="1">1</button>	
			<button class="btn" value="2">2</button>
			<button class="btn" value="3">3</button>
			<button class="btn btn-function" value="+">+</button>


			<!-- ROW -->
			<!-- 6th row with 0, decimal and result buttons -->
			<button class="btn" value="0">0</button>	
			<button class="btn" value=".">.</button>
			<button class="btn grow-2 btn-equal " value="=">=</button>
		</div>
	</div>
	<div class="content">
    <a href="../dokumentace.pdf" class="help-btn" target="_blank">GUIDE</a>
	<?php 

	// process if calculatior was used
	if (isset($_SESSION['input'])) { 
		// setting up counter for history
		if (!isset($_SESSION['count'])) 
			$_SESSION['count'] = 0; 
		else
			$_SESSION['count']++;

		// setting up history array to store intermediate results
		if (!isset($_SESSION['history'])) 
			$_SESSION['history'] = array();

		// maximum number of elements in history memory
		$max = 5;

		echo "<h2>CALCULATIONS HISTORY</h2>";

		// insert values into calculators memory
		if($_SESSION['result'] != "") {
			$history_insert = array("assignment" => $_SESSION['result'], "output" => $_SESSION['output']);
			$_SESSION['history'][$_SESSION['count']%$max] = $history_insert;
		} else {
			$_SESSION['count']--;
		}
		 
		// print history
		$utils->print_hist_array($_SESSION['history'], $_SESSION['count'], $max);


	// unset memory on entering home-page
	} else { 
		unset($_SESSION['count']);
		unset($_SESSION['history']);
	}
	?>
	</div>
	<div class="about">
		<h2>ABOUT US</h2>
		<div>
			<img src="img/kerbals/xcolog.svg" width="100px">
			<p class="name"><span class="smaller">x</span>colog<span class="smaller">00</span></p>
			<p>příručky, licence</p>
		</div>

		<div>
			<img src="img/kerbals/xjanda.svg" width="100px">
			<p class="name"><span class="smaller">x</span>janda<span class="smaller">28</span></p>
			<p> makefile, profiling, prezentace</p>
		</div>

		<div>
			<img src="img/kerbals/xkocma.svg" width="100px">
			<p class="name"><span class="smaller">x</span>kocma<span class="smaller">08</span></p>
			<p>doxygen, instalace</p>
		</div>

		<div>
			<img src="img/kerbals/xsouce.svg" width="100px">
			<p class="name"><span class="smaller">x</span>souce<span class="smaller">15</span></p>
			<p>GUI, icon, mockup, JS</p>
		</div>
	
	</div>

	<script type="text/javascript" src="calc_script.js"></script>
	<script type="text/javascript">
		//dark mode
		let dark = <?php echo $_SESSION['switch']; ?>;
		var checkbox = document.querySelector("input[type=checkbox]");

		checkbox.addEventListener('change', function() 
		{
			window.location.href = (this.checked) ? "?switch=1" : "?switch=0";
		});
		if(dark == 1)
		{
			document.querySelector('body').classList.add('dark');
		}

	</script>
	</div>
	<noscript>
		<style>
			body{
				font-family: 'Signika', sans-serif;
				padding: 2em 0;
				text-align: center;
				min-width: 400px;
				max-width: 50%;
				margin: 0 auto;
				background: none;
				font-size: 16px !important;
				color: unset;
				font-weight: unset;
			}
			.noscript-container
			{
				display: flex;
				align-items: center;
				justify-content: center;
				flex-direction: column;
				margin-top: 8vh;
			}
			a{
				color:black
			}
			
			a:hover {
				text-decoration: none;
				color: #2A8EC4
			}

			ol {
				list-style-position: inside;
				padding: 0;
			}
			
			h2{
				text-align:center;
				font-size: 2em;
				color: #2A8EC4;
			}
			h2:not(:first-child){
				margin-top: 3em;
			}

			h3{
				color: #EC8E76;
				margin-bottom: 10px;
			}
			
			.images{
				text-align:center
			}
			
			hr{
				display: flex;
				align-self: center;
				max-width: 80%;
				margin: 4em;
			}
			h1
			{
				padding: 0;
				margin: 0;
				background: none;
				color: #2A8EC4;
			}
			@media screen and (min-width: 1200px)
			{
				body
				{
					font-size: 25px;
				}
			}
		</style>
		<div class="noscript-container">
			<link rel="stylesheet" type="text/css" href="enablejs.css"></link>
			<img style="max-width: 300px;" src="img/error.svg">
			<h2>Jak stáhnout Google Chrome</h2>
				<p>Tento krok doporučujeme v případě, že stále používáte nějakou starší verzi Internetu Explorer.</p>
				<ol>
					<li>Navštivte následující <href><a href="https://www.google.com/chrome/">stránku pro stažení Google Chromu</a></href></li>
					<li>Klikněte na tlačítko "Stáhněte si Chrome". Viz <a href="img1.png">obrázek s pozicí</a></li>
					<li>Otevřete .exe soubor, který jste právě stáhli. Pokud vás instalátor požádá o provádění změn, tak zaklikněte ano.</li>
					<li>Po úspěšné instalaci by se měl vytvořit zástupce na ploše, který stačí spustit.</li>
				</ol>
			<h2>Spuštění Javascriptu pro jednotlivé prohlížeče</h2>
			<h3>Safari</h3>
				<ol>
					<li>V levém horním rohu klikněte na tlačítko Safari a zvolte Předvolby.</li>
					<li>Po otevření předvoleb použijte sekci Zabezpečení.</li>
					<li>Ujistěte se, že Javascript je povolen, případně zaškrtněte políčko u něj.</li>
				</ol>
			<h3>Mozilla Firefox</h3>
				<ol>
					<li>Do vyhledavače napiště "about:config" a stiskněte Enter. Budete muset odsouhlasit, že budete opatrní. Systém vás případně upozorní na nebezpečné věci. <a href="img/img3.png">obrázek s textem pro vyhledavač</a></li>
					<li>Vyhledejte "javascript.enabled" <a href="img/img4.png">obrázek s textem</a></li>
					<li>Ujistěte se, že je tato položka nastavena na true, případně ji přepněte pomocí tlačítka Toggle na konci řádku. Viz obrázek. <a href="img/img5.png">tlačítko Toggle</a></li>
				</ol>
			<h3>Google Chrome</h3>
					<ol>
						<li>V pravém horním rohu klikněte na tlačítko Více (3&nbsp;tečky). Viz <a href="img/img2.png">tlačítko Více</a></li>
						<li>Zvolte "Nastavení", kde v sekci "Ochrana soukromí a zabezpečení" zvolte "Nastavení webu"</li>
						<li>Ujistěte se, že u položky Javascript máte uvedeno "Povoleno" (v případě "Zablokováno" je potřeba zakliknout tlačítko vedle nápisu)</li>
					</ol>
			<h3>Microsoft Edge</h3>
					<ol>
						<li>Stejně jako u Google Chromu zaklikněte tlačítko více (3&nbsp;tečky v pravém horním rohu) a zvolte Nastavení.</li>
						<li>V sekci (dá se zvolit v levém sloupci) Oprávnění pro soubory cookie a weby naleznete položku Javascript, který musí být povolen pro správné fungování.</li>
					</ol>

					<hr>
				
			<div class="images">
			<p><figure><img class="img" src="img/img1.png" alt="pozice tlačítka"/>
				<figcaption>Pozice tlačítka "Stáhnout Chrome"</figcaption></figure></p>
			<p><img class="img" src="img/img2.png" width="264" height="126" alt="tlačítko více"/>
				<figcaption>Pozice tlačítka Více</figcaption></p>
			<p><img class="img" src="img/img3.png" width="577" height="104" alt="zadaní do vyhledavače"/>
				<figcaption>Zadaní "about:config" do vyhledavače</figcaption></p>
			<p><img class="img" src="img/img4.png" alt="vyhledávání javascriptu"/>
				<figcaption>Vyhledání "javascript.enabled"</figcaption></p>
			<p><img class="img" src="img/img5.png" width="71" height="66" alt="tlačítko Toggle"/>
				<figcaption>Tlačítko Toggle</figcaption></p>
			</div>
		</div>
	
	</noscript>	
</body>
</html>
