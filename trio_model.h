/**
 * @file trio_model.h
 * @author Melissa Ip
 *
 * The TrioModel class is used to create an object that contains sequencing
 * read data, estimation parameters, and useful matrices in calculating the
 * probability of de novo mutation.
 *
 * The probability of mutation is calculated using a modified trio model,
 * similar but different to the model described in the following paper:
 * 
 * Cartwright et al.: Family-Based Method for Capturing De Novo Mutations
 * http://www.ncbi.nlm.nih.gov/pmc/articles/PMC3728889/
 *
 * This is the implementation for an improved trio model with
 * Dirichlet-multinomial approximations.
 *
 * If this is on the infinite_sites_model branch, this is the simplified trio
 * model (infinite sites model) with simpler multinomial approximations for
 * practicing implementing the expectation-maximization algorithm.
 *
 * Example usage:
 *
 *   TrioModel params;  // uses default parameters
 *   ReadDataVector data = {  // sequencing data in order: child, mother, father
 *     {30, 0, 0, 0},
 *     {30, 0, 0, 0},
 *     {30, 0, 0, 0}
 *   };
 *
 *   double probability = params.MutationProbability(data);
 *   data.set_germline_mutation_rate(0.000001);
 *   double new_probability = params.MutationProbability(data);
 */
#ifndef TRIO_MODEL_H
#define TRIO_MODEL_H

#include "read_dependent_data.cc"  // FIXME: change to .h


/**
 * TrioModel class header. See top of file for a complete description.
 */
class TrioModel {
 public:
  // Default constructor and constructor to customize parameters.
  TrioModel();
  TrioModel(double population_mutation_rate, double germline_mutation_rate,
            double somatic_mutation_rate, double sequencing_error_rate,
            double dirichlet_dispersion,
            const RowVector4d &nucleotide_frequencies);

  // Calculates probability of mutation given input read data.
  double MutationProbability(const ReadDataVector &data_vec);
  void SetReadDependentData(const ReadDataVector &data_vec);

  // True if the two TrioModel objects are equal to each other.
  bool Equals(const TrioModel &other);

  // Get and set functions.
  double population_mutation_rate();
  void set_population_mutation_rate(double rate);
  double germline_mutation_rate();
  void set_germline_mutation_rate(double rate);
  double homozygous_match();
  double heterozygous_match();
  double no_match();
  double somatic_mutation_rate();
  void set_somatic_mutation_rate(double rate);
  double sequencing_error_rate();
  void set_sequencing_error_rate(double rate);
  double dirichlet_dispersion();
  void set_dirichlet_dispersion(double dispersion);
  RowVector4d nucleotide_frequencies();
  void set_nucleotide_frequencies(const RowVector4d &frequencies);
  bool has_mutation();
  void set_has_mutation(bool has_mutation);
  RowVector16d population_priors_single();
  RowVector256d population_priors();
  Matrix4_16d germline_probability_mat_single();
  Matrix16_256d germline_probability_mat();
  Matrix16_16d somatic_probability_mat();
  Matrix3_16d sequencing_probability_mat();
  Matrix16_4d alphas();
  ReadDependentData* read_dependent_data();

 private:


  // Helper functions for MutationProbability.
  void GermlineTransition(bool is_numerator=false);
  void SomaticTransition(bool is_numerator=false);
  RowVector256d GetRootMat(const RowVector256d &child_germline_probability,
                           const RowVector256d &parent_probability);

  // Calculates probability of allele spectrum given read counts.
  double SpectrumProbability(const RowVector4d &nucleotide_counts);

  // Functions for setting up the model and relevant arrays.
  RowVector256d PopulationPriors();
  Matrix16_16d PopulationPriorsExpanded();
  RowVector16d PopulationPriorsSingle();
  void SetGermlineMutationProbabilities();
  double GermlineMutation(int child_nucleotide_idx, int parent_genotype_idx,
                          bool no_mutation_flag);
  Matrix4_16d GermlineProbabilityMatSingle(bool no_mutation_flag=false);
  Matrix16_256d GermlineProbabilityMat(bool no_mutation_flag=false);
  double SomaticMutation(int nucleotide_idx, int other_nucleotide_idx);
  Matrix16_16d SomaticProbabilityMat();
  Matrix16_16d SomaticProbabilityMatDiag();
  void SequencingProbabilityMat(const ReadDataVector &data_vec);
  Matrix16_4d Alphas();

  // Instance member variables.
  double population_mutation_rate_;
  double homozygous_match_;
  double heterozygous_match_;
  double no_match_;
  double germline_mutation_rate_;
  double somatic_mutation_rate_;
  double sequencing_error_rate_;
  double dirichlet_dispersion_;  // unused
  RowVector4d nucleotide_frequencies_;
  Matrix16_4d alphas_;
  RowVector16d population_priors_single_;  // unused
  RowVector256d population_priors_;
  Matrix4_16d germline_probability_mat_single_;
  Matrix16_256d germline_probability_mat_;
  Matrix16_256d germline_probability_mat_num_;
  Matrix16_16d somatic_probability_mat_;
  Matrix16_16d somatic_probability_mat_diag_;

  // TODO: Update simulation files to use refactored trio model.
  ReadDependentData read_dependent_data_;  // contains TreePeels
};

#endif
