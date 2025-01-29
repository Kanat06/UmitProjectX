# Project: Interactive Mouse Controller

This project allows controlling a computer's mouse via magnetometer data received over a UDP connection from an Arduino device. The system includes both a **Python script** (for mouse control on the computer) and an **Arduino sketch** (for collecting magnetometer data).

## 📂 Project Structure
- `/python-code/`  
    - `main.py` - Python script that listens for UDP data and controls the mouse movement based on magnetometer values.
- `/arduino-code/`  
    - `umit.ino` - Arduino sketch that collects accelerometer, gyroscope, and magnetometer data and sends it over UDP.

## 🚀 How to Run

### 1. **Python Code**:
   - Install required libraries:
     ```bash
     pip install pynput
     ```
   - Run the script:
     ```bash
     python python-code/main.py
     ```

### 2. **Arduino Code**:
   - Upload the `umit.ino` sketch to your **Arduino** board using the **Arduino IDE**.
   - The Arduino will start collecting sensor data and send it over UDP to the Python script.

## 📜 License
This project is distributed under the **MIT License**.

