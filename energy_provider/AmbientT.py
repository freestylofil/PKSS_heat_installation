import numpy as np


class AmbientTemperature:

    def __init__(self, temperature=0.0):
        self._temperature = temperature

    @property
    def temperature(self) -> float:
        return self._temperature

    @temperature.setter
    def temperature(self, hour) -> None:
        self.__calculate_temperature(hour)

    def __calculate_temperature(self, hour) -> object:
        if hour < 6:
            self._temperature -= 0.875 + (np.random.normal(0, 0.3))
        elif hour < 16:
            self._temperature += 1 + (np.random.normal(0, 1))
        elif hour < 22:
            self._temperature -= 0.5 + (np.random.normal(0, 0.3))
        else:
            self._temperature -= 0.875 + (np.random.normal(0, 0.3))
