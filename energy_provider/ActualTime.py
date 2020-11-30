from datetime import datetime, timedelta
import ntplib

time_client = ntplib.NTPClient()


class ActualTime:

    def __init__(self, date=datetime.now()):
        self._date = date
        self._date0 = date
        self._period = timedelta(minutes=5)

    @property
    def date(self) -> datetime:
        return self._date

    @date.setter
    def date(self, date: datetime) -> None:
        self._date = date

    def get_timestamp(self):
        return int(datetime.timestamp(self._date))

    def check_period(self) -> bool:
        if (self._date - self._date0) >= self._period:
            self._date0 = self.date
            return True
        else:
            return False
        #return False if timedelta.total_seconds((self._date0 - self._date) % self._period) else True


def get_time(host, port) -> datetime:
    now = time_client.request(host, 3,port)
    return datetime.fromtimestamp(int(now.tx_time))
