# 📡 ESP32 WiFi Manager using Web Server

This project allows an ESP32 to connect to WiFi using stored credentials and fall back to Access Point mode if the connection fails. It features a web-based UI to:

- Add/Save WiFi SSID & Password
- Remove WiFi credentials
- Automatically reconnect on reboot

---

## ⚙️ Features

- WiFi Auto-connect using stored credentials in NVS (Preferences)
- Fallback to Access Point (AP) mode if WiFi fails
- Web portal to:
  - Enter SSID and Password
  - Save credentials (stored in NVS)
  - Remove credentials
- Automatic reboot on save or removal

---

## 🛠️ Hardware Required

- ESP32 Dev Board
- USB Cable
- WiFi Connection (Hotspot or Router)

---

## 🧰 Libraries Used

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Preferences.h>
🚀 How It Works
ESP32 tries to connect to the last saved WiFi credentials.

If failed, it starts an Access Point (AP) with SSID: ESP32-Setup.

You can connect to this AP and open 192.168.4.1 to enter new credentials.

Credentials are saved to non-volatile storage (NVS) using Preferences.

On next boot, it will auto-connect using saved credentials.

🧪 Testing
Connect to ESP32-Setup if no WiFi is saved.

Enter SSID and password on the portal.

Press Save.

ESP32 restarts and connects to WiFi.

IP is printed via Serial Monitor.

📂 File Structure
arduino
Copy
Edit
ESP32-WiFi-Manager/
├── ESP32_WiFi_Manager.ino
├── data/  <-- optional for SPIFFS HTML
└── README.md
🖼️ Web Portal HTML
html
Copy
Edit
<form action="/save">
  SSID: <input type="text" name="ssid"><br>
  Password: <input type="password" name="password"><br>
  <input type="submit" value="Save">
</form>
<form action="/remove">
  <input type="submit" value="Remove">
</form>
🔄 API Routes
Endpoint	Description
/	HTML page for credential input
/save	Save new WiFi credentials
/remove	Remove saved credentials

📦 Dependencies
Install these libraries in Arduino IDE (included by default with ESP32 core):

WiFi

WebServer

SPIFFS

Preferences

🛡️ License
MIT License - free to use, modify, and distribute.

yaml
Copy
Edit

---

## 📊 Flowchart

Below is the process flowchart described in text format. You can use it with draw.io or Lucidchart to convert it to a visual diagram.

pgsql
Copy
Edit
      +------------------------+
      |  ESP32 Boot Starts     |
      +-----------+------------+
                  |
                  v
     +-----------------------------+
     | Load WiFi SSID & Password   |
     | from Preferences (NVS)      |
     +-------------+---------------+
                   |
      +------------+-------------+
      |                          |
      v                          v
+------------------+ +---------------------------+
| Credentials Found| | No Credentials Found |
+--------+---------+ +------------+--------------+
| |
v v
+---------------------------+ +-----------------------+
| Connect to Saved WiFi | | Start AP Mode |
| (20 attempts max) | | SSID: ESP32-Setup |
+------------+--------------+ +-----------+-----------+
| |
+-------+-------+ +-------+-------+
| Connected OK? | | User connects |
+-------+-------+ +-------+-------+
| |
+-------+-------+ +-------+---------------+
| YES | | Access Web Server UI |
| Show IP Addr | | Form: SSID & Password|
+---------------+ +-----------+-----------+
|
+--------------+--------------+
| /save -> Store to NVS |
| /remove -> Clear NVS |
+--------------+--------------+
|
+---------v----------+
| Restart ESP32 |
+----------------------+
