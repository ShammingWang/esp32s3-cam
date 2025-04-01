# ESP32-S3 Project

This repository focuses on developing applications for the ESP32-S3 using the ESP-IDF environment.

## Overview

- **main**: Contains the main component source files for the ESP32-S3 project.
- **hand-gesture-recognition-mediapipe**: Includes a gesture recognition backend using MediaPipe. Inside this folder, the `app_service.py` file serves as a backend service for hand gesture recognition.

## Prerequisites

- ESP-IDF installed and configured for ESP32-S3.
- Python 3 environment (required for the gesture recognition backend).

## Build Instructions

1. Open a terminal and navigate to this project folder.
2. Set up the ESP-IDF environment.
3. Run:
   ```
   idf.py set-target esp32s3
   idf.py menuconfig
   idf.py build
   ```
4. Flash the firmware to your device:
   ```
   idf.py flash
   idf.py monitor
   ```

## Backend Service

To run the hand gesture recognition backend:

```
cd hand-gesture-recognition-mediapipe
python app_service.py
```

This starts a local service that processes camera streams or images to recognize gestures.

## License

Please refer to individual files and components for license details. Use this software at your own risk and follow any applicable guidelines.
