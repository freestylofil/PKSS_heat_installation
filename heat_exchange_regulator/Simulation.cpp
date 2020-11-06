#include "Simulation.h"
#include <algorithm>

Simulation::OutputData Simulation::step(const InputData& input)
{
    double u = 55.0 - 1.75 * input.To;
    double error = u - input.Tzco;
    double Fzm = pid(error, input.dt);
    return { std::clamp(Fzm, 0.0, FzmMax), input.Tm };
}
