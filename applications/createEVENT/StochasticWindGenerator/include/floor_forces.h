#ifndef _FLOOR_FORCES_H_
#define _FLOOR_FORCES_H_

#include <functional>
#include <string>
#include <tuple>
#include <vector>
#include <nlohmann_json/json.hpp>
#include <json_object.h>
#include "wind_generator.h"

/**
 * Floor force calculation functionality
 */
namespace floor_forces {

/**
 * Calculate static and dynamic floor forces based on Wittig & Sinha 1975 and
 * power law for wind profile
 * @param[in] exposure_category Exposure category based on ASCE-7
 * @param[in] gust_speed Gust speed of wind in mph
 * @param[in] drag_coeff Drag coefficient for building
 * @param[in] width Width of building
 * @param[in] height Building height
 * @param[in] num_floors Number of floors in building
 * @param[in] total_time Total time desired for time history
 * @return Returns a tuple containing a vector of the static floor wind loads
 *         and a JSON object containing the event details for the dynamic floor
 *         wind loads
 */
std::function<std::tuple<std::vector<double>, nlohmann::json>(
    std::string, double, double, unsigned int, double)>
    wittig_sinha;

/**
 * Calculate static and dynamic floor forces for given seed based on Wittig &
 * Sinha 1975 and power law for wind profile
 * @param[in] exposure_category Exposure category based on ASCE-7
 * @param[in] gust_speed Gust speed of wind in mph
 * @param[in] drag_coeff Drag coefficient for building
 * @param[in] width Width of building
 * @param[in] height Building height
 * @param[in] num_floors Number of floors in building
 * @param[in] total_time Total time desired for time history
 * @param[in] seed Seed to use in random number generator
 * @return Returns a tuple containing a vector of the static floor wind loads
 *         and a JSON object containing the event details for the dynamic floor
 *         wind loads
 */
std::function<std::tuple<std::vector<double>, nlohmann::json>(
    std::string, double, double, unsigned int, double)>
    wittig_sinha;

std::function<std::tuple<std::vector<double>, nlohmann::json>(
    std::string, double, double, unsigned int, double, int)>
    wittig_sinha_seed;
};  // namespace floor_forces
}

#endif // _FLOOR_FORCES_H_
