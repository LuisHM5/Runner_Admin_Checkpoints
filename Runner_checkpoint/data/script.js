GetRaceStatus();
const gateway = `ws://${window.location.hostname}:${window.location.port}/ws`;
let websocket;
let timecounter;
const timeSpan = document.querySelector("#time");
const containerTime = document.querySelector("#container-time");
const containerBtnStartRace = document.querySelector("#container-race");
// Init web socket when the page loads
window.addEventListener("load", onload);

function onload(event) {
  initWebSocket();
}

function getReadings() {
  websocket.send("getReadings");
}

function initWebSocket() {
  console.log("Trying to open a WebSocket connection…");
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

// When websocket is established, call the getReadings() function
function onOpen(event) {
  console.log("Connection opened");
  getReadings();
}

function onClose(event) {
  console.log("Connection closed");
  setTimeout(initWebSocket, 2000);
}

// Function that receives the message from the ESP32 with the readings
let inRace = false;
function onMessage(event) {
  console.log(event.data);
  var myObj = JSON.parse(event.data);
  var keys = Object.keys(myObj);

  if (myObj.hasOwnProperty("time")) {
    // add time to span
    const { time } = myObj;
    timeSpan.innerHTML = time;
    containerBtnStartRace.classList.add("hidden");
    containerTime.classList.remove("hidden");
    timeCount();
  }
}

// set time out 1s
const timeCount = () => {
  timecounter = setInterval(() => {
    console.log("timecount");
    // Add 1 to time in format with ceros 00:00:00
    const time = timeSpan.innerHTML;
    const timeArray = time.split(":");
    const seconds = parseInt(timeArray[2]);
    const minutes = parseInt(timeArray[1]);
    const hours = parseInt(timeArray[0]);
    let newTime = "";
    if (seconds < 59) {
      newTime = `${hours < 10 ? "0" + hours : hours}:${minutes < 10 ? "0" + minutes : minutes}:${
        seconds + 1 < 10 ? "0" + (seconds + 1) : seconds + 1
      }`;
    } else if (minutes < 59) {
      newTime = `${hours < 10 ? "0" + hours : hours}:${minutes + 1 < 10 ? "0" + (minutes + 1) : minutes + 1}:00`;
    } else {
      newTime = `${hours + 1 < 10 ? "0" + (hours + 1) : hours + 1}:00:00`;
    }
  }, 1000);
};

// RACE

const handleInitRace = (event) => {
  event.preventDefault();
  // Send post
  const url = "/start-race";
  const method = "POST";

  fetch(url, {
    method,
  })
    .then((response) => response.text())
    .then((data) => {
      console.log(data);
      if (data == "ok") {
        containerBtnStartRace.classList.add("hidden");
        containerTime.classList.remove("hidden");
      }
      timeCount();
    })
    .catch((error) => console.log(error));
};

function GetRaceStatus() {
  const url = "/status-race";
  const method = "GET";

  fetch(url, {
    method,
  })
    .then((response) => response.text())
    .then((data) => {
      console.log(data);
      if (data == "true") {
        containerBtnStartRace.classList.add("hidden");
        containerTime.classList.remove("hidden");
        timeCount();
      }
    })
    .catch((error) => console.log(error));
}
