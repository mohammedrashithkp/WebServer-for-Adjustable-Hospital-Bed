#include <WiFi.h>
#include <WebServer.h>

#define PWMupper 5
#define PWMlower 6
#define PWMside 9
#define DIRupper 4
#define DIRlower 8
#define DIRside 11

const char* ssid = <ssid>;
const char* password = <password>;

// Create a WebServer object on port 80
WebServer server(80);

void setup() {
  Serial.begin(9600);

  pinMode(PWMupper, OUTPUT);
  pinMode(PWMlower, OUTPUT);
  pinMode(PWMside, OUTPUT);
  pinMode(DIRupper, OUTPUT);
  pinMode(DIRlower, OUTPUT);
  pinMode(DIRside, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Route for root web page
  server.on("/", HTTP_GET, [](){
    server.send(200, "text/html", generateHTML());
  });

  // Define motor control routes
  server.on("/stop", HTTP_GET, [](){
    stopMotors();
    server.send(200, "text/plain", "Motors stopped");
  });

  server.on("/upper_up", HTTP_GET, [](){
    upperUp();
    server.send(200, "text/plain", "Upper body up");
  });

  server.on("/upper_down", HTTP_GET, [](){
    upperDown();
    server.send(200, "text/plain", "Upper body down");
  });

  server.on("/tilt_left", HTTP_GET, [](){
    tiltLeft();
    server.send(200, "text/plain", "Tilt left");
  });

  server.on("/tilt_right", HTTP_GET, [](){
    tiltRight();
    server.send(200, "text/plain", "Tilt right");
  });

  server.on("/lower_up", HTTP_GET, [](){
    lowerUp();
    server.send(200, "text/plain", "Lower up");
  });

  server.on("/lower_down", HTTP_GET, [](){
    lowerDown();
    server.send(200, "text/plain", "Lower down");
  });

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void stopMotors() {
  analogWrite(PWMside, 0);
  analogWrite(PWMlower, 0);
  analogWrite(PWMupper, 0);
}

void upperUp() {
  analogWrite(PWMupper, 255);
  digitalWrite(DIRupper, HIGH);
  analogWrite(PWMside, 0);
  analogWrite(PWMlower, 0);
}

void upperDown() {
  analogWrite(PWMupper, 255);
  digitalWrite(DIRupper, LOW);
  analogWrite(PWMside, 0);
  analogWrite(PWMlower, 0);
}

void tiltLeft() {
  analogWrite(PWMside, 255);
  digitalWrite(DIRside, HIGH);
  analogWrite(PWMupper, 0);
  analogWrite(PWMlower, 0);
}

void tiltRight() {
  analogWrite(PWMside, 255);
  digitalWrite(DIRside, LOW);
  analogWrite(PWMupper, 0);
  analogWrite(PWMlower, 0);
}

void lowerUp() {
  analogWrite(PWMlower, 255);
  digitalWrite(DIRlower, HIGH);
  analogWrite(PWMupper, 0);
  analogWrite(PWMside, 0);
}

void lowerDown() {
  analogWrite(PWMlower, 255);
  digitalWrite(DIRlower, LOW);
  analogWrite(PWMupper, 0);
  analogWrite(PWMside, 0);
}

// Function to generate the HTML and CSS for the web interface
String generateHTML() {
  String html = "<!DOCTYPE html><html lang='en'><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #2c3e50; color: white; text-align: center; padding: 20px; }";
  html += "h1 { color: #ecf0f1; font-size: 2.5rem; margin-bottom: 20px; }";
  html += ".container { display: flex; flex-wrap: wrap; justify-content: center; }";
  html += ".button { padding: 15px 30px; font-size: 18px; color: white; background-color: #1abc9c; border: none; border-radius: 10px; cursor: pointer; margin: 10px; transition: 0.3s; }";
  html += ".button:hover { background-color: #16a085; transform: translateY(-2px); }";
  html += ".button:active { background-color: #16a085; transform: translateY(1px); }";
  html += ".stop-button { background-color: #e74c3c; }";
  html += ".stop-button:hover { background-color: #c0392b; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>Motor Control Interface</h1>";
  html += "<div class='container'>";
  html += "<button class='button stop-button' onclick=\"fetch('/stop')\">Stop</button><br>";
  html += "<button class='button' onclick=\"fetch('/upper_up')\">Upper Up</button>";
  html += "<button class='button' onclick=\"fetch('/upper_down')\">Upper Down</button><br>";
  html += "<button class='button' onclick=\"fetch('/tilt_left')\">Tilt Left</button>";
  html += "<button class='button' onclick=\"fetch('/tilt_right')\">Tilt Right</button><br>";
  html += "<button class='button' onclick=\"fetch('/lower_up')\">Lower Up</button>";
  html += "<button class='button' onclick=\"fetch('/lower_down')\">Lower Down</button><br>";
  html += "</div>";
  html += "</body></html>";
  return html;
}
