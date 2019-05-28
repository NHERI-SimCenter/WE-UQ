#include <chrono>
#include <fstream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <Units.h>
#include <configure.h>
#include <nlohmann_json/json.hpp>
#include "command_parser.h"
#include "wind_generator.h"

using json = nlohmann::json;
typedef std::chrono::duration<
    int, std::ratio_multiply<std::chrono::hours::period, std::ratio<8> >::type>
    Days; /* UTC: +8:00 */

int main(int argc, char** argv) {

  // Read command line arguments
  CommandParser inputs;
  try {
    inputs = CommandParser(argc, argv);    
  } catch (const std::invalid_argument& e) {
    std::cerr << "\nException caught in command line processing: " << e.what() << std::endl;
    return 1;
  }
  
  // Check if help menu requested
  if (inputs.get_help_flag()) {
    return 0;
  }

  // Initialize stochastic load generation library
  config::initialize();

  // Get the building information, units
  Units::UnitSystem bim_units, event_units;
  std::ifstream bim_file(inputs.get_bim_file());
  json input_data;
  bim_file >> input_data;

  bim_units.lengthUnit = Units::ParseLengthUnit(input_data["units"]["length"]);
  event_units.lengthUnit = Units::ParseLengthUnit("in");
  double length_conversion = Units::GetLengthFactor(bimUnits, eventUnits);
  double height = input_data["GeneralInformation"]["height"];
  double width = input_data["GeneralInformation"]["width"];
  unsigned int num_floors = input_data["GeneralInformation"]["stories"];
  // Loop over input events and generate corresponding time histories
  json events_array = json::array();

  try {
    if (num_floors <= 0 || height <= 0.0 || width <= 0.0) {
      throw std::runtime_error(
          "ERROR: In main() of StochasticWindGenerator: Check General "
          "Information inputs. Number of floors, height and width all must be "
          "greater than 0\n");
    }

    for (json::iterator it = input_data["Events"].begin();
         it != input_data["Events"].end(); ++it) {
      if (it->at("type") == "StochasticWindModel-WittigSinha1975") {
        json current_event;
        current_event.emplace("type", "Wind");
        current_event.emplace("subtype", "StochasticWindModel-WittigSinha1975");

        // Random variable flag has NOT been passed
        double drag_coeff = 1.0, gustWindSpeed = 100.0;
        if (!inputs.get_rv_flag()) {
          drag_coeff = it->at("dragCoefficient");
          gust_wind_speed = it->at("gustSpeed");

	  if (drag_coeff <= 0.0 || gust_wind_speed <= 0.0) {
            throw std::runtime_error(
                "ERROR: In main() of StochasticWindGenerator: Check drag "
                "coefficient and gust wind speed; both must be "
                "greater than 0.0\n");
          }
        }

        // CONTINUE HERE TO ADD REST OF EVENT DETAILS
      } else {
        events_array.push_back(*it);
      }
    }
  } catch (const std::exception &e) {
    std::cerr
        << "ERROR: In main() of StochasticWindGenerator: "
        << e.what() << std::endl;
    return 1;
  }

  // Check if random variable flag has been passed
  if (inputs.get_rv_flag()) {
    json event;
    event.emplace("randomVariables", json::array());
    json event_description;
    event_description.emplace("type", "Wind");
    event_description.emplace("subtype", "StochasticWindModel-WittigSinha1975");

    auto pattern = json::object({{"dof", 1},
                                 {"timeSeries", "accel_x"},
                                 {"type", "Value"}});

    event_description.emplace("pattern", json::array({pattern}));
    event.emplace("Events", json::array({event_description}));
    std::ofstream event_file;
    event_file.open(inputs.get_event_file());

    if (!event_file.is_open()) {
      std::cerr << "\nERROR: In main() of StochasticGroundMotion: Could "
                   "not open output location\n";
    }

    // Write prettyfied JSON to file
    event_file << std::setw(4) << event << std::endl;
    event_file.close();

    if (event_file.fail()) {
      std::cerr
          << "\nERROR: In In main() of StochasticGroundMotion:: Error when "
             "closing output location\n";
    }
  // No random variable flag passed, so generate ground motion
  } else {
    std::shared_ptr<EQGenerator> eq_generator;
    json event;
    event.emplace("randomVariables", json::array());

    std::ifstream bim_file(inputs.get_bim_file());
    json input_data;
    bim_file >> input_data;
    
    for (json::iterator it = input_data["Events"].begin();
         it != input_data["Events"].end(); ++it) {
      try {
        // Check seed provided
        if (inputs.seed_provided()) {
          eq_generator = std::make_shared<EQGenerator>(
              inputs.get_model_name(), it->at("momentMagnitude"),
              it->at("ruptureDist"), it->at("vs30"), inputs.get_seed());
        } else {
          // Calculate number of nanoseconds in time to use as seed
          std::chrono::time_point<std::chrono::system_clock> now =
              std::chrono::system_clock::now();
          auto duration = now.time_since_epoch();
          Days days = std::chrono::duration_cast<Days>(duration);
          duration -= days;
          auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
          duration -= hours;
          auto minutes =
              std::chrono::duration_cast<std::chrono::minutes>(duration);
          duration -= minutes;
          auto seconds =
              std::chrono::duration_cast<std::chrono::seconds>(duration);
          duration -= seconds;
          auto milliseconds =
              std::chrono::duration_cast<std::chrono::milliseconds>(duration);
          duration -= milliseconds;
          auto microseconds =
              std::chrono::duration_cast<std::chrono::microseconds>(duration);
          duration -= microseconds;
          auto nanoseconds =
              std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

          eq_generator = std::make_shared<EQGenerator>(
              inputs.get_model_name(), it->at("momentMagnitude"),
              it->at("ruptureDist"), it->at("vs30"), nanoseconds.count());	  
        }

        auto time_history =
            eq_generator->generate_time_history("StochasticMotion")
                .get_library_json();

        if (time_history.at("Events").size() != 1) {
          throw std::runtime_error(
              "ERROR: In main() of StochasticGroundMotion with getRV "
              "flag set: Generated events should have length 1\n");
        }
        auto event_data = time_history.at("Events")[0];

        auto array_entry = json::object(
            {{"name", event_data.at("name")},
             {"type", event_data.at("type")},
             {"dT", event_data.at("dT")},
             {"Data", "Time history generated using " +
                          inputs.get_model_name() + " model"},
             {"numSteps", event_data.at("numSteps")},
             {"timeSeries", json::array({event_data.at("timeSeries")[0]})},
             {"pattern", json::array({event_data.at("pattern")[0]})}});

        auto event_array = json::array();
        event_array.push_back(array_entry);
        event.emplace("Events", event_array);
      } catch (const std::exception& e) {
        std::cerr
            << "ERROR: In main() of StochasticGroundMotion without getRV flag: "
            << e.what() << std::endl;
        return 1;
      }
    }

    // Write prettyfied JSON to file
    std::ofstream event_file;
    event_file.open(inputs.get_event_file());

    if (!event_file.is_open()) {
      std::cerr << "\nERROR: In main() of StochasticGroundMotion: Could "
                   "not open output location\n";
    }
    event_file << std::setw(4) << event << std::endl;
    event_file.close();

    if (event_file.fail()) {
      std::cerr
          << "\nERROR: In In main() of StochasticGroundMotion:: Error when "
             "closing output location\n";
    }
  }
  return 0;
}
