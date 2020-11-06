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
	RegulatorPID pid = RegulatorPID{ 0.7, 0.1, 0.5, 10.0 };
	static constexpr double FzmMax = 80.0;
};

