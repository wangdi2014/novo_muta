/**
 * @file infinite_driver.cc
 * @author Melissa Ip
 *
 * This is a test driver file for using the expectation-maximization
 * implementation applied to the trio model.
 *
 * To compile on Herschel and include GSL:
 * c++ -std=c++11 -L/usr/local/lib -lgsl -lgslcblas -lm -I/usr/local/include -o infinite_driver utility.cc read_dependent_data.cc trio_model.cc em_algorithm.cc infinite_driver.cc
 */
#include "em_algorithm.h"


int main() {
  TrioModel params;
  ReadDataVector data = {
  	{40, 0, 0, 0},
  	{40, 0, 0, 0},
  	{40, 0, 0, 0}
  };

  double probability = params.MutationProbability(data);
  double somatic = GetSomaticStatistic(params);
  double germline = GetGermlineStatistic(params);
  double sum = somatic + germline;

  cout << probability << endl;
  cout << somatic << endl;
  cout << germline << endl;
  cout << sum << endl;

  return 0;
}