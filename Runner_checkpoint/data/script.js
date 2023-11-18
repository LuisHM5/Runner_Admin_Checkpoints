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
  var msgObj = JSON.parse(event.data);
  var keys = Object.keys(msgObj);

  if (msgObj.hasOwnProperty("time")) {
    if (!inRace) {
      const { time } = msgObj;
      setTimeRace({ time });
    }
  }

  if (msgObj.hasOwnProperty("cards")) {
    /*
    MODEL
     "cards": {
    "73A8EAFC": { "punto1": "10:00:00", "punto2": "10:20:00" },
    "538C1BF5": { "punto1": "10:00:00", "punto2": "10:20:00" }
  }
    */
    const { cards } = msgObj;
    console.log("cards value:", cards);
    const cardsKeys = Object.keys(cards);
    console.log("cards keys:", cardsKeys);
    const table = document.querySelector(".table-body");
    table.innerHTML = "";
    cardsKeys.forEach((card) => {
      // Iterate over each point
      const points = cards[card];
      const pointsKeys = Object.keys(points);
      console.log("points keys:", pointsKeys);
      pointsKeys.forEach((point) => {
        // order by time and point
        /*
            <table id="table" class="table">
      <thead class="table-head">
        <tr>
          <th>Tarjeta</th>
          <th>Punto</th>
          <th>Tiempo</th>
        </tr>
      </thead>
      <tbody class="table-body">
      </tbody>
    </table>
        */
        const time = points[point];
        const tr = document.createElement("tr");
        const tdCard = document.createElement("td");
        tdCard.innerHTML = card;
        const tdPoint = document.createElement("td");
        tdPoint.innerHTML = point;
        const tdTime = document.createElement("td");
        tdTime.innerHTML = time;
        tr.appendChild(tdCard);
        tr.appendChild(tdPoint);
        tr.appendChild(tdTime);
        table.appendChild(tr);
      });
    });
  }

  if (msgObj.hasOwnProperty("status")) {
    const { status } = msgObj;
    console.log("status value:", status);
    if (!status) {
      console.log("Entro a status false");
      setDefaultState();
    }
  }
}

function setDefaultState() {
  containerTime.classList.add("hidden");
  containerBtnStartRace.classList.remove("hidden");
  try {
    console.log("clear interval");
    clearInterval(timecounter);
  } catch (error) {
    console.log(error);
  }
  inRace = false;
}

function setTimeRace({ time }) {
  timeSpan.innerHTML = time;
  containerBtnStartRace.classList.add("hidden");
  containerTime.classList.remove("hidden");
  timeCount();
}

// set time out 1s
const timeCount = () => {
  console.log("invoke timecounter | status:", inRace);
  if (inRace) return;
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
    timeSpan.innerHTML = newTime;
  }, 1000);

  inRace = true;
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
    .then((response) => response.json())
    .then((data) => {
      const { time } = data;
      if (time) {
        setTimeRace({ time });
      }
    })
    .catch((error) => console.log(error));
};

const handleStopRace = (event) => {
  const url = "/stop-race";
  const method = "POST";
  // confirm async

  if (confirm("¿Está seguro de detener la carrera?")) {
    clearInterval(timecounter);
    fetch(url, {
      method,
    })
      .then((response) => response.text())
      .then((data) => {
        console.log(data);
        if (data == "ok") {
          setDefaultState();
        } else {
          alert("No se pudo detener la carrera");
        }
      })
      .catch((error) => console.log(error));
  }
};
function GetRaceStatus() {
  const url = "/status-race";
  const method = "GET";

  fetch(url, {
    method,
  })
    .then((response) => response.json())
    .then((data) => {
      const { time } = data;

      if (time) {
        setTimeRace({ time });
      }
    })
    .catch((error) => console.log(error));
}
