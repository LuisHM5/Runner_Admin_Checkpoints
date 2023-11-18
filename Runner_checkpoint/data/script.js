GetRaceStatus();
GetDataRace();
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
  // websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

// When websocket is established, call the getReadings() function
// function onOpen(event) {
//   console.log("Connection opened");
//   getReadings();
// }

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
    const { cards } = msgObj;
    setDataTable({ cards });
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

function setDataTable({ cards }) {
  const table = document.querySelector(".table-body");
  table.innerHTML = "";

  const cardsEntries = Object.entries(cards);

  const cardsEntriesOrdered = cardsEntries.sort((a, b) => {
    const pointsA = a[1];
    const pointsB = b[1];
    const pointsKeysA = Object.keys(pointsA);
    const pointsKeysB = Object.keys(pointsB);
    const lastTimeA = pointsA[pointsKeysA[pointsKeysA.length - 1]];
    const lastTimeB = pointsB[pointsKeysB[pointsKeysB.length - 1]];

    // Assuming time is in HH:mm:ss format
    const timeComparison = new Date(`1970-01-01T${lastTimeA}`) - new Date(`1970-01-01T${lastTimeB}`);

    return timeComparison;
  });

  cardsEntriesOrdered.forEach(([card, points]) => {
    // Iterate over each point
    const pointsKeys = Object.keys(points);
    pointsKeys.forEach((point) => {
      const row = document.createElement("tr");
      const cardTd = document.createElement("td");
      const pointTd = document.createElement("td");
      const timeTd = document.createElement("td");
      cardTd.innerHTML = card;
      pointTd.innerHTML = point;
      timeTd.innerHTML = points[point];
      row.appendChild(cardTd);
      row.appendChild(pointTd);
      row.appendChild(timeTd);
      table.appendChild(row);
    });
  });
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

function setDataTable({ cards }) {
  const cardsKeys = Object.keys(cards);
  const table = document.querySelector(".table-body");
  table.innerHTML = "";

  const cardsKeysOrdered = cardsKeys.sort((a, b) => {
    const pointsA = cards[a];
    const pointsB = cards[b];
    const pointsKeysA = Object.keys(pointsA);
    const pointsKeysB = Object.keys(pointsB);
    const lastTimeA = pointsA[pointsKeysA[pointsKeysA.length - 1]];
    const lastTimeB = pointsB[pointsKeysB[pointsKeysB.length - 1]];
    if (lastTimeA > lastTimeB) {
      return -1;
    }
    if (lastTimeA < lastTimeB) {
      return 1;
    }
    // a must be equal to b
    return 0;
  });

  cardsKeysOrdered.forEach((card) => {
    // Iterate over each point
    const points = cards[card];
    const pointsKeys = Object.keys(points);
    console.log("points keys:", pointsKeys);
    pointsKeys.forEach((point) => {
      const row = document.createElement("tr");
      const cardTd = document.createElement("td");
      const pointTd = document.createElement("td");
      const timeTd = document.createElement("td");
      cardTd.innerHTML = card;
      pointTd.innerHTML = point;
      timeTd.innerHTML = points[point];
      row.appendChild(cardTd);
      row.appendChild(pointTd);
      row.appendChild(timeTd);
      table.appendChild(row);
    });
  });
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

function handleInitRace(event) {
  console.log("INIT RACE");
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
        const table = document.querySelector(".table-body");
        table.innerHTML = "";
      }
    })
    .catch((error) => console.log(error));
}

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

function GetDataRace() {
  const url = "/data-race";
  const method = "GET";

  fetch(url, {
    method,
  })
    .then((response) => response.json())
    .then((data) => {
      const { cards } = data;

      if (time) {
        setDataTable({ cards });
      }
    })
    .catch((error) => console.log(error));
}
