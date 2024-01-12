let ul = document.querySelector("div");
let lis = document.querySelectorAll("li");

let page1 = document.getElementById("page1");
let page2 = document.getElementById("page2");
let page3 = document.getElementById("page3");

ul.addEventListener("click", changePage);

function changePage(event) {
  if (event.target.id === "1") {
    exclusive();
    page1.style.display = "flex";
  } else if (event.target.id === "2") {
    exclusive();
    page2.style.display = "flex";
  } else if (event.target.id === "3") {
    exclusive();
    page3.style.display = "flex";
  } else {
    console.log("error");
  }
}

function exclusive() {
  page1.style.display = "none";
  page2.style.display = "none";
  page3.style.display = "none";
}
