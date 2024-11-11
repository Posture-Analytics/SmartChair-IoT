"""Module to plot heatmaps of the distance data from two VL53L5CX sensors in real time."""

import serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Set up serial connection (adjust the COM port and baud rate)
ser = serial.Serial('COM3', 115200, timeout=1)

# Define the plot
fig, (ax1, ax2) = plt.subplots(1, 2)
heatmap1 = ax1.imshow(np.zeros((8, 8)), cmap='viridis', vmin=0, vmax=3000)  # Adjust vmax as needed
heatmap2 = ax2.imshow(np.zeros((8, 8)), cmap='viridis', vmin=0, vmax=3000)  # Adjust vmax as needed

# Function to parse serial data
def parse_serial_data():
    data1 = []
    data2 = []
    while len(data1) < 64 or len(data2) < 64:
        line = ser.readline().decode().strip()
        if line:
            print(line)
            values = [value.strip() for value in line.split('\t')]
            try:
                if values[0].startswith('1:'):
                    data1.extend(int(value.split(':')[1]) for value in values)
                elif values[0].startswith('2:'):
                    data2.extend(int(value.split(':')[1]) for value in values)
            except ValueError:
                continue
    
    return np.array(data1).reshape((8, 8)), np.array(data2).reshape((8, 8))

# Update function for animation
def update(frame):
    if ser.in_waiting > 0:
        data_matrix1, data_matrix2 = parse_serial_data()
        heatmap1.set_data(data_matrix1)
        heatmap2.set_data(data_matrix2)
        return heatmap1, heatmap2

# Animation setup without `blit=True`
ani = animation.FuncAnimation(fig, update, interval=500, cache_frame_data=False)
fig.colorbar(heatmap1, ax=ax1)
fig.colorbar(heatmap2, ax=ax2)
plt.show()
