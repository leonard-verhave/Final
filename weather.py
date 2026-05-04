import requests
import serial
import time

# API key for OpenWeather and the city where the user is in 
API_KEY = "88aa92248943c219a2a264a1bd3975d0" 
CITY = "Cheyenne"                         
SERIAL_PORT = 'COM4'                         
BAUD_RATE = 115200                     

# Seriel connection
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
    time.sleep(2) # Give the ESP32 time to reboot after connecting
    print(f"Connected to ESP32 on {SERIAL_PORT}")
except Exception as e:
    print(f"Connection Error: {e}")
    exit()

def get_weather():
    #Fetches live temperature data from the OpenWeather API.
    url = f"http://api.openweathermap.org/data/2.5/weather?q={CITY}&appid={API_KEY}&units=imperial"
    try:
        r = requests.get(url)
        data = r.json()
        if data["cod"] == 200:
            return data["main"]["temp"]
    except:
        return None

# Variable to track the weather last updated
last_weather_update = 0

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        if "Room Temp:" in line:
            print(line) # Print the breadboard sensor data in the Thonny Shell

    # Fetch and send Cheyenne weather once every 30 seconds
    current_time = time.time()
    if current_time - last_weather_update > 30:
        temp = get_weather()
        if temp:
            print(f"Cheyenne Temp: {temp} F") 
            ser.write(f"{temp}\n".encode())  # Send the number to the ESP32
            last_weather_update = current_time
    
    # Brief pause to prevent the CPU from overworking
    time.sleep(0.1)
