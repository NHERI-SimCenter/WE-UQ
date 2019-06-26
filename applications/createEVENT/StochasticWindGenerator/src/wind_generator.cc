#include <memory>
#include <string>
#include <json_object.h>
#include <factory.h>
#include <stochastic_model.h>
#include "wind_generator.h"

WindGenerator::WindGenerator(std::string model_name,
                             std::string exposure_category, double gust_speed,
                             double height, unsigned int num_floors,
                             double total_time) {
  wind_model_ = Factory<stochastic::StochasticModel, std::string, double,
                        double, unsigned int, double>::instance()
                    ->create(model_name, std::move(exposure_category),
                             std::move(gust_speed), std::move(height),
                             std::move(num_floors), std::move(total_time));
}

WindGenerator::WindGenerator(std::string model_name,
                             std::string exposure_category, double gust_speed,
                             double height, unsigned int num_floors,
                             double total_time, int seed_value) {
  wind_model_ = Factory<stochastic::StochasticModel, std::string, double,
                        double, unsigned int, double, int>::instance()
                    ->create(model_name, std::move(exposure_category),
                             std::move(gust_speed), std::move(height),
                             std::move(num_floors), std::move(total_time),
                             std::move(seed_value));
}

utilities::JsonObject
WindGenerator::generate_time_history(const std::string &event_name) {
  return wind_model_->generate(event_name, true);
}
