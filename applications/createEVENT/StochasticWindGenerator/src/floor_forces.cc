#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <json_object.h>
#include <nlohmann_json/json.hpp>
#include "function_dispatcher.h"
#include "floor_forces.h"
#include "wind_generator.h"

static DispatchRegister<std::tuple<std::vector<double>, nlohmann::json>,
                        std::string, double, double, double, double,
                        unsigned int, double>
    wittig_sinha_function("WittigSinha1975", floor_forces::wittig_sinha);

namespace floor_forces {
  
std::function<std::tuple<std::vector<double>, nlohmann::json>(
    std::string, double, double, double, double, unsigned int, double)>
    wittig_sinha = [](std::string exposure_category, double gust_speed,
                      double drag_coeff, double width, double height,
                      unsigned int num_floors, double total_time)
    -> std::tuple<std::vector<double>, nlohmann::json> {
  double density = 1.226;
  auto wind_model = WindGenerator("WittigSinha", exposure_category, gust_speed,
                                  height, num_floors, total_time);
  // Calculate floor heights
  heights = std::vector<double>(num_floors);
  heights[0] = height / num_floors;

  for (unsigned int i = 1; i < heights.size(); ++i) {
    heights[i] = heights[i - 1] + height / num_floors;
  }

  // Get dynamic wind velocities
  auto dynamic_wind = (wind_model->generate_time_history("DynamicWindSpeed"))
                          .get_library_json();

  // Get static wind velocities 
  std::vector<double> static_wind;
  Dispatcher<double, const std::string &, const std::vector<double> &, double,
             double, std::vector<double> &>::instance()
      ->dispatch("ExposureCategoryVel", exposure_category, heights, 0.4,
                 gust_speed, static_wind);

  // Iterate over floor dynamic velocity time histories and find dynamic wind
  // force
  unsigned int counter = 0;
  for (json::iterator it = dynamic_wind["Events"][0]["timeSeries"].begin();
       it != dynamic_wind["Events"][0]["timeSeries"].end(); ++it) {
    std::vector<double> velocity = it->at("data");

    // Calculate dynamic wind force
    for (auto &vel : velocity) {
      vel = density * drag_coeff * vel * static_wind[counter] * width * height /
            num_floors;
    }
    // Set data to be wind force
    it->at("data") = velocity;
    counter++;
  }

  // Calculate static wind force
  for (unsigned int i = 0; i < static_wind.size(); ++i) {
    static_wind[i] = 0.5 * density * drag_coeff * static_wind[i] *
                     static_wind[i] * width * height / num_floors;
  }

  return std::make_tuple(static_wind, dynamic_wind);
}

std::function<std::tuple<std::vector<double>, nlohmann::json>(
							      std::string, double, double, double, double, unsigned int, double, int)>
    wittig_sinha = [](std::string exposure_category, double gust_speed,
                      double drag_coeff, double width, double height,
                      unsigned int num_floors, double total_time, int seed)
    -> std::tuple<std::vector<double>, nlohmann::json> {
  double density = 1.226;
  auto wind_model = WindGenerator("WittigSinha", exposure_category, gust_speed,
                                  height, num_floors, total_time, seed);
  // Calculate floor heights
  heights = std::vector<double>(num_floors);
  heights[0] = height / num_floors;

  for (unsigned int i = 1; i < heights.size(); ++i) {
    heights[i] = heights[i - 1] + height / num_floors;
  }

  // Get dynamic wind velocities
  auto dynamic_wind = (wind_model->generate_time_history("DynamicWindSpeed"))
                          .get_library_json();

  // Get static wind velocities 
  std::vector<double> static_wind;
  Dispatcher<double, const std::string &, const std::vector<double> &, double,
             double, std::vector<double> &>::instance()
      ->dispatch("ExposureCategoryVel", exposure_category, heights, 0.4,
                 gust_speed, static_wind);

  // Iterate over floor dynamic velocity time histories and find dynamic wind
  // force
  unsigned int counter = 0;
  for (json::iterator it = dynamic_wind["Events"][0]["timeSeries"].begin();
       it != dynamic_wind["Events"][0]["timeSeries"].end(); ++it) {
    std::vector<double> velocity = it->at("data");

    // Calculate dynamic wind force
    for (auto &vel : velocity) {
      vel = density * drag_coeff * vel * static_wind[counter] * width * height /
            num_floors;
    }
    // Set data to be wind force
    it->at("data") = velocity;
    counter++;
  }

  // Calculate static wind force
  for (unsigned int i = 0; i < static_wind.size(); ++i) {
    static_wind[i] = 0.5 * density * drag_coeff * static_wind[i] *
                     static_wind[i] * width * height / num_floors;
  }

  return std::make_tuple(static_wind, dynamic_wind);
}
}
