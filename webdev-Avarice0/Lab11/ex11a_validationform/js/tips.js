function writeCopy() {
  let d = new Date();
  let el = document.querySelector("#copy");
  el.innerHTML = 'Copyright © ' + d.getFullYear();
}

document.addEventListener('DOMContentLoaded', writeCopy);


// fields
const bill = document.querySelector("#bill");
const numPeople = document.querySelector("...");
const tip = //todo

// elements
const amt = document.querySelector("#amt");
const ttp = document.querySelector("...");
const app =  //todo

// listeners when fields change
const inputColl = document.querySelectorAll("  "); //todo
for (let item of inputColl) { // loop each item
  item.addEventListener("change", doCalculate);
}

// 
function doCalculate() {
  let totalTips = (1.0 * bill.value) * (1.0 * tip.value) * 0.01;
  let totalAmount = //todo
  let amountPerPerson = //todo
  
  amt.innerHTML = totalAmount;
  ttp.innerHTML = //todo
  app.innerHTML = //todo
}