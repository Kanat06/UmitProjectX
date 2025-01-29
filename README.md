import socket
import json
from pynput.mouse import Controller

# Initialize mouse controller
mouse = Controller()

# Set up the UDP server
UDP_IP = "0.0.0.0"  # Listen on all interfaces
UDP_PORT = 7771

# Create UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# Calibration values for the magnetometer (adjust as needed)
magnetometer_offset = {'magX': 0, 'magY': 0, 'magZ': 0}

# Sensitivity values (adjust based on testing)
SENSITIVITY = 1  # Controls movement intensity

# Initial mouse position
initial_x, initial_y = mouse.position

# Print startup message
print(f"Listening on {UDP_IP}:{UDP_PORT}...")

def calibrate_magnetometer(data):
    """ Set the current readings as offsets to calibrate the sensor. """
    global magnetometer_offset
    magnetometer_offset = {
        'magX': data['magX'],
        'magY': data['magY'],
        'magZ': data['magZ']
    }
    print(f"Magnetometer calibrated with offset: {magnetometer_offset}")

def process_magnetometer_data(data):
    """ Process magnetometer data to control mouse movement. """
    # Apply calibration offsets
    magX = data['magX'] - magnetometer_offset['magX']
    magY = data['magY'] - magnetometer_offset['magY']
    magZ = data['magZ'] - magnetometer_offset['magZ']

    # Calculate movement based on sensor data
    x_move = magX * SENSITIVITY
    y_move = magY * SENSITIVITY

    # Print debugging information
    print(f"Raw Magnetometer: magX={magX}, magY={magY}, magZ={magZ}")
    print(f"Calculated movement: x_move={x_move}, y_move={y_move}")

    # Move the mouse
    current_pos = mouse.position
    new_x = int(current_pos[0] + x_move)
    new_y = int(current_pos[1] + y_move)

    mouse.position = (new_x, new_y)
    print(f"Moving mouse to: {new_x}, {new_y}")

while True:
    try:
        # Receive message
        data, addr = sock.recvfrom(1024)  # Buffer size 1024 bytes
        data_str = data.decode('utf-8', errors='ignore')  # Decode to string

        # Print received message
        print(f"Received message from {addr}: {data_str}")

        # Parse JSON
        sensor_data = json.loads(data_str)

        # Check for calibration request
        if sensor_data.get('calibrate', False):
            calibrate_magnetometer(sensor_data)
        else:
            process_magnetometer_data(sensor_data)

    except Exception as e:
        print(f"Error: {e}")
