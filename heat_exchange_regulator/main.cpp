#include <iostream>
#include "Simulation.h"

int main()
{
	double dt = 0.1;
	double To = 75.0;
	double Tm = 70.0;
	double Tzco = 70.0;
	Simulation sim;
	while (true)
	{
		Simulation::OutputData out = sim.step({ dt, To, Tm, Tzco });
		Tzco = 1.5 * out.Tzm - 7.0;
	}
}