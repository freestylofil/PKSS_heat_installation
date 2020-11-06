#pragma once


class RegulatorPID
{
public:
	RegulatorPID(double P, double I, double D, double N = 10.0);
	double operator()(double y, double dt);
private:
	const double P;
	const double I;
	const double D;
	const double N;
	double integralSum = 0;
	double filterSum = 0;
	double filterPrevious = 0;
	double previous = 0;
};

