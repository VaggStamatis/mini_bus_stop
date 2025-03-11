import requests
# TODO Map the route_codes from the respone_times to the LineID from the bus stop information

# 061061 bus stop (Άγιος Λουκάς Πατησίων to  Πολυτεχνίο/Ακαδημία/Σύνταγμα !)
# gets the information for the specified Bus stop
response_info = requests.get("http://telematics.oasa.gr/api/?act=webRoutesForStop&p1=061061")

# gets the bus arrival times for the specified bus stop
response_times = requests.get("http://telematics.oasa.gr/api/?act=getStopArrivals&p1=061061")

# format to json obj
json_response_times = response_times.json()
json_response_info = response_info.json()

# print the data
print(json_response_times)
print(json_response_info)
