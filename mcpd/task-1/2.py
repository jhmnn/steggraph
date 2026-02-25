import requests
import json

api_key = "b5c286be7f96b8e393f5ac63b1c44f77"
city = "Moscow"

geo_url = f"https://api.openweathermap.org/geo/1.0/direct?q={city}&limit=1&appid={api_key}"
geo_response = requests.get(geo_url)
geo_data = geo_response.json()

if not geo_data:
    print("city not found")
    exit()

lat = geo_data[0]["lat"]
lon = geo_data[0]["lon"]

weather_url = f"https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={api_key}"
weather_response = requests.get(weather_url)

if weather_response.status_code == 200:
    data = weather_response.json()
    with open("weather.json", "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=4)
    print("saved to weather.json")
else:
    print("error:", weather_response.status_code)
