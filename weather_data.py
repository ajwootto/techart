import json
import sys
import urllib2
import lxml
import lxml.html
import serial


open_weather_key = "48dfc67c29ac7993c20359a807ef1308"
open_weather_base = "http://api.openweathermap.org/data/2.5/group?appid=48dfc67c29ac7993c20359a807ef1308&units=metric&id="
aqi_base_url = 'http://aqicn.org/city/tokyo' 

cities = {'tokyo': '1850147', 'toronto': '6167865', 'beijing': '1816670', 'london': '2643743', 'singapore': '1880252'}
weather_cities = {}

for city in cities.keys():
  weather_cities[city] = {}

id_string = []
for city in cities.keys():
  id_string.append(cities[city])

MAX_TEMP = 40
MAX_PRECIP = 20
MAX_WIND = 75
MAX_AQI = 150

current_city = "toronto"

def aqi_city(city):
  if city=='singapore':
    req = urllib2.Request('http://aqicn.org/city/' + city + "/south")
  else:
        req = urllib2.Request('http://aqicn.org/city/' + city)
  opener = urllib2.build_opener() 
  req.add_header("User-Agent", 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.87 Safari/537.36')
  html = opener.open(req).read()
  parsed = lxml.html.document_fromstring(html)
  elements = parsed.cssselect("td.aqiwgt-table-aqicell div.aqivalue")
  weather_cities[city]['aqi'] = elements[0].text_content()

def weather_city(ids):
  return json.load(urllib2.urlopen(open_weather_base + ids))

def massage_weather(weather):
  for data in weather['list']:
    #not every city has rain returned
    rain = data.get('rain')
    if rain:
      rain = rain['1h']
    else:
      rain = 0
    city_data = weather_cities[data['name'].lower()]
    city_data["rain"] = rain
    city_data["temp"] = data['main']['temp']
    city_data["wind"] = data['wind']['speed']

def update_weather():
  massage_weather(weather_city(','.join(id_string)))

  for city in cities.keys():
    aqi_city(city)


update_weather()

print weather_cities







