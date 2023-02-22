let  hamburger = document.querySelector('.burger');
hamburger.addEventListener('click', function()
{
    hamburger.classList.toggle("open");
});


let mainNav = document.getElementById('js-menu');
let navBarToggle = document.getElementById('js-navbar-toggle');

navBarToggle.addEventListener('click', function () {
  mainNav.classList.toggle('active');
});


/* SMOOTH SCROLLING */ 
const links = document.querySelectorAll("a.nav-links");

for (const link of links) {
  link.addEventListener("click", clickHandler);
  link.addEventListener('click', function()
  {
    mainNav.classList.toggle('active');
    hamburger.classList.toggle("open");
  });
}

function clickHandler(e) {
  e.preventDefault();
  const href = this.getAttribute("href");
  const offsetTop = document.querySelector(href).offsetTop;

  scroll({
    top: offsetTop-100,
    behavior: "smooth"
  });
}




function addCover()
{
  //todo viz přechod v 127.0.0.1 auto
}
//document.querySelectorAll('#reference .container a').addEventListener('click', addCover());

references = document.querySelectorAll('#reference .container div');
if(references.length % 2 == 1)
{
  references[references.length-1].classList.add("none");
}
