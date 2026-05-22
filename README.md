# BUS STOP PROJECT
The following project implements 1 to 4 Bus Stop signs (located in Athens/Greece). The purpose of the project is to inform the user about bus arrival times on each bus stop by displayingeach bus stop one at a time.  

Τo do so we combine the power and wifi connectivity of an **ESP32** microcontroller with the **OASA TELEMATICS API**, making requests to the appropriate endpoints in order to obtain the bus arrival times for a specific bus stop. 

## Platform 
|      Platform     | Language |                 IDE                           | 
| ------------------| ---------| --------------------------------------------- |
| ESP32 DevKit C v4 |    C++   | Visucal studio Code with platformIO extension |


## Hardware and Wiring
In order to build the project we need:
- An EPS32 devkit C v4 with wifi module
- An LCD 16x2 monitor
- An I2C module (to communicate and send data to the LCD)
- 4 wires for the connections

You can follow the schema below to implement the projects wiring.
![Alt text](/assets/wiring.png)
If you don't have an LCD with a build in I2C module then you can connect the I2C module to the screen and make the corresponding connections between the I2C module and the ESP32. 

## API Explanation
The OASA Telematics API (https://oasa-telematics-api.readthedocs.io/en/latest/getLinesAndRoutesForMl.html) is an API used to fetch data regarding bus stops and bus arrival times based on Athens/Greece. To use the API and make requests we need the 6-digit number of the bus stop we are intreseted in (can be found using the app, or on the stop sign on the street).

The 2 endpoints we are interested in are the following

```bash
# Obtain the arrival times of the buses based on the XXXXXX (6-digit) bus stop code 
"http://telematics.oasa.gr/api/?act=getStopArrivals&p1=XXXXXX"


# Obtain the bus lines information based on the XXXXXX (6-digit) bus stop code
"http://telematics.oasa.gr/api/?act=webRoutesForStop&p1=XXXXXX"
```
Later by combining the ariival times and the line information we can create a finalized data table looking like: 
| Id   | Time | Description                                              |
|------|------|----------------------------------------------------------|
| 14   | 2    | LAMPRINI - L.ALEXANDRAS - GIROKOMEIO                    |
| 3    | 8    | N. FILADELFEIA - ANO PATISIA - NEO PSYCHIKO            |
| 608  | 14   | GALATSI - AKADIMIA - NEKR. ZOGRAFOU                    |
| Α8   | 35   | MAROUSI - N. IONIA - POLYTECHNEIO                      |
| Α8   | 50   | MAROUSI - N. IONIA - POLYTECHNEIO                      |

## Project Structure
```bash
├── mini_bus_stop/
    ├── data/    # index html file serving the UI
    │   └── index.html # UI of the project
    ├── src/
    │   ├── api/    # WiFi connection handler
    │   ├── config/     # SPIFFS services
    │   ├── lcd/        # lcd handling funcitons
    │   ├── models/     # base model of BusArrival
    │   ├── services/   # services for merging data
    │   ├── state/      # implementation of the Algorithmic state machine
    │   ├── web/        # handles web server functionality
    │   ├── wifi/       # handels wifi connectivity
    │   └── main.cpp
```
## Version 2 Algorithmic State Machine Diagram
``` bash
            APP_WAIT_WIFI
                  |
                  |
             |-----------|
             |           |
        APP ERROR     APP_READY
                         |
                  APP_NEXT_BUS_STOP ←--
                         |            ↑
                  APP_FETCHING_DATA   |
                         |            |
                  APP_DISPLAY_DATA    |
                         |            |
                         ------------→|

```

## SET UP AND UI EXPLANATION
When you set up the device for the first time you simply power it up with the appropriate usb cable type your ESP32 requires and you will see a message on the screen with an IP address probably something like : 192.168.4.1
Open a computer or a selfphone and navigate to the wifi settings. Connect to EPS32 Setup wifi. Open a browser type the IP printed on your screen and you will be redirected to the following User interface.

![Alt text](/assets/UI.png)
There you can fill the fields accordingly and set the whole device to your likings with your WiFi credentials and the 4 bus stops you are most intrested at.

## 3D Printed Case
Huge thanks to @geokscott for the amazing 3d printable case that fits the projects perfectly. 
You can find the files for the whole print with the following link:
> https://www.printables.com/model/611802-esp32-16x2-lcd-lora-case-enclosure/files

## LIVE TEST IMAGES 
On a live test (image below) we can see that in our specified bus stop the bus with id:608 arrives in 1 minute and heading to Zografou Graveyard (Νεκροταφείο (GR) = Graveyard).
![Alt text](/assets/arrivaltimePrint.JPG)



### Useful Commands

Upload html to spiffs in order to serve it to user 
> pio run -t uploadfs

Upload firmware to target 
> pio run -t upload 

Open serial monitor
> pio device monitor

Erase Flash (Spiffs)
> python -m esptool --chip esp32 erase-flash


# Creator
Evangelos Stamatis. 
For more information and questions regarding the project setup and functionality please contant me on the email: evanstamatis@gmail.com 

## YouTube video 
https://youtu.be/CQ-oXIufYDI

## TO DO
- Print only the 3 or 5 most recently coming buses to each stop
- Bigger screen for more data 