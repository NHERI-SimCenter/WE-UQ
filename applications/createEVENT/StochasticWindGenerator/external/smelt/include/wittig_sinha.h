#ifndef _WITTIG_SINHA_H_
#define _WITTIG_SINHA_H_

#include <string>
#include <vector>
#include <Eigen/Dense>
#include "json_object.h"
#include "stochastic_model.h"

namespace stochastic {

/**
 * Stochastic model for generating wind loads using discrete frequency
 * functions with Cholesky decomposition and FFTs, as described in
 * Wittig & Sinha (1975), "Simulation of multicorrelated random
 * processes using the FFT algorithm"
 */
class WittigSinha : public StochasticModel {
 public:
  /**
   * @constructor Default constructor
   */
  WittigSinha() = default;

  /**
   * @constructor Construct wind load generator based on model input parameters
   * using exposure category-based velocity profile. Divides building height
   * equally by number of floors, providing time histories at floors.
   * @param[in] exposure_category Exposure category based on ASCE-7
   * @param[in] gust_speed Gust speed of wind in mph
   * @param[in] height Building height
   * @param[in] num_floors Number of floors in building
   * @param[in] total_time Total time desired for time history
   */
  WittigSinha(std::string exposure_category, double gust_speed,
              double height, unsigned int num_floors, double total_time);

  /**
   * @constructor Construct wind load generator based on model input parameters
   * using exposure category-based velcoty profile with specified seed value.
   * Divides building height equally by number of floors, providing time
   * histories at floors.
   * @param[in] exposure_category Exposure category based on ASCE-7
   * @param[in] gust_speed Gust speed of wind in mph
   * @param[in] height Building height
   * @param[in] num_floors Number of floors in building
   * @param[in] total_time Total time desired for time history
   * @param[in] seed_value Value to seed random variables with to ensure
   *                       repeatability
   */
  WittigSinha(std::string exposure_category, double gust_speed,
              double height, unsigned int num_floors, double total_time,
              int seed_value);

  /**
   * @constructor Construct wind load generator based on model input parameters
   * using exposure category-based velocity profile at specific horizontal and
   * vertical locations.
   * @param[in] exposure_category Exposure category based on ASCE-7
   * @param[in] gust_speed Gust speed of wind in mph
   * @param[in] heights Vector of heights at which to calculate time histories
   * @param[in] x_locations Vector of x locations at which to calculate time histories
   * @param[in] y_locations Vector of y locations at which to calculate time histories
   * @param[in] total_time Total time desired for time history
   */
  WittigSinha(std::string exposure_category, double gust_speed,
              const std::vector<double>& heights,
              const std::vector<double>& x_locations,
              const std::vector<double>& y_locations, double total_time);

  /**
   * @constructor Construct wind load generator based on model input parameters
   * using exposure category-based velocity profile at specific horizontal and
   * vertical locations with specified seed value.
   * @param[in] exposure_category Exposure category based on ASCE-7
   * @param[in] gust_speed Gust speed of wind in mph
   * @param[in] heights Vector of heights at which to calculate time histories
   * @param[in] x_locations Vector of x locations at which to calculate time histories
   * @param[in] y_locations Vector of y locations at which to calculate time histories
   * @param[in] total_time Total time desired for time history
   * @param[in] seed_value Value to seed random variables with to ensure
   *                       repeatability
   */
  WittigSinha(std::string exposure_category, double gust_speed,
              const std::vector<double>& heights,
              const std::vector<double>& x_locations,
              const std::vector<double>& y_locations, double total_time,
              int seed_value);

  /**
   * @destructor Virtual destructor
   */
  virtual ~WittigSinha() {};
  
  /**
   * Delete copy constructor
   */
  WittigSinha(const WittigSinha&) = delete;

  /**
   * Delete assignment operator
   */
  WittigSinha& operator=(const WittigSinha&) = delete;

  /**
   * Generate wind velocity time histories based on Wittig & Sinha (1975) model
   * with provided inputs and store outputs as JSON object
   * @param[in] event_name Name to assign to event
   * @param[in] units Indicates that time histories should be returned in
   *                  units of ft/s. Defaults to false where time histories
   *                  are returned in units of m/s
   * @return JsonObject containing loading time histories
   */
  utilities::JsonObject generate(const std::string& event_name,
                                 bool units = false) override;

  /**
   * Generate wind velocity time histories based on Wittig & Sinha (1975) model
   * with provided inputs and write results to file in JSON format
   * @param[in] event_name Name to assign to event
   * @param[in, out] output_location Location to write outputs to
   * @param[in] units Indicates that time histories should be returned in
   *                  units of ft/s. Defaults to false where time histories
   *                  are returned in units of m/s
   * @return Returns true if successful, false otherwise
   */
  bool generate(const std::string& event_name,
                const std::string& output_location, bool units = false) override;

  /**
   * Calculate the cross-spectral density matrix 
   * @param[in] frequency Frequency at which to calculate cross-spectral density
   * @return Matrix containing cross-spectral density functions
   */
  Eigen::MatrixXd cross_spectral_density(double frequency) const;

  /**
   * Generate matrix of complex random number from standard normal distribution scaled
   * by lower Cholesky decomposition of the cross-spectral density matrix
   * @return A matrix containing complex random numbers
   */
  Eigen::MatrixXcd complex_random_numbers() const;

  /**
   * Generate velocity time histories at vertical location specified
   * @param[in] random_numbers Matrix of complex random numbers to use for
   *                           velocity time history generation
   * @param[in] column_index Index for column to use in input random numbers
   *                         matrix
   * @param[in] units Indicates that time histories should be returned in
   *                  units of ft/s. Otherwise time histories are returned
   *                  in units of m/s
   * @return Vector containing velocity time history for vertical location
   *         requested
   */
  std::vector<double> gen_location_hist(const Eigen::MatrixXcd& random_numbers,
                                        unsigned int column_index,
                                        bool units) const;

 private:
  std::string exposure_category_; /**< Exposure category for building based on ASCE-7 */
  double gust_speed_; /**< Gust speed for wind */
  double bldg_height_; /**< Height of building */
  unsigned int num_floors_; /**< Number of floors */
  int seed_value_; /**< Integer to seed random distributions with */
  std::vector<double> heights_; /**< Locations along building height at which
                                   velocities are generated */
  std::vector<double> local_x_; /**< Locations along local x-axis at which to
                                  generate velocities */
  std::vector<double> local_y_; /**< Locations along local y-axis at which to
                                  generate velocities */
  double freq_cutoff_; /**< Cut-off frequency */
  double time_step_; /**< Time step in time histories */
  unsigned int num_times_; /**< Total number of time steps */
  unsigned int num_freqs_; /**< Total number of frequency steps */
  std::vector<double> frequencies_; /**< Range of frequencies */
  std::vector<double> wind_velocities_; /**< Vertical wind velocity profile */
  double friction_velocity_; /**< Friction velocity */
};
}  // namespace stochastic

#endif  // _WITTIG_SINHA_H_
