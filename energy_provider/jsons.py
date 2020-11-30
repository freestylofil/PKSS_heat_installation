import json


def to_json(ip, timestamp, outdoor_t, water_t):
    to_js = {
        "IP": ip,
        "timestamp": timestamp,
        "To": outdoor_t,
        "Tzm": water_t
    }
    js = json.JSONEncoder().encode(to_js)
    return js

def to_json_log(str, timestamp, numeric):
    to_js = {"request": "PUT",
             "variable": str,
             "timestamp": timestamp,
             "value": numeric}
    js = json.JSONEncoder().encode(to_js)
    return js
"""for key in py:
    print(key, '->', py[key])
print(py["values"]["Water Temperature"])"""


def from_json(json_instance) -> dict:
    py = json.JSONDecoder().decode(json_instance)
    return py


def to_json_time_test(time):
    dt = {
        "year": time.date.year,
        "month": time.date.month,
        "day": time.date.day,
        "hour": time.date.hour,
        "minute": time.date.minute,
        "second": time.date.second
    }
    js = json.JSONEncoder().encode(dt)
    return js
