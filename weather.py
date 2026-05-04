import requests
import serial
import time

# 1. WeatherOpen API key 
API_KEY = "" 
CITY = "Cheyenne"
SERIAL_PORT = "COM4" 
BAUD_RATE = 115200

# 2. Setup Serial Connection
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2) 
    print("Connected to ESP32 on COM4")
except Exception as e:
    print(f"Error: {e}")
    print("Make sure the Arduino Serial Monitor is closed!")

def get_weather(city, api_key):
    url = f"http://api.openweathermap.org/data/2.5/weather?q={city}&appid={api_key}&units=imperial"
    try:
        response = requests.get(url)
        return response.json() if response.status_code == 200 else None
    except:
        return None

# 3. Main Loop
last_weather_check = 0
print("Starting Weather and Sensor Sync...")

while True:
    # A. Check for Room Temp 
    if ser.in_waiting > 0:
        raw_line = ser.readline().decode('utf-8', errors='ignore').strip()
        if "ROOM_TEMP" in raw_line:
            try:
                r_temp = raw_line.split(":")[1]
                print(f"--- Room Temperature: {r_temp}°F ---")
            except IndexError:
                pass

    # B. Check Cheyenne Weather (Every 30 seconds)
    current_time = time.time()
    if current_time - last_weather_check > 30:
        data = get_weather(CITY, API_KEY)
        if data:
            c_temp = data['main']['temp']
            print(f"\n--- Cheyenne Weather: {c_temp}°F ---")
            ser.write(f"{c_temp}\n".encode()) # This tells the LEDs what to do
        last_weather_check = current_time

    time.sleep(0.1)