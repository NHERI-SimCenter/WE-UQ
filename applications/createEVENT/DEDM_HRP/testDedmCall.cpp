int 
callDEDM_HRP(double shpValue,   // Cross-sectional shape (model):  1.00, 0.50, 0.33
	     double hValue,     // Model height (1,2,3,4,5): 0.1,0.2,0.3,0.4,0.5
	     int expCond,       // Exposure condition: 4,6
	     double timeValue,  // Averaging time Time_value 3600
	     double uH,         // Mean wind velocity at top (Ultimate limit state 700yr) m/s
	     double uHHab,      // Mean wind velocity at top (Habitability 10 yr) m/s
	     double B, double D, double H, // building width, depth, and height in METRE
	     int nFloor,        // number of floors
	     const char *outputFilename);

/* 1 0.2 6 3600 100 75 2.54 2.54 5.08 1 */
int main(int argc, char *argv[])
{
  return callDEDM_HRP(1.0, 0.2, 4, 3600, 53, 34, 40, 40, 200, 2, "testOfDEDM_HRP.mat");
}
