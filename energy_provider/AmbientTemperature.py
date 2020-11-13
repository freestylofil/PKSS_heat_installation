import xlrd


class AmbientTemperature:
    def __init__(self, location):
        self._temperatures = xlrd.open_workbook(location).sheet_by_index(0).col_slice(1)
        self._temperatures = [i.value for i in self._temperatures]
        self._temperatures.pop(0)

    @property
    def temperature(self):
        return self._temperatures.pop(0)

