#pragma once
#include "RegulatorPID.h"
#include <vector>

class Simulation
{
public:
	struct InputData
	{
		double dt;
		double To;
		double Tm;
		double Tzco;
	};
	struct OutputData
	{
		double Fzm;
		double Tzm;
	};
public:
	Simulation() = default;
	Simulation::OutputData step(const InputData& input);
private:
	RegulatorPID pid = RegulatorPID{ 0.00137219220753553,
									 1.3918405587825e-06,
									 0.048213073117336,
									 0.774293501527489 };
	static constexpr double FzmMax = 80.0*3600.0;
};

