#ifndef _FLOOR_FORCES_H_
#define _FLOOR_FORCES_H_

#include <functional>
#include <string>
#include <tuple>
#include <vector>
#include <nlohmann_json/json.hpp>
#include <json_object.h>
#include "function_dispatcher.h"
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
 * @param[in] force_conversion Force conversion factor
 * @return Returns a tuple containing a vector of the static floor wind loads
 *         and a JSON object containing the event details for the dynamic floor
 *         wind loads
 */
std::function<std::tuple<std::vector<double>, nlohmann::json>(
    std::string, double, double, double, double, unsigned int, double, double)>
    wittig_sinha = [](std::string exposure_category, double gust_speed,
                      double drag_coeff, double width, double height,
                      unsigned int num_floors, double total_time,
                      double force_conversion)
    -> std::tuple<std::vector<double>, nlohmann::json> {
  double density = 1.226;
  auto wind_model =
      WindGenerator("WittigSinhaDiscreteFreqWind", exposure_category,
                    gust_speed, height, num_floors, total_time);
  // Calculate floor heights
  auto heights = std::vector<double>(num_floors);
  heights[0] = height / num_floors;

  for (unsigned int i = 1; i < heights.size(); ++i) {
    heights[i] = heights[i - 1] + height / num_floors;
  }

  // Get dynamic wind velocities
  auto dynamic_wind =
      wind_model.generate_time_history("DynamicWindSpeed").get_library_json();

  // Get static wind velocities
  std::vector<double> static_wind(num_floors);

  unsigned int counter = 0;  
  for (nlohmann::json::iterator it =
           dynamic_wind["Events"][0]["pattern"].begin();
       it != dynamic_wind["Events"][0]["pattern"].end(); ++it) {
    static_wind[counter] = it->at("profileVelocity");
    counter++;
  }

  // Iterate over floor dynamic velocity time histories and find dynamic wind
  // force
  counter = 0;
  for (nlohmann::json::iterator it =
           dynamic_wind["Events"][0]["timeSeries"].begin();
       it != dynamic_wind["Events"][0]["timeSeries"].end(); ++it) {
    std::vector<double> velocity = it->at("data");

    // Calculate dynamic wind force
    for (auto& vel : velocity) {
      vel = force_conversion * density * drag_coeff * vel *
            static_wind[counter] * width * heights[counter] / num_floors;
    }
    // Set data to be wind force
    it->at("data") = velocity;
    counter++;
  }
  
  // Calculate static wind force
  for (unsigned int i = 0; i < static_wind.size(); ++i) {
    static_wind[i] = force_conversion * 0.5 * density * drag_coeff *
                     static_wind[i] * static_wind[i] * width *
                     heights[i] / num_floors;
  }

  return std::make_tuple(static_wind, dynamic_wind);
};

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
 * @param[in] force_conversion Force conversion factor
 * @param[in] seed Seed to use in random number generator
 * @return Returns a tuple containing a vector of the static floor wind loads
 *         and a JSON object containing the event details for the dynamic floor
 *         wind loads
 */
std::function<std::tuple<std::vector<double>, nlohmann::json>(
    std::string, double, double, double, double, unsigned int, double, double,
    int)>
    wittig_sinha_seed =
        [](std::string exposure_category, double gust_speed, double drag_coeff,
           double width, double height, unsigned int num_floors,
           double total_time, double force_conversion,
           int seed) -> std::tuple<std::vector<double>, nlohmann::json> {
  double density = 1.226;
  auto wind_model =
      WindGenerator("WittigSinhaDiscreteFreqWind", exposure_category,
                    gust_speed, height, num_floors, total_time, seed);
  // Calculate floor heights
  auto heights = std::vector<double>(num_floors);
  heights[0] = height / num_floors;

  for (unsigned int i = 1; i < heights.size(); ++i) {
    heights[i] = heights[i - 1] + height / num_floors;
  }

  // Get dynamic wind velocities
  auto dynamic_wind =
      wind_model.generate_time_history("DynamicWindSpeed").get_library_json();

  // Get static wind velocities
  std::vector<double> static_wind(num_floors);

  unsigned int counter = 0;  
  for (nlohmann::json::iterator it =
           dynamic_wind["Events"][0]["pattern"].begin();
       it != dynamic_wind["Events"][0]["pattern"].end(); ++it) {
    static_wind[counter] = it->at("profileVelocity");
    counter++;
  }

  // Iterate over floor dynamic velocity time histories and find dynamic wind
  // force
  counter = 0;
  for (nlohmann::json::iterator it =
           dynamic_wind["Events"][0]["timeSeries"].begin();
       it != dynamic_wind["Events"][0]["timeSeries"].end(); ++it) {
    std::vector<double> velocity = it->at("data");

    // Calculate dynamic wind force
    for (auto& vel : velocity) {
      vel = force_conversion * density * drag_coeff * vel *
            static_wind[counter] * width * heights[counter] / num_floors;
    }
    // Set data to be wind force
    it->at("data") = velocity;
    counter++;
  }

  // Calculate static wind force
  for (unsigned int i = 0; i < static_wind.size(); ++i) {
    static_wind[i] = force_conversion * 0.5 * density * drag_coeff *
                     static_wind[i] * static_wind[i] * width *
                     heights[i] / num_floors;
  }

  return std::make_tuple(static_wind, dynamic_wind);
};
}  // namespace floor_forces

#endif // _FLOOR_FORCES_H_
