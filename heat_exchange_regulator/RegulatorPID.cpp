#include "regulatorPID.h"

RegulatorPID::RegulatorPID(double P, double I, double D, double N)
	:
	P(P), I(I), D(D), N(N)
{
}

double RegulatorPID::operator()(double y, double dt)
{
	integralSum += previous * dt;
	filterSum += filterPrevious * dt;
	previous = y;
	filterPrevious = (D * y - filterSum) * N;
	return P * y + integralSum * I + filterPrevious;
}
