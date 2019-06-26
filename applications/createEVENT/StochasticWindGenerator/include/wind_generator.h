#ifndef _WIND_GENERATOR_H_
#define _WIND_GENERATOR_H_

#include <string>
#include <vector>
#include <smelt/include/json_object.h>
#include <smelt/include/stochastic_model.h>

/**
 * Class for generating time histories for wind events
 */
class WindGenerator {
 public:
  /**
   * @constructor Delete default constructor
   */
  WindGenerator() = delete;

  /**
   * @constructor Construct wind generator for wind loading based on inputs
   * @param[in] model_name Name of stochastic model
   * @param[in] exposure_category Exposure category based on ASCE-7
   * @param[in] gust_speed Gust speed of wind in mph
   * @param[in] height Building height
   * @param[in] num_floors Number of floors in building
   * @param[in] total_time Total time desired for time history
   */
  WindGenerator(std::string model_name, std::string exposure_category,
                double gust_speed, double height, unsigned int num_floors,
                double total_time);

  /**
   * @constructor Construct wind generator for wind loading based on inputs
   * @param[in] model_name Name of stochastic model
   * @param[in] exposure_category Exposure category based on ASCE-7
   * @param[in] gust_speed Gust speed of wind in mph
   * @param[in] height Building height
   * @param[in] num_floors Number of floors in building
   * @param[in] total_time Total time desired for time history
   */
  WindGenerator(std::string model_name, std::string exposure_category,
                double gust_speed, double height, unsigned int num_floors,
                double total_time, int seed);

  /**
   * Generate time history based on model parameters
   * @param[in] event_name Name to use for event
   * @return JsonObject containing loading time histories
   */
  utilities::JsonObject generate_time_history(const std::string& event_name);

 private:
  std::shared_ptr<stochastic::StochasticModel> wind_model_; /**< Stochastic
                                                             model to generate
                                                             earthquake time
                                                             histories */
};

#endif // _WIND_GENERATOR_H_
