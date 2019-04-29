// WindSim.cpp : Defines the entry point for the console application.
//
#include <vector>
#include "WindSim.h"
using namespace Wind;

int main()
{
  	std::vector<double> hfloors = {288.0, 144.0, 144.0, 144.0, 144.0};
  //	std::vector<double> hfloors = {288.0};

	WindForces forces = GetWindForces(Wind::ExposureCategory::B, 90.0, 1.30, 40.0, hfloors, 100.0);
	std::vector<double> force1 = forces.getFloorForces(0);
	std::vector<double> force2 = forces.getFloorForces(1);
	double dt = forces.getTimeStep();

    return 0;
}

