# ESP32-S3 Audio Alarm Clock

A digital alarm clock built on the ESP32-S3 platform. This project features a color TFT interface, high-fidelity I2S audio streaming, and a precise hardware Real-Time Clock (RTC). Navigation is handled via an intuitive dual-rotary encoder system.

## ✨ Key Features
- **Precise Timekeeping:** Utilizes the DS3231 RTC module for high accuracy, with built-in battery backup.
- **High-Quality Audio:** I2S-based audio playback (via `ESP32-audioI2S`) for internet radio or MP3 alarm sounds.
- **Rich UI:** 2.4" or 2.8" ILI9341 TFT display support with a custom-built menu system.
- **Dual Encoder Control:** 
    - **Left Encoder:** Handles volume control and list scrolling.
    - **Right Encoder:** Handles menu navigation, confirming options, and exiting screens.
- **NTP Synchronization:** Built-in support to sync the hardware RTC with internet time servers (NTP).
- **Dynamic Alarm Management:** Add, delete, and toggle multiple alarms directly from the device interface.
- **PSRAM Optimized:** Specifically configured to utilize the 8MB PSRAM of the ESP32-S3 for smooth audio buffering.

## 🛠️ Hardware Requirements
- **Microcontroller:** ESP32-S3 (DevKitC-1-N16R8)
- **Display:** ILI9341 TFT LCD (SPI)
- **RTC:** DS3231 (I2C)
- **Audio:** I2S DAC (e.g., MAX98357A, PCM5102, or internal S3 pins)
- **Input:** 
    - 2x Rotary Encoders with Push Buttons
    - 1x Physical Button (for Snooze/Stop)

## 📦 Software & Libraries
The project is built using **PlatformIO** and requires the following libraries:
- `Adafruit GFX` & `Adafruit ILI9341` (Graphics)
- `ESP32-audioI2S` (Audio streaming)
- `DS3231` by northernwidget (RTC management)
- `Rotary` (Encoder handling)

## ⚙️ Configuration (platformio.ini)
The project is pre-configured for the **ESP32-S3-DevKitC-1-N16R8** with 16MB Flash and 8MB PSRAM. 

```ini
[env:ESP32-S3-DevKitC-1-N16R8]
platform = espressif32
board = ESP32-S3-DevKitC-1-N16R8
framework = arduino
build_flags = 
    -DBOARD_HAS_PSRAM
    -mfix-esp32-psram-cache-issue
board_build.filesystem = littlefs
board_build.partitions = boards/default_16MB.csv
```

## 🕹️ User Interface Guide
*   **Main Screen:** Displays the current time, active alarms, and volume level (`V=X`).
*   **Volume Control:** Rotate the **Left Encoder** on the main screen to adjust volume (0-10).
*   **Enter Menu:** Press the **Left Encoder** button to open the Main Menu.
*   **Navigation:**
    *   **Left Encoder:** Scroll through alarm lists or change time values (hours/minutes).
    *   **Right Encoder:** Switch between "Back", "Add", and "Delete" options.
*   **Stopping Alarms:** When the alarm triggers, press the dedicated **Hardware Button** to stop the audio stream.

## 🚀 Getting Started
1.  **Clone the Repository:**
    ```bash
    git clone https://github.com/YOUR_USERNAME/YOUR_PROJECT_NAME.git
    ```
2.  **Open in VS Code:** Ensure you have the **PlatformIO** extension installed.
3.  **Check Pin Definitions:** Open `defines.h` and verify that the GPIO pins match your wiring.
4.  **Build and Upload:** Connect your ESP32-S3 and click the **Upload** icon in the PlatformIO toolbar.

## ⚖️ Copyright & Usage

This project was created solely for my professional portfolio and demonstration purposes. All rights reserved. No part of this repository may be used, redistributed, or modified without my explicit permission.