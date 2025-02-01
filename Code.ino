#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Preferences.h>

// Initialize web server and WiFi variables
WebServer server(80);
Preferences preferences;
char ssid[32] = "DESKTOP-LCQEGRI 4231";
char password[32] = "#######";    // hotsport Password

// HTML page for web server
const char* html_page = R"=====(
<!DOCTYPE html>
<html>
<body>
  <h1>Manage WiFi Connections</h1>
  <form action="/save">
    SSID: <input type="text" name="ssid"><br>
    Password: <input type="password" name="password"><br>
    <input type="submit" value="Save">
  </form>
  <form action="/remove">
    <input type="submit" value="Remove">
  </form>
</body>
</html>
)=====";

// Handle root path
void handleRoot() {
  server.send(200, "text/html", html_page);
}

// Handle save credentials
void handleSave() {
  String newSSID = server.arg("ssid");
  String newPassword = server.arg("password");

  if (newSSID.length() > 0 && newPassword.length() > 0) {
    newSSID.toCharArray(ssid, 32);
    newPassword.toCharArray(password, 32);

    // Save credentials to NVS
    preferences.putString("ssid", newSSID);
    preferences.putString("password", newPassword);

    server.send(200, "text/html", "Saved! Rebooting...");
    delay(2000);
    ESP.restart();
  } else {
    server.send(200, "text/html", "SSID/Password cannot be empty");
  }
}

// Handle remove credentials
void handleRemove() {
  WiFi.disconnect();

  // Clear credentials from NVS
  preferences.clear();

  server.send(200, "text/html", "Removed! Rebooting...");
  delay(2000);
  ESP.restart();
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect. Starting AP mode.");
    WiFi.softAP("ESP32-Setup");
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP Address: ");
    Serial.println(IP);
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize SPIFFS and Preferences
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  preferences.begin("wifi", false);

  // Retrieve stored credentials
  String storedSSID = preferences.getString("ssid", "");
  String storedPassword = preferences.getString("password", "");

  if (storedSSID.length() > 0 && storedPassword.length() > 0) {
    storedSSID.toCharArray(ssid, 32);
    storedPassword.toCharArray(password, 32);
  }

  connectToWiFi();

  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.on("/remove", handleRemove);
  server.begin();
}

void loop() {
  server.handleClient();
}
