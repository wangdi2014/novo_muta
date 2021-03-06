/*
 * @file counts_probability.cc
 * @author Melissa Ip
 *
 * This parses a simulation file, where the first column represents
 * the index of the trio in the reference TrioVector at 4x coverage, the
 * second column represents the number of random trios that have a mutation,
 * and the third column represents the number of random trios that do not have
 * a mutation. The rowwise sum of the second and third column is the total
 * number of random trios that match the key trio (given its index in the first
 * column). This file is generated by simulation_driver.cc.
 *
 * This calculates the empirical probability for each trio using the following
 * formula:
 *
 * P(mutation|trio) = #trios with mutation / #total trios
 *
 * The probabilities should match the probabilities from the result of
 * MutationProbability from simulation_trio.cc. This prints the probability
 * for each trio on a new line.
 *
 * To compile on Herschel:
 * c++ -std=c++11 -L/usr/local/lib -I/usr/local/include -o counts_probability utility.cc counts_probability.cc
 *
 * To run this file, provide the following command line inputs:
 * ./counts_probability <input>.txt <output>.txt
 */
#include <fstream>
#include <sstream>

#include "utility.h"


int main(int argc, const char *argv[]) {
  if (argc < 3) {
    Die("USAGE: counts_probability <input>.txt <output>.txt");
  }

  const string file_name = argv[1];
  ifstream fin(file_name);
  if (!fin.is_open() || 0 != fin.fail()) {
    Die("Input file cannot be read.");
  }

  const string fout_name = argv[2];
  ofstream fout(fout_name);

  double probability = 0.0;
  vector<double> probabilities;
  int index = 0;  // Currently unused.
  int has_mutation_total = 0;
  int has_no_mutation_total = 0;
  int total_trios = 0;
  string line;

  while (getline(fin, line)) {
    line.erase(remove(line.begin(), line.end(), '\n'), line.end());
    stringstream str(line);
    str >> index;
    str >> has_mutation_total;
    str >> has_no_mutation_total;
    total_trios = has_mutation_total + has_no_mutation_total;
    if (total_trios == 0) {
      probability = 0.0;
    } else {
      probability = (double) has_mutation_total / total_trios;
    }
    probabilities.push_back(probability);
  }
  fin.close();

  ostream_iterator<double> output_iter(fout, "\n");
  copy(probabilities.begin(), probabilities.end(), output_iter);
  fout.close();

  return 0;
}
