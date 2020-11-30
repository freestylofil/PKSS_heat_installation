import numpy as np


class WaterTemperature:
    def __init__(self):
        self._temperature = 85.0

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

    def failure(self):
        return True if abs(np.random.normal(0, 0.25)) > 1 else False
