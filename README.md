# BUS STOP PROJECT
The following project implements a small Bus Stop sign (located in Athens/Greece). The purpose of the project is to inform the user about bus arrival times on an area of choice. 

Τo do so we combine the power and wifi connectivity of an **ESP32** microcontroller with the **OASA TELEMATICS API**, μaking requests to the appropriate endpoints in order to obtain the bus arrival times for a specific bus stop. 

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
    ├── include/    # header files
    │   ├── api_requests.h
    │   ├──lcd_i2c.h
    │   └── wifi_module.h
    ├── src/
    │   ├── modules/ 
    │   │   ├── api_request.cpp     # OASA API request handler
    │   │   ├── lcd_i2c.cpp         # DATA to LCD connection via I2C
    │   │   └── wifi_module.cpp     # WiFi connection handler
    │   └── main.cpp
```

## LIVE TEST IMAGES 
On a live test (image below) we can see that in our specified bus stop the bus with id:3 arrives in 6 minutes and covers the route from N.Filadelfia to Neo Psyxiko.
![Alt text](/assets/demo_1.jpg)


# Creator
Evangelos Stamatis. 
For more information and questions regarding the project setup and functionality please contant me on the email: evanstamatis@gmail.com 