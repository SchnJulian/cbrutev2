//
// Created by Julian Schnabel on 18.02.20.
//

#ifndef CBRUTEV2_GENERATOR_H
#define CBRUTEV2_GENERATOR_H

#include "util.h"


class Generator {
public:
  Generator(int argc, char **argv);

  virtual ~Generator();

  bool start();

  std::size_t get_approximate_memory();

private:
  // Parse arguments
  bool parse_arguments(int argc, char **args);

  // Constant pools
  static constexpr char ALPHABETIC_L[] = "abcdefghijklmnopqrstuvwxyz";
  static constexpr char ALPHABETIC_U[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static constexpr char ALPHABETIC[] =
          "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static constexpr char NUMERIC[] = "0123456789";
  static constexpr char SPECIAL[] = "!\"$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
  static constexpr char ASCII[] =
          "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"$%&'()*"
          "+,-./:;<=>?@[\\]^_`{|}~";
  static constexpr char DEFAULT_FILE_EXTENSION[] = ".txt";

  std::size_t element_count = 0; // Number of possible characters

  // File assets
  FILE *output_file{};
  bool file_mode{};

  bool init_file();

  std::string path;

  void print_comb();

  void print_append();

  void print_prepend();

  void print_append_prepend();

  std::mutex m;
  bool permute = false; // Generate permutations
  std::string permute_str;

  void get_permutations();

  bool running{}; // Indicates whether cbrute is still generating combinations


  // Lengths
  std::size_t length_min = 0;
  std::size_t length_max = 0;

  // Total combination/permutation count
  std::size_t total_N = 0;
  std::size_t todo_print = 0;

  std::size_t get_total_n();

  std::size_t get_single_n(std::size_t length);

  long double approximate_memory = 0;

  // Prints combinations/permutations to console
  bool console_log{};

  // Strings to append to / prepend before
  char *append_str = nullptr;
  bool append = false;
  char *prepend_str = nullptr;
  bool prepend = false;
  // CHARPOOL
  char *char_pool = nullptr;

  std::queue<std::string> combinations;

  void get_comb(); // Wrapper method for get_comb_k
  void get_comb_k(const std::string &prefix, std::size_t k);

  // Benchmark variables
  std::__1::chrono::steady_clock::time_point begin;
  std::__1::chrono::steady_clock::time_point end;
  std::__1::chrono::milliseconds duration{};

  void print_report(std::ostream &os);
};

#endif // CBRUTEV2_GENERATOR_H
