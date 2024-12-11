//incomplete
function writeCopy() {
    let d = new Date();
    let el = document.querySelector("#copy");
    el.innerHTML = 'Copyright © ' + d.getFullYear();
  }
  
  document.addEventListener('DOMContentLoaded', writeCopy);
  
  // keep scores
  var lastNum = -1;
  var draw = 0;
  var win = 0;
  var lose = 0;
  
  var banker, player, drawn;
  var boss, selected;
  var scissor, paper, stone;
  var pSelect, restart;
  var myTimerEvt = 0;
  
  const imgList = ["scissor.svg", "paper.svg", "stone.svg"]; // images
  /* update the image of element to imgID **/
  
  function updateImage(ele, imgID) {
    if (!imgID) { // if undefine
      imgID = Math.floor(Math.random() * 3);
    } else if (imgID == -1) { // restart to blank
      ele.src = "img/blank.svg";
      return 0;
    }
    ele.src = "img/" + imgList[imgID];
    return imgID;
  }
  
  
  function init() {
    // define elements
    banker = document.querySelector("#banker"); // banker's score
    player = document.querySelector("#player"); // player's score
    drawn = document.querySelector("#drawn"); // draw
  
    boss = document.querySelector("#bossImg"); // banker
    selected = document.querySelector("#selectedImg"); // player
  
    scissor = document.querySelector("#scissorIcon"); // icons
    paper = document.querySelector("#paperIcon");
    stone = document.querySelector("#stoneIcon");
  
    pSelect = document.querySelector(".selection"); // selection panel
    restart = document.querySelector("#restart"); // restart button
  
    // define the click listeners
    scissor.addEventListener("click", chooseMe);
    paper.addEventListener("click", chooseMe);
    stone.addEventListener("click", chooseMe);
  
    restart.addEventListener("click", doStart);
    restart.style.display = "none"; // hide restart button
  
  //  displayScores(); // display the scores
  //  doStart();
  }
  
  document.addEventListener('DOMContentLoaded', init);
  