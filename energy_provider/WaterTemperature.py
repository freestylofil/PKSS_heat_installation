from typing import Any, Union


class WaterTemperature:
    def __init__(self):
        self._temperature = 77.0

    @property
    def temperature(self):
        return self._temperature

    @temperature.setter
    def temperature(self, To):
        if To < -20:
            self._temperature = 135
        elif To > 6:
            self._temperature = 70
        else:
            self._temperature = 70 - (2.5 * (To - 6))
