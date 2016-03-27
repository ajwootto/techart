import json
import sys
import urllib2
import lxml
import lxml.html
import serial
import threading

open_weather_key = "48dfc67c29ac7993c20359a807ef1308"
open_weather_base = "http://api.openweathermap.org/data/2.5/group?appid=48dfc67c29ac7993c20359a807ef1308&units=metric&id="
aqi_base_url = 'http://aqicn.org/city/tokyo' 


CITY_LIST = ['tokyo', 'toronto', 'beijing', 'london', 'singapore']
current_city_index = 0
ELEMENTS = ['rain', 'temp', 'wind', 'aqi']
MAX_ELEMENTS = {'rain': 20, 'temp': 40, 'wind': 75, 'aqi': 150}

cities = {'tokyo': '1850147', 'toronto': '6167865', 'beijing': '1816670', 'london': '2643743', 'singapore': '1880252'}
weather_cities = {}

SERIAL_PORTS = {'rain': '/dev/ttyACM0', 'temp': '/dev/ttyACM1', 'wind': '/dev/ttyACM2', 'aqi': '/dev/ttyACM3'}
SERIAL_COMS = {}

for port in SERIAL_PORTS.keys():
  try:
    SERIAL_COMS[port] = serial.Serial(SERIAL_PORTS[port], 9600)
  except:
    print "No Arduino Found For: " + port + " " + SERIAL_PORTS[port]
    SERIAL_COMS[port] = None


for city in cities.keys():
  weather_cities[city] = {}

id_string = []
for city in cities.keys():
  id_string.append(cities[city])

MAX_TEMP = 40
MAX_PRECIP = 20
MAX_WIND = 75
MAX_AQI = 150


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
    if rain and rain.get('1h'):
      rain = rain['1h']
    elif rain and rain.get('3h'):
      rain = rain['3h']
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

def write_serial(element, value):
  if SERIAL_COMS[element]:
    SERIAL_COMS[element].write(value)

def update_current_city():
  global current_city_index
  current_city_index += 1
  if current_city_index > len(CITY_LIST) - 1:
    current_city_index = 0


update_weather()

def calculate_output(value, max_val):
  return 205*value/max_val + 50



print weather_cities



class WeatherThread(threading.Thread):
    def __init__(self, event):
        threading.Thread.__init__(self)
        self.stopped = event

    def run(self):
        while not self.stopped.wait(900):
          update_weather()
          print weather_cities

class SerialThread(threading.Thread):
    def __init__(self, event):
        threading.Thread.__init__(self)
        self.stopped = event

    def run(self):
        while not self.stopped.wait(5):
          for element in ELEMENTS:
            cur_city = CITY_LIST[current_city_index]
            write_serial(element, 'C ' + cur_city)
            value = calculate_output(float(weather_cities[cur_city][element]), MAX_ELEMENTS[element])
            write_serial(element, "V " + str(round(value)))
          update_current_city()



stop = threading.Event()
weather_thread = WeatherThread(stop)
weather_thread.daemon = True
weather_thread.start()

serial_thread = SerialThread(stop)
serial_thread.daemon = True
serial_thread.start()

while True:
  for com in SERIAL_COMS.keys():
    if SERIAL_COMS[com]:
      SERIAL_COMS[com].readline()

  if not weather_thread.isAlive() or not serial_thread.isAlive():
    raise Exception("WE DEAD")








