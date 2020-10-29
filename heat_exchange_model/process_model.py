import numpy as np
from scipy.integrate import odeint


class ProcessModel:

    def __init__(self, x0):
        self.x0 = {'Tpm': x0[0],
                   'Tzco': x0[1]}

    def system(self, x0, t, Tzm=90, Fzm=40, Fzco=150, Tpco=75):

        Mm = 2995
        Mco = 3028
        c_wym = 2699
        ro = 1000
        c_w = 4205
        k_w = 250000
        Tpm = x0[0]
        Tzco = x0[1]
        dTpmdt = (Fzm*ro*c_w*(Tzm-Tpm)-k_w*(Tpm-Tzco))/(Mm*c_wym)
        dTzcodt = (-1*Fzco*ro*c_w*(Tzco-Tpco)+k_w*(Tpm-Tzco)) / (Mco * c_wym)
        return [dTpmdt, dTzcodt]

    def simulate(self, arguments, delta=1, mock=False):
        coeffs = tuple(arguments)
        t = np.linspace(0, delta, 100)
        if mock:
            x = odeint(self.system, [self.x0['Tpm'], self.x0['Tzco']], t) #default_mock_values
        else:
            x = odeint(self.system, [self.x0['Tpm'], self.x0['Tzco']], t, args=coeffs)
        # print(x) # for testing
        self.x0['Tpm'] = x[-1, 0]
        self.x0['Tzco'] = x[-1, 1]

