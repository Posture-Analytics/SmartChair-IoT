"""Matplotlib heatmap example for one VL53L5CX distance array sensor."""

import serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Set up serial connection (adjust the COM port and baud rate)
ser = serial.Serial('COM3', 115200, timeout=1)

# Define the plot
fig, ax = plt.subplots()
heatmap = ax.imshow(np.zeros((8, 8)), cmap='viridis', vmin=0, vmax=3000)  # Adjust vmax as needed

# Function to parse serial data
def parse_serial_data():
    data = []
    while len(data) < 64:
        line = ser.readline().decode().strip()
        if line:
            row = [int(val) for val in line.split('\t') if val.isdigit()]
            if len(row) == 8:
                data.extend(row)
    return np.array(data).reshape((8, 8))

# Update function for animation
def update(frame):
    if ser.in_waiting > 0:
        data_matrix = parse_serial_data()
        heatmap.set_data(data_matrix)
        return heatmap,

# Animation setup without `blit=True`
ani = animation.FuncAnimation(fig, update, interval=500)
plt.colorbar(heatmap)
plt.show()
