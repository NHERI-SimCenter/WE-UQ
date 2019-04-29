#include <vector> 

class WindForces
{
 public:
  WindForces(int numFloors, int size);
  ~WindForces();
  WindForces(const WindForces& other);
  std::vector<double> getFloorForces(int i);
  void setFloorForces(int i, const double* vector);
  double getTimeStep();
  void setTimeStep(double timeStep);
  
 private:
  double** m_data;
  int m_size;
  int m_numFloors;
  double m_timeStep;
};


