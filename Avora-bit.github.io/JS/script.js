{         //nav bar
  function ShowTopNav() {
    var x = document.getElementById("myTopnav");
    if (x.className === "topnav") x.className += " responsive";
    else x.className = "topnav";
  }
}

{         //collapsible timeline
  var coll = document.getElementsByClassName("collapsible");
  var i;

  for (i = 0; i < coll.length; i++) {
    coll[i].addEventListener("click", collapseAnim);
  }

  function collapseAnim() {
    this.classList.toggle("active");
    var content = this.nextElementSibling;
    if (content.style.maxHeight) {
      content.style.maxHeight = null;
    } else {
      content.style.maxHeight = content.scrollHeight + "px";
    }
  }
}

{         //scroll to top button
  var mybutton = document.getElementById("topBtn");
  window.onscroll = function () { scrollFunction() };
  function scrollFunction() {
    if (document.body.scrollTop > 200 || document.documentElement.scrollTop > 200) {
      mybutton.style.display = "block";
    }
    else {
      mybutton.style.display = "none";
    }
  }
  function topFunction() {
    document.body.scrollTop = 0;
    document.documentElement.scrollTop = 0;
  }
}

{         //score slideshow
  let slideIndex = 1;
  showSlides(slideIndex);

  function plusSlides(n) {
    showSlides(slideIndex += n);
  }

  function currentSlide(n) {
    showSlides(slideIndex = n);
  }

  function showSlides(n) {
    let i;
    let slides = document.getElementsByClassName("mySlides");
    let dots = document.getElementsByClassName("dot");
    if (n > slides.length) { slideIndex = 1 }
    if (n < 1) { slideIndex = slides.length }
    for (i = 0; i < slides.length; i++) {
      slides[i].style.display = "none";
    }
    for (i = 0; i < dots.length; i++) {
      dots[i].className = dots[i].className.replace(" active", "");
    }
    slides[slideIndex - 1].style.display = "block";
    dots[slideIndex - 1].className += " active";
  }


}

{         // play the piano
  const audioContext = new AudioContext();
  const oscList = [];
  let mainGainNode = null;

  const keyboard = document.querySelector(".keyboard");
  const wavePicker = document.querySelector("select[name='waveform']");
  const volumeControl = document.querySelector("input[name='volume']");

  let noteFreq = null;
  let customWaveform = null;
  let sineTerms = null;
  let cosineTerms = null;

  function createNoteTable() {
    const noteFreq = [];
    for (let i = 0; i < 9; i++) {
      noteFreq[i] = [];
    }
    //list of frequencies
    // https://pages.mtu.edu/~suits/notefreqs.html

    {
      noteFreq[2]["C"] = 65.41;
      noteFreq[2]["C#"] = 69.30;
      noteFreq[2]["D"] = 73.42;
      noteFreq[2]["D#"] = 77.78;
      noteFreq[2]["E"] = 82.41;
      noteFreq[2]["F"] = 87.31;
      noteFreq[2]["F#"] = 92.50;
      noteFreq[2]["G"] = 98.00;
      noteFreq[2]["G#"] = 103.83;
      noteFreq[2]["A"] = 110.00;
      noteFreq[2]["A#"] = 116.54;
      noteFreq[2]["B"] = 123.47;
    }
    {
      noteFreq[3]["C"] = 130.81;
      noteFreq[3]["C#"] = 138.59;
      noteFreq[3]["D"] = 146.83;
      noteFreq[3]["D#"] = 155.56;
      noteFreq[3]["E"] = 164.81;
      noteFreq[3]["F"] = 174.61;
      noteFreq[3]["F#"] = 185.00;
      noteFreq[3]["G"] = 196.00;
      noteFreq[3]["G#"] = 207.65;
      noteFreq[3]["A"] = 220.00;
      noteFreq[3]["A#"] = 233.08;
      noteFreq[3]["B"] = 246.94;
    }
    {
      noteFreq[4]["C"] = 261.63;
      noteFreq[4]["C#"] = 277.18;
      noteFreq[4]["D"] = 293.66;
      noteFreq[4]["D#"] = 311.13;
      noteFreq[4]["E"] = 329.63;
      noteFreq[4]["F"] = 349.23;
      noteFreq[4]["F#"] = 369.99;
      noteFreq[4]["G"] = 392.00;
      noteFreq[4]["G#"] = 415.30;
      noteFreq[4]["A"] = 440.00;
      noteFreq[4]["A#"] = 466.16;
      noteFreq[4]["B"] = 493.88;
    }
    {
      noteFreq[5]["C"] = 523.25;
      noteFreq[5]["C#"] = 554.37;
      noteFreq[5]["D"] = 587.33;
      noteFreq[5]["D#"] = 622.25;
      noteFreq[5]["E"] = 659.25;
      noteFreq[5]["F"] = 698.46;
      noteFreq[5]["F#"] = 739.99;
      noteFreq[5]["G"] = 783.99;
      noteFreq[5]["G#"] = 830.61;
      noteFreq[5]["A"] = 880.00;
      noteFreq[5]["A#"] = 932.33;
      noteFreq[5]["B"] = 987.77;
    }
    {
      noteFreq[6]["C"] = 1046.50;
      noteFreq[6]["C#"] = 1108.73;
      noteFreq[6]["D"] = 1174.66;
      noteFreq[6]["D#"] = 1244.51;
      noteFreq[6]["E"] = 1318.51;
      noteFreq[6]["F"] = 1396.91;
      noteFreq[6]["F#"] = 1479.98;
      noteFreq[6]["G"] = 1567.98;
      noteFreq[6]["G#"] = 1661.22;
      noteFreq[6]["A"] = 1760.00;
      noteFreq[6]["A#"] = 1864.66;
      noteFreq[6]["B"] = 1975.53;
    }
    return noteFreq;
  }

  function setup() {
    noteFreq = createNoteTable();

    volumeControl.addEventListener("change", changeVolume, false);

    mainGainNode = audioContext.createGain();
    mainGainNode.connect(audioContext.destination);
    mainGainNode.gain.value = volumeControl.value;

    // Create the keys; skip any that are sharp or flat; for
    // our purposes we don't need them. Each octave is inserted
    // into a <div> of class "octave".

    noteFreq.forEach((keys, idx) => {
      const keyList = Object.entries(keys);
      const octaveElem = document.createElement("div");
      octaveElem.className = "octave";

      keyList.forEach((key) => {
        if (key[0].length === 1) {
          octaveElem.appendChild(createKey(key[0], idx, key[1]));
        }
      });

      keyboard.appendChild(octaveElem);
    });

    document.querySelector("div[data-note='B'][data-octave='5']").scrollIntoView(false);

    sineTerms = new Float32Array([0, 0, 1, 0, 1]);
    cosineTerms = new Float32Array(sineTerms.length);
    customWaveform = audioContext.createPeriodicWave(cosineTerms, sineTerms);

    for (let i = 0; i < 9; i++) {
      oscList[i] = {};
    }
  }

  setup();

  function createKey(note, octave, freq) {
    const keyElement = document.createElement("div");
    const labelElement = document.createElement("div");

    keyElement.className = "key";
    keyElement.dataset["octave"] = octave;
    keyElement.dataset["note"] = note;
    keyElement.dataset["frequency"] = freq;

    labelElement.innerHTML = `${note}<sub>${octave}</sub>`;
    keyElement.appendChild(labelElement);

    keyElement.addEventListener("mousedown", notePressed, false);
    keyElement.addEventListener("mouseup", noteReleased, false);
    keyElement.addEventListener("mouseover", notePressed, false);
    keyElement.addEventListener("mouseleave", noteReleased, false);

    return keyElement;
  }

  function playTone(freq) {
    const osc = audioContext.createOscillator();
    mainGainNode.connect(audioContext.destination);

    const zeroGain = 0.00001;
    const maxGain = volumeControl.value;
    // const sustainedGain = 0.01;

    mainGainNode.gain.value = zeroGain;

    const setAttack = () =>
      mainGainNode.gain.exponentialRampToValueAtTime(maxGain, audioContext.currentTime + 0.05);
    // const setDecay = () =>
    //   mainGainNode.gain.exponentialRampToValueAtTime(sustainedGain, audioContext.currentTime + 1);
    // const setRelease = () =>
    //   mainGainNode.gain.exponentialRampToValueAtTime(zeroGain, audioContext.currentTime + 2);

    setAttack();
    // setDecay();
    // setRelease();
    osc.connect(mainGainNode);

    const type = wavePicker.options[wavePicker.selectedIndex].value;

    if (type === "custom") {
      osc.setPeriodicWave(customWaveform);
    } else {
      osc.type = type;
    }

    osc.frequency.value = freq;
    osc.start();

    return osc;
  }

  function notePressed(event) {
    if (event.buttons & 1) {
      const dataset = event.target.dataset;

      if (!dataset["pressed"]) {
        const octave = Number(dataset["octave"]);
        oscList[octave][dataset["note"]] = playTone(dataset["frequency"]);
        dataset["pressed"] = "yes";
      }
    }
  }

  function noteReleased(event) {
    const dataset = event.target.dataset;
    if (dataset && dataset["pressed"]) {
      const octave = Number(dataset["octave"]);
      oscList[octave][dataset["note"]].stop();
      delete oscList[octave][dataset["note"]];
      delete dataset["pressed"];
    }
  }

  function changeVolume(event) {
    mainGainNode.gain.value = volumeControl.value;
  }
}
