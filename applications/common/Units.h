#ifndef UNITS_H
#define UNITS_H

//The Units namespace contains method and data types to help handle unit conversion
namespace Units
{

enum class LengthUnit {
  Meter = 1,
  Inch = 2,
  Centimeter = 3,
  Millimeter = 4,
  Foot = 5,
  Unknown = 0
};
enum class TimeUnit {
  Second = 1,
  Minute = 2,
  Hour = 3,
  Unknown = 0
};
enum class ForceUnit {
  Kips = 1,
  Pounds = 2,
  Newton = 3,
  Kilonewton = 4,
  Unknown = 0
};

//This struct encapsulates a unit system that would have units set for different quantities
struct UnitSystem {
  LengthUnit lengthUnit = LengthUnit::Unknown;
  TimeUnit timeUnit = TimeUnit::Unknown;
  ForceUnit forceUnit = ForceUnit::Unknown;
};

//This method parses a string int a length unit enumerator
LengthUnit ParseLengthUnit(const char* lengthUnitString);

//This method parses a string int a time unit enumerator
TimeUnit ParseTimeUnit(const char* timeUnitString);

// This method parses a string to a force enumerator
ForceUnit ParseForceUnit(const char *forceUnit);

//This method finds the convertion factor from one length unit to another
double GetLengthFactor(UnitSystem& fromUnit, UnitSystem& toUnit);

//This method finds the convertion factor from one time unit to another
double GetTimeFactor(UnitSystem& fromUnit, UnitSystem& toUnit);

//This method finds the convertion factor from one acceleration unit to another
double GetAccelerationFactor(UnitSystem& fromUnit, UnitSystem& toUnit);

// This method finds the conversion factor from one force unit to another
double GetForceFactor(const UnitSystem &fromUnit, const UnitSystem &toUnit);

//This method will return gravity in the provided unit system (e.g. BIM Units)
double GetGravity(UnitSystem& unit);

}

#endif
