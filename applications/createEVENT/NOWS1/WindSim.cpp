#include "WindSim.h"
#include "windsim_dk1_main.h"
#include "windsim_dk1_main_emxAPI.h"

namespace Wind
{
	WindForces GetWindForces(Wind::ExposureCategory category, double gustWindSpeed, double dragCoeff, double width, std::vector<double> floorsHeights, double seed)
	{
		//First let's convert exposure category
		char EC = 'A';
		switch (category)
		{
		case A:
			EC = 'A';
			break;
		case B:
			EC = 'B';
			break;	
		case C:
			EC = 'C';
			break;
		case D:
			EC = 'D';
			break;
		default:
			EC = 'A';
		}

		int size = 6000;

		emxArray_real_T* windForce;
		emxArray_real_T* windSpeed;

		emxInitArray_real_T(&windForce, 2);
		emxInitArray_real_T(&windSpeed, 2);
		windsim_dk1_main_initialize();
		
		int numFloors = floorsHeights.size();
		emxArray_real_T* HFloors = emxCreate_real_T(1, numFloors);
		
		for (int i = 0; i < numFloors; i++)
			HFloors->data[i] = floorsHeights[i];
		double timeStep;
		windsim_dk1_main(EC, gustWindSpeed, dragCoeff, width, HFloors, seed, windSpeed, windForce, &timeStep);

		WindForces forces(numFloors, size);
		for (int i = 0; i < numFloors; i++)
		{
			forces.setFloorForces(i, windForce->data + i*size);
		}

		forces.setTimeStep(timeStep);
		windsim_dk1_main_terminate();

		return forces;
	}

	WindForces::WindForces(int numFloors, int size)
	{
		this->m_numFloors = numFloors;
		this->m_size = size;

		this->m_data = new double*[m_numFloors];
		for (int i = 0; i < this->m_numFloors; i++)
			this->m_data[i] = new double[m_size];
	}

	WindForces::~WindForces()
	{
		for (int i = 0; i < this->m_numFloors; i++)
			delete[] this->m_data[i];
		delete[] this->m_data;
	}

	WindForces::WindForces(const WindForces& other)
	{
		this->m_size = other.m_size;
		this->m_numFloors = other.m_numFloors;
		this->m_data = new double*[m_numFloors];
		this->m_timeStep = other.m_timeStep;

		for (int i = 0; i < this->m_numFloors; i++)
		{
			this->m_data[i] = new double[m_size];
			memcpy(this->m_data[i], other.m_data[i], m_size * sizeof(double));
		}	
	}

	std::vector<double> WindForces::getFloorForces(int floorIndex)
	{
		std::vector<double>forces;
		forces.reserve(m_size);
		for (int i = 0; i < this->m_size; i++)
			forces.push_back(m_data[floorIndex][i]);
		return forces;
	}

	void WindForces::setFloorForces(int floorIndex, const double * vector)
	{
		memcpy(this->m_data[floorIndex], vector, m_size * sizeof(double));
	}
	double WindForces::getTimeStep()
	{
		return m_timeStep;
	}

	void WindForces::setTimeStep(double timeStep)
	{
		m_timeStep = timeStep;
	}
}
