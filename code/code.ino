#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define touch_pin D1
#define buzzer_pin D2
#define green_pin D3
#define blue_pin D4
#define yellow_pin D5
#define red_pin D6
#define sevencolorflash_pin D0

int val;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);
  WiFi.begin("alexalr", "december");

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(touch_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(yellow_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(sevencolorflash_pin, OUTPUT);

  server.on("/", handleRoot);
  server.on("/control", handleControl);

  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() {
  server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html>
      <head>
        <style>
          body {
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #f0f0f0;
          }
          .container {
            height: 100%;
            width: 100%;
            text-align: center;
            background-color: #fff;
            padding: 20px;
          }
          h1 {
            font-size: 42pt;
            padding-top: 5rem;
            text-align: center;
            color: #333;
          }
          h3 {
            font-size: 32pt;
            text-align: center;
            color: #333;
          }
          .buttons {
            display: flex;
            justify-content: center;
          }
          .button-gap {
            margin-right: 20px;
          }
          button {
            margin-bottom: 30px;
            padding: 20px;
            font-size: 32pt;
            font-weight: bold;
            color: #fff;
            background-color: #0baac6;
            border: none;
            border-radius: 6px;
            cursor: pointer;
            transition: background-color 0.3s;
          }
          button:hover {
            background-color: #0056b3;
          }
          p {
            text-align: center;
            padding-top: 30px;
            font-weight: bold;
            font-size: 20pt;
          }
        </style>
      </head>
      <body>
        <div class="container">
          <h1>SAMSUGN SMART HOME</h1>
          <h3>Lampu Teras</h3>
          <div class="buttons">
            <button class="button-gap" onclick="controlLight('green', 'on')">
              ON
            </button>
            <button onclick="controlLight('green', 'off')">OFF</button><br />
          </div>

          <h3>Lampu Dapur</h3>
          <div class="buttons">
            <button class="button-gap" onclick="controlLight('blue', 'on')">
              ON
            </button>
            <button onclick="controlLight('blue', 'off')">OFF</button><br />
          </div>
          <h3>Lampu Kamar Utama</h3>
          <div class="buttons">
            <button class="button-gap" onclick="controlLight('yellow', 'on')">
              ON
            </button>
            <button onclick="controlLight('yellow', 'off')">OFF</button><br />
          </div>
          <h3>Lampu Toilet</h3>
          <div class="buttons">
            <button class="button-gap" onclick="controlLight('red', 'on')">
              ON
            </button>
            <button onclick="controlLight('red', 'off')">OFF</button><br />
          </div>
          <h3>Lampu Disko</h3>
          <div class="buttons">
            <button class="button-gap" onclick="controlLight('sevencolorflash', 'on')">
              ON
            </button>
            <button onclick="controlLight('sevencolorflash', 'off')">OFF</button><br />
          </div>
          <h3>Alarm</h3>
          <div class="buttons">
            <button class="button-gap" onclick="controlLight('buzzer', 'on')">
              ON
            </button>
            <button onclick="controlLight('buzzer', 'off')">OFF</button><br />
          </div>
          <script>
            function controlLight(light, state) {
              var xhttp = new XMLHttpRequest();
              xhttp.open("GET", "control?light=" + light + "&state=" + state, true);
              xhttp.send();
            }
          </script>
          <p>© Alexis Divasonda Sigat Ngaing - 210711407. 2024. FTI UAJY. IoT</p>
        </div>
      </body>
    </html>
  )rawliteral");
}

void handleControl() {
  String light = server.arg("light");
  String state = server.arg("state");
  int pin;

  if (light == "green") {
    pin = green_pin;
  } else if (light == "blue") {
    pin = blue_pin;
  } else if (light == "yellow") {
    pin = yellow_pin;
  } else if (light == "red") {
    pin = red_pin;
  } else if (light == "sevencolorflash") {
    pin = sevencolorflash_pin;
  } else if (light == "buzzer") {
    pin = buzzer_pin;
  } else {
    server.send(400, "text/plain", "Invalid light");
    return;
  }

  if (state == "on") {
    digitalWrite(pin, HIGH);
  } else if (state == "off") {
    digitalWrite(pin, LOW);
  } else {
    server.send(400, "text/plain", "Invalid state");
    return;
  }

  server.send(200, "text/plain", "OK");
}

void loop() {
  server.handleClient();
  val = digitalRead(touch_pin);
  if (val == HIGH) {
    digitalWrite(buzzer_pin, HIGH);
  } else {
    digitalWrite(buzzer_pin, LOW);
  }
}
