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
                        unsigned int, double, double>
    wittig_sinha_function("WittigSinha1975", floor_forces::wittig_sinha);

static DispatchRegister<std::tuple<std::vector<double>, nlohmann::json>,
                        std::string, double, double, double, double,
                        unsigned int, double, double, int>
    wittig_sinha_function_seed("WittigSinha1975",
                               floor_forces::wittig_sinha_seed);
